/** 
  * @file    HDC1080.h
  * @version 1.0.0
  * @author  
  * @brief   Header file of the HDC1080 driver module.
  * @date    01/10/2019
  *
  * Example: 
  * code
  * #include "mbed.h"
  * #include "main.h"
  * #include "HDC1080/HDC1080.h"
  *
  * HDC1080 HDC1080(SDA,SCL); 
  *  
  * int main() {
  * float lght_lx=OPT3001.Readlight(); 
  * }
  * @endcode
  */

/** Define to prevent recursive inclusion
 */
#pragma once

/** Includes 
 */
#include "mbed.h"

#ifndef HDC1080_H
#define HDC1080_H

//#define     I2C_FREQ                    100000

/** Register Map.
 *  Device ID Register (offset = 7Fh) [reset = 3001h]
 *  Manufacturer ID Register (offset = 7Eh) [reset = 5449h]
 */
#define     HDC1080_REG_TEMP_RESULT     0x00 //Temperature measurement output
#define     HDC1080_REG_HUM_RESULT      0x01 //Relative Humidity measurement output
#define     HDC1080_REG_CONFIG          0x02 //HDC1080 configuration and status
#define     HDC180_REG_MAN_ID           0xFE //ID of Texas Instruments
#define     HDC1080_REG_DEV_ID          0xFF

/** Default HDC1080 I2C address. This should be set according to the 
 *  configuration of the hardware address pins. 
 */
#define     HDC1080_H_DEFAULT_ADDRESS        (0x40 << 1)  



/** This base class for the HDC1080 will be used to read Manufacture id, serial number
 *  Temperature and Humidity measurements
 */ 
class HDC1080:public I2C {
public:

    /*Status*/
    typedef int HDC1080_Status_t;

        enum 
        {
            HDC1080_OK                            = 0, /**< enum value 1 */
            HDC1080_READ_FAIL                     = 1,
            HDC1080_WRITE_FAIL                    = 2,
            HDC1080_VERIFY_FAIL                   = 3,
            HDC_CHIP_ERROR                        = -255
        };

    /** Constructor.
     * @param sda I2C data line pin
     * @param scl I2C clock line pin 
     */  
    HDC1080( PinName sda, PinName slc) ;                

    /** Destructor. Will disable write_control
     */
    ~HDC1080();                                          
    
    int ReadSignature(void);
    float ReadTemperature( void);                   // to read the Temperature from HDC1080 
    float ReadHumidity(void);                       // to read the Humidity from HDC1080
    unsigned long ReadDeviceId(void);            
    
protected:
     uint16_t read2Bytes(int chip_addr, int offset);
};

 #endif

 
