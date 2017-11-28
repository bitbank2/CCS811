//
// CCS811 Air quality sensor
//
// Written by Larry Bank - 11/4/2017
// Copyright (c) 2017 BitBank Software, Inc.
// bitbank@pobox.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

static int file_i2c = -1;

//
// Opens a file system handle to the I2C device
// Starts the 'app' in the CCS811 microcontroller
// into continuous mode to read values every second
// Returns 0 for success, 1 for failure
//
int ccs811Init(int iChannel, int iAddr)
{
int i, rc;
unsigned char ucTemp[32];
char filename[32];

	sprintf(filename, "/dev/i2c-%d", iChannel);
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		fprintf(stderr, "Failed to open the i2c bus; run as sudo?\n");
		return 1;
	}

	if (ioctl(file_i2c, I2C_SLAVE, iAddr) < 0)
	{
		fprintf(stderr, "Failed to acquire bus access or talk to slave\n");
		file_i2c = 0;
		return 1;
	}

	ucTemp[0] = 0x20; // HW_ID (read the Hardware ID)
	rc = write(file_i2c, ucTemp, 1);
	i = read(file_i2c, ucTemp, 1);
	if (rc != 1 || i != 1 || ucTemp[0] != 0x81)
	{
		printf("Error, ID doesn't match 0x81; wrong device?\n");
		printf("Value returned = %02x\n", ucTemp[0]);
		return 1;
	}

	ucTemp[0] = 0xf4; // APP_START
	rc = write(file_i2c, ucTemp, 1); // tell it to start running the 'APP'
	if (rc == 1)
	{
		ucTemp[0] = 0x01; // MEAS_MODE 
		ucTemp[1] = 0x10; // constant power mode (001), no interrupts
		rc = write(file_i2c, ucTemp, 2);
		if (rc != 2) {} // suppress compiler warning
	}
	return 0;

} /* ccs811Init() */

//
// Turn off the sensor and close the I2C handle
//
void ccs811Shutdown(void)
{
unsigned char ucTemp[2];
int rc;

	if (file_i2c != -1)
	{
		ucTemp[0] = 0x01; // MEAS_MODE
		ucTemp[1] = 0; // return to idle
		rc = write(file_i2c, ucTemp, 2);
		if (rc != 2) {} // suppress compiler warning
		close(file_i2c);
		file_i2c = -1;
	}
} /* ccs811Shutdown() */

//
// Set the calibration values of temperature and humidity
// to provide more accurate air quality readings
// Temperature in Celcius and Humidity as percent (50 = 50%)
//
int ccs811SetCalibration(float fTemp, float fHumid)
{
unsigned char ucTemp[5];
int i, rc;
	if (file_i2c != -1)
	{
		i = (int)(fHumid * 512.0); // convert to 512th fractions
		ucTemp[0] = 0x05; // environmental data
		ucTemp[1] = (unsigned char)(i >> 8); // high byte
		ucTemp[2] = (unsigned char)i; // low byte

		i = (int)((fTemp  - 25.0) * 512.0); // offset of -25C
		ucTemp[3] = (unsigned char)(i >> 8); // high byte
		ucTemp[4] = (unsigned char)i; // low byte
		rc = write(file_i2c, ucTemp, 5);
		if (rc != 5) {} // suppress compiler warning
		return 1;
	}
	return 0;
} /* ccs811SetCalibration() */

//
// Read the sensor's algorithm values
//
int ccs811ReadValues(int *eCO2, int *TVOC)
{
unsigned char ucTemp[16];
int i, rc;

	if (file_i2c != -1)
	{
		ucTemp[0] = 0x02; // Algorithm data results
		rc = write(file_i2c, ucTemp, 1); // write address of register to read
		i = read(file_i2c, ucTemp, 5); // read results and status
		if (rc != 1 || i != 5)
		{
			return 0; // something went wrong
		}

		if (ucTemp[4] & 1) // error, read the error value
		{
			ucTemp[0] = 0xe0;
			rc = write(file_i2c, ucTemp, 1);
			i = read(file_i2c, ucTemp, 1); // read the error
			printf("error = %02x\n", ucTemp[0]);
			return 0;
		}
		if ((ucTemp[4] & 0x99) == 0x98) // firmware valid and data ready
		{	
			*eCO2 = (ucTemp[0] << 8) + ucTemp[1];
			*TVOC = (ucTemp[2] << 8) + ucTemp[3];
			if (*eCO2 > 2000 || *TVOC > 1200) // invalid values
				return 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
} /* ccs811ReadValues() */
