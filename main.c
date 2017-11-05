// CCS811 Air Quality Sensor demo program
// Copyright (c) 2017 Larry Bank
// email: bitbank@pobox.com
// Project started 11/5/2017
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <ccs811.h>

int main(int argc, char *argv[])
{
int i;
int eCO2, TVOC;


	// Check which I2C channel you're using
	// Raspberry Pi's usually use channel 1
	// Other ARM boards tend to use channel 0
	i = ccs811Init(0, 0x5B);
	if (i != 1)
	{
		return -1; // problem - quit
	}
	printf("CCS811 device successfully opened.\n");
	printf("Allow 48 hours initial burn-in and 20 minutes to warm up (each use)\n");
	usleep(1000000); // wait for data to settle for first read

	for (i=0; i<36000; i++) // read values once every 5 seconds for 30 minutes
	{
		if (ccs811ReadValues(&eCO2, &TVOC))
		{
			printf("eCO2 = %d, TVOC = %d\n", eCO2, TVOC);
		}
		else
			printf("data not ready\n");
		printf("%02d:%02d min:sec have passed\n", (i*5)/60, (i*5) % 60);
		usleep(5000000); // every 5 seconds
	}

return 0;
} /* main() */
