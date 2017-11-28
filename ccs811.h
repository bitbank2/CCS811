#ifndef CCS811_H
#define CCS811_H
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

//
// Opens a file system handle to the I2C device
// Starts the 'app' in the CCS811 microcontroller
// into continuous mode to read values every second
// returns 0 for success, 1 for error
//
int ccs811Init(int iChannel, int iAddr);

//
// Set the calibration values of temperature and humidity
// to provide more accurate air quality readings
// Temperature in Celcius and Humidity as percent (50 = 50%)
//
int ccs811SetCalibration(float fTemp, float fHumid);

//
// Read the sensor's algorithm values
//
int ccs811ReadValues(int *eCO2, int *TVOC);

//
// Turn off the sensor and close the I2C handle
//
void ccs811Shutdown(void);

#endif // CCS811_H
