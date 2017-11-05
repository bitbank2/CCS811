CCS811 Air Quality Sensor Library
Written by Larry Bank
Copyright (c) 2017 BitBank Software, Inc.

bitbank@pobox.com

A simple C library and test program to initialize and read the values from an
I2C connected CCS811. The I2C interface of the CCS811 seems to have a problem
and results in frequent read errors on the Raspberry Pi boards. The code takes
this into account and returns a status indicating if the value is valid. I
tested this on AllWinner CPUs (Orange Pi boards) and did not experience the
same errors. This leads me to believe that there is a bug in the I2C driver
for the Broadcom SoCs used in the Raspberry Pi.

