#include "mbed.h"
#include "HDC1080.h"

#define     HDC_TEMP_OFF        0x00 //Temperature measurement output
#define     HDC_HUMID_OFF       0x01 //Relative Humidity measurement output
#define     HDC_CONFIG_OFF      0x02 //HDC1080 configuration and status
#define     HDC_MANID_OFF       0xFE //ID of Texas Instruments
#define     HDC_SER_OFF_FIRST   0xFB //First 2 bytes of the serial ID of the part
#define     HDC_SER_OFF_MID     0xFC //Mid 2 bytes of the serial ID of the part
#define     HDC_SER_OFF_LAST    0xFD //Last byte bit of the serial ID of the part
#define     I2C_FREQ            100000
#define     CHIP_ADDRESS        (0x40 << 1)   // left shift 1 bit for 7 bit address required by

 //Shift by one bit to get 7 bit I2C Addrress
char HDC_COMMN = HDC_MANID_OFF;
const float HDC_CHIP_ERROR = -255;
const unsigned long HDC_CHIP_SER_ERROR = 0;
char Buffer[5];
Serial pc(PC_10, PC_11);

HDC1080::HDC1080(PinName sda, PinName slc) : I2C(sda,slc) 
{
    memset(Buffer,'\0',5);
    Buffer[0] = HDC_CONFIG_OFF;
    this->frequency(I2C_FREQ);
    int res = this->write(CHIP_ADDRESS, Buffer, 2);
    pc.printf("HDC Constructor Initialization  : Res =%d\r\n", res);
}

int HDC1080::ReadSignature(void)
{ 
    
uint16_t  Manufacturer_ID = read2Bytes(CHIP_ADDRESS, HDC_MANID_OFF);
    if (Manufacturer_ID == 0) {
     
        pc.printf("Error  reading HDC Manufacturer ID\r\n");
       
        return (int) HDC_CHIP_ERROR;
    } else {  
           
        pc.printf("Manufacturer_ID  :%x\r\n", (int) Manufacturer_ID);
      
        return Manufacturer_ID;
    }    
}  
    
 uint16_t HDC1080::readTemperature()
{
    uint16_t  rawT = read2Bytes(CHIP_ADDRESS, HDC_TEMP_OFF);
    if (rawT == 0) {
       
        pc.printf("error in reading  chip Temp\r\n");
       
        return HDC_CHIP_ERROR;
    } else {
        float temp = ((float) rawT / pow(2.0f, 16.0f)) * 165.0f - 40.0f;
        
        printf("Temperature   : %0.3f\r\n", temp);
        pc.printf("Temperature   : %0.3f\r\n", temp);
        
        return temp;
    }
}   
    
       
    
 uint8_t HDC1080::readHumidity()
{   
    wait_us(15);
    uint16_t  rawH = read2Bytes(CHIP_ADDRESS, HDC_HUMID_OFF);
    if (rawH == 0) {
       
        pc.printf("error in reading  chip Temp\r\n");
       
        return HDC_CHIP_ERROR;
    } else {
        float humidity = ((float) rawH / pow(2.0f, 16.0f)) * 100.0f;
        
        pc.printf("Humidity   : %0.3f\r\n", humidity);
        
        return humidity;
    }
}   
    
    unsigned long HDC1080::readSerialNumber(void)
{    
    wait_us(15); //15ms
    memset(Buffer,0,4);
    Buffer[0] = HDC_MANID_OFF;
    int res = this->write(CHIP_ADDRESS, Buffer, 1);
    if (res != 0) {      
     
      pc.printf("Error writing chip addr res=%d\r\n", res);
    
      return (unsigned long) HDC_CHIP_SER_ERROR;
    }      
 
    wait_us(15); //0.015
    memset(Buffer,0,4);
    res = this->read(CHIP_ADDRESS, Buffer,4);
    if (res != 0) {
     
      pc.printf("Errot reading chip serial res=%d#\r\n", res);
     
      return (unsigned long) HDC_CHIP_SER_ERROR;
    }
      
//    unsigned long rawser = Buffer[0] << 16 | Buffer[1] << 8 | Buffer[0];
      unsigned long rawser = Buffer[2] << 16 | Buffer[1] << 8 | Buffer[0];
  
    pc.printf("Serial Number is =%lu\r\n", rawser);
   
    return rawser;
}
    
    
    //Private Member functions 
    
    uint16_t HDC1080::read2Bytes(int chip_addr, int offset)
{
    memset(Buffer,0,3);
    // send chip address onto buss
    Buffer[0] = offset;
    int res =this->write(chip_addr, Buffer, 1);
    if (res != 0) {
       
        pc.printf("error Communicating  to chip %d offst=%d\r\n", chip_addr, offset);
       
        return 0;
    }
    // read data from chip
    wait_us(15); //0.015
    memset(Buffer,0,3);
    res = this->read(CHIP_ADDRESS, Buffer,2);
    if (res != 0) {
        pc.printf("error Communicating to  chip %d offst=%d\r\n", chip_addr, offset);
        return 0;
    }
    return  Buffer[0] << 8 | Buffer[1];
}
