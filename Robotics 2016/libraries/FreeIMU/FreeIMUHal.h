/*
FreeIMU.h - A libre and easy to use orientation sensing library for Arduino
Copyright (C) 2011 Fabio Varesano <fabio at varesano dot net>

Development of this code has been supported by the Department of Computer Science,
Universita' degli Studi di Torino, Italy within the Piemonte Project
http://www.piemonte.di.unito.it/


This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef FreeIMU_HAL_h
#define FreeIMU_HAL_h

// Uncomment the appropriated version of FreeIMU you are using
//#define FREEIMU_v01
//#define FREEIMU_v02
//#define FREEIMU_v03
//#define FREEIMU_v035
//#define FREEIMU_v035_MS
//#define FREEIMU_v035_BMP
//#define FREEIMU_v04

// 3rd party boards. Please consider donating or buying a FreeIMU board to support this library development.
//#define SEN_10121 //IMU Digital Combo Board - 6 Degrees of Freedom ITG3200/ADXL345 SEN-10121 http://www.sparkfun.com/products/10121
//#define SEN_10736 //9 Degrees of Freedom - Razor IMU SEN-10736 http://www.sparkfun.com/products/10736
//#define SEN_10724 //9 Degrees of Freedom - Sensor Stick SEN-10724 http://www.sparkfun.com/products/10724
//#define SEN_10183 //9 Degrees of Freedom - Sensor Stick  SEN-10183 http://www.sparkfun.com/products/10183
//#define ARDUIMU_v3 //  DIYDrones ArduIMU+ V3 http://store.diydrones.com/ArduIMU_V3_p/kt-arduimu-30.htm or https://www.sparkfun.com/products/11055
#define GEN_MPU6050 // Generic MPU6050 breakout board. Compatible with GY-521, SEN-11028 and other MPU6050 wich have the MPU6050 AD0 pin connected to GND.

// *** No configuration needed below this line ***


#define FREEIMU_LIB_VERSION "20121122"

#define FREEIMU_DEVELOPER "Fabio Varesano - varesano.net"

#if F_CPU == 16000000L
  #define FREEIMU_FREQ "16 MHz"
#elif F_CPU == 8000000L
  #define FREEIMU_FREQ "8 MHz"
#endif


// board IDs
#if defined(FREEIMU_v01)
  #define FREEIMU_ID "FreeIMU v0.1"
#elif defined(FREEIMU_v02)
#define FREEIMU_ID "FreeIMU v0.2"
#elif defined(FREEIMU_v03)
  #define FREEIMU_ID "FreeIMU v0.3"
#elif defined(FREEIMU_v035)
  #define FREEIMU_ID "FreeIMU v0.3.5"
#elif defined(FREEIMU_v035_BMP)
  #define FREEIMU_ID "FreeIMU v0.3.5_BMP"
#elif defined(FREEIMU_v035_MS)
  #define FREEIMU_ID "FreeIMU v0.3.5_MS"
#elif defined(FREEIMU_v04)
  #define FREEIMU_ID "FreeIMU v0.4"
#elif defined(SEN_10121)
  #define FREEIMU_ID "SparkFun 10121"
#elif defined(SEN_10736)
  #define FREEIMU_ID "SparkFun 10736"
#elif defined(SEN_10724)
  #define FREEIMU_ID "SparkFun 10724"
#elif defined(SEN_10183)
  #define FREEIMU_ID "SparkFun 10183"
#elif defined(ARDUIMU_v3)
  #define FREEIMU_ID "DIY Drones ArduIMU+ V3"
#endif



#define HAS_ITG3200() (defined(FREEIMU_v01) || defined(FREEIMU_v02) || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP) || defined(SEN_10121) || defined(SEN_10736) || defined(SEN_10724) || defined(SEN_10183))
#define HAS_ADXL345() (defined(FREEIMU_v01) || defined(FREEIMU_v02) || defined(FREEIMU_v03) || defined(SEN_10121) || defined(SEN_10736) || defined(SEN_10724) || defined(SEN_10183))
#define HAS_BMA180() (defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP))
#define HAS_MPU6050() (defined(FREEIMU_v04) || defined(GEN_MPU6050))
#define HAS_MS5611() (defined(FREEIMU_v035_MS) || defined(FREEIMU_v04))
#define HAS_HMC5883L() (defined(FREEIMU_v01) || defined(FREEIMU_v02) || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP) || defined(FREEIMU_v04) || defined(SEN_10736) || defined(SEN_10724) || defined(SEN_10183)  || defined(ARDUIMU_v3))
#define HAS_MPU6000() (defined(ARDUIMU_v3))

#define IS_6DOM() (defined(SEN_10121) || defined(GEN_MPU6050))
#define IS_9DOM() (defined(FREEIMU_v01) || defined(FREEIMU_v02) || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP) || defined(FREEIMU_v04) || defined(SEN_10736) || defined(SEN_10724) || defined(SEN_10183) || defined(ARDUIMU_v3))
#define HAS_AXIS_ALIGNED() (defined(FREEIMU_v01) || defined(FREEIMU_v02) || defined(FREEIMU_v03) || defined(FREEIMU_v035) || defined(FREEIMU_v035_MS) || defined(FREEIMU_v035_BMP) || defined(FREEIMU_v04) || defined(SEN_10121) || defined(SEN_10736))



#include <Wire.h>
#include "Arduino.h"
#include "calibration.h"

#ifndef CALIBRATION_H
#include <EEPROM.h>
#endif

#define FREEIMU_EEPROM_BASE 0x0A
#define FREEIMU_EEPROM_SIGNATURE 0x19

//#if FREEIMU_VER <= 3
#if HAS_ADXL345()
  #include <ADXL345.h>
  // default I2C 7-bit addresses of the sensors
  #define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW // SDO connected to GND
  //#define FIMU_ADXL345_DEF_ADDR ADXL345_ADDR_ALT_HIGH // SDO connected to GND
#elif HAS_BMA180()
  #include <bma180.h>
  #define FIMU_ACC_ADDR BMA180_ADDRESS_SDO_LOW
  //#define FIMU_ACC_ADDR BMA180_ADDRESS_SDO_HIGH
//#elif FREEIMU_VER == 7
#elif HAS_MPU6050()
  #include <Wire.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_ADDRESS
#elif HAS_MPU6000()
  #include <SPI.h>
  #include "I2Cdev.h"
  #include "MPU60X0.h"
  #define FIMU_ACCGYRO_ADDR MPU60X0_DEFAULT_SS_PIN
#endif


#if HAS_MS5611()
  #include <MS561101BA.h>
  #define FIMU_BARO_ADDR MS561101BA_ADDR_CSB_LOW
  //#define FIMU_BARO_ADDR MS561101BA_ADDR_CSB_HIGH
#endif

#if HAS_ITG3200()
  #include <ITG3200.h>
#endif

#if HAS_HMC5883L()
  #include <HMC58X3.h>
#endif


#define FIMU_BMA180_DEF_ADDR BMA180_ADDRESS_SDO_LOW
#define FIMU_ITG3200_DEF_ADDR ITG3200_ADDR_AD0_LOW // AD0 connected to GND
// HMC5843 address is fixed so don't bother to define it


#endif
