#ifndef  __LM92_H__
#define  __LM92_H__

#include  <stdint.h>

#include  "usrlib\Wire.h"

#define    SENSOR_ADDR1  0x48
#define    SENSOR_ADDR2  0x48+1
#define    SENSOR_ADDR3  0x48+2
#define    SENSOR_ADDR4  0x48+3

class  LM92Class 
{
public:
  LM92Class();
  void  begin( void );
  void  begin( uint8_t ch );
  
  void      setModule( uint8_t module );
  int16_t   getADC( uint8_t  slave );
  uint16_t  getStatus(uint8_t slave );
  int32_t   getTempValue( uint8_t slave );
  uint16_t  getReg(uint8_t slave, uint8_t reg);
  void      setReg(uint8_t slave, uint8_t reg, int16_t value );
  
  uint8_t   isAvailable(uint8_t slave);
  uint8_t   getConfig(uint8_t slave);
  void      setConfig(uint8_t slave, uint8_t mode);
  
  int16_t   getLow( uint8_t slave );
  void      setLow( uint8_t slave, int16_t value );
  int16_t   getHigh( uint8_t slave );
  void      setHigh( uint8_t slave, int16_t value );
  int16_t   getCrit( uint8_t slave );
  void      setCrit( uint8_t slave, int16_t value );
  
private:
    TwoWire  i2c;
    uint8_t  i2c_module;
    uint8_t  i2c_slave;
    uint16_t i2c_status;
  
};


#endif  //__LM92_H__

