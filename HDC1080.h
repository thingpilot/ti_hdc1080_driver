#include "mbed.h"

#ifndef HDC1080_H
#define HDC1080_H



class HDC1080:public I2C {
public:
      
    HDC1080( PinName sda, PinName slc) ;                 // constructor

//    ~HDC1080();                                          // destructor
    
    int ReadSignature(void);
    uint16_t readTemperature( void);                   // to read the Temperature from HDC1080 
    uint8_t readHumidity(void);                       // to read the Humidity from HDC1080
    unsigned long readSerialNumber(void);            
    
protected:
     uint16_t read2Bytes(int chip_addr, int offset);
};

 #endif

 
