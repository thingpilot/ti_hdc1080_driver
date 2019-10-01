/**
  * @file    OPT3001.cpp
  * @version 1.0.0
  * @author  
  * @brief   HDC1080 driver module. Measures Temperature and Humidity.
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
  * float lght_lux=OPT3001.Readlight(); 
  * }
  * @endcode
  */

/** Includes
 */
#include "HDC1080.h"

char Buffer[3];
/** Constructor.
 * 
 * @param sda I2C data line pin
 * @param scl I2C clock line pin
 */
HDC1080::HDC1080(PinName sda, PinName slc) : I2C(sda,slc) 
{
    
    memset(Buffer,'\0',3);                  /*Clear Buffer */
    Buffer[0] = HDC1080_REG_CONFIG;
  //  this->frequency(I2C_FREQ);
    int res = this->write(HDC1080_H_DEFAULT_ADDRESS, Buffer, 2);
    
}
/** This method will be used for reading the Manufacture ID.
 */
int HDC1080::ReadSignature(void)
{ 
    
uint16_t  Manufacturer_ID = read2Bytes(HDC1080_H_DEFAULT_ADDRESS, HDC180_REG_MAN_ID);
    if (Manufacturer_ID == 0) {

        return (int) HDC_CHIP_ERROR;
    } else {  
           return Manufacturer_ID;
    }    
}  

/**Returns the device id (decimal value).
 */
  unsigned long HDC1080::ReadDeviceId(void)
{    
    uint16_t  Device_ID = read2Bytes(HDC1080_H_DEFAULT_ADDRESS, HDC1080_REG_DEV_ID);
    
    
    if (Device_ID == 0) {

        return (int) HDC_CHIP_ERROR;
    } else {  
           
       // pc.printf("Manufacturer_ID  :%x\r\n", (int) Manufacturer_ID);

        return Device_ID;
    } 
}    

/** Relative Humidity for TI HDC1080
 */
 float HDC1080::ReadTemperature()
{
    uint16_t  rawT = read2Bytes(HDC1080_H_DEFAULT_ADDRESS, HDC1080_REG_TEMP_RESULT);
    if (rawT == 0) {
       
       // pc.printf("error in reading  chip Temp\r\n");
       
        return HDC_CHIP_ERROR;
    } else {
        float temp = ((float) rawT / pow(2.0f, 16.0f)) * 165.0f - 40.0f;
        
        printf("Temperature   : %0.3f\r\n", temp);
       // pc.printf("Temperature   : %0.3f\r\n", temp);
        
        return temp;
    }
}      
    
 float HDC1080::ReadHumidity()
{   
    wait_us(15000);
    uint16_t  rawH = read2Bytes(HDC1080_H_DEFAULT_ADDRESS, HDC1080_REG_HUM_RESULT);
    if (rawH == 0) {
       
       // pc.printf("error in reading  chip Temp\r\n");
       
        return HDC_CHIP_ERROR;
    } else {
        float humidity = ((float) rawH / pow(2.0f, 16.0f)) * 100.0f;
        
      //  pc.printf("Humidity   : %0.3f\r\n", humidity);
        
        return humidity;
    }
}   
    
    
    /*Private Member function. Reads 2 bytes */
    
    uint16_t HDC1080::read2Bytes(int chip_addr, int offset)
{
    memset(Buffer,0,3);
    // send chip address onto buss
    Buffer[0] = offset;
    int res =this->write(chip_addr, Buffer, 1);
    if (res != 0) {
       
     //   pc.printf("error Communicating  to chip %d offst=%d\r\n", chip_addr, offset);
       
        return HDC1080_WRITE_FAIL;
    }
    else{

        return  HDC1080_OK;
    }
    // read data from chip
    wait_us(15000); //0.015
    memset(Buffer,0,3);
    res = this->read(HDC1080_H_DEFAULT_ADDRESS, Buffer,2);
    if (res != 0) {
    //    pc.printf("error Communicating to  chip %d offst=%d\r\n", chip_addr, offset);
        return HDC1080_WRITE_FAIL;
    }
    return  Buffer[0] << 8 | Buffer[1];
}
   
   
    //disable write
    HDC1080::~HDC1080(){
        
    }
