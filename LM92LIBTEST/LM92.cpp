#include  <stdint.h>
#include  <Energia.h>

#include  "LM92.h"
//#include  "usrlib\Wire.h"

LM92Class::LM92Class( void )
{
  i2c_module= 0;
}

void  LM92Class::begin( void )
{
  i2c.setModule( i2c_module );
  i2c.begin();
}
void  LM92Class::begin( uint8_t ch )
{
  i2c.begin();
}
void  LM92Class::setModule(uint8_t module )
{
  i2c.setModule( module );
  i2c.begin();
}


int16_t  LM92Class::getADC( uint8_t  slave )
{
  int16_t ret = getReg(slave, 0x00);
  i2c_status = ret & 0x07;
  ret = ret >>3;

  return ret;
}

int32_t  LM92Class::getTempValue( uint8_t slave )
{
  int32_t  tempValue;
  int16_t  ret = getADC( slave );
  //(for debug)
  //ret = 0x0820;// 130.0 degC
  //ret = 0x1C90 | 0xF000;// -55.0 degC
  tempValue = (ret * 25)/4;    //25/4 = 625/100;
  
  return tempValue;
}
uint16_t  LM92Class::getStatus(uint8_t slave )
{
  //getADC( slave );
  //i2c_status = 0x07;
  return i2c_status;
}

//Manufacturer's Identification Register
uint8_t  LM92Class::isAvailable(uint8_t slave)
{
  uint16_t id = getReg(slave, 0x07);
  if( id == 0x8001 ) return 1;
  return 0;
}

int16_t  LM92Class::getLow( uint8_t slave )
{
  int16_t ret = getReg(slave, 0x04);
  ret = ret >>3;
  return (ret * 25)/4;
}
void     LM92Class::setLow( uint8_t slave, int16_t value )
{
  int16_t  wk= (value*4)/25;
  wk = (wk<<3)&0xfff8;
  setReg(slave,0x04, wk);
}
int16_t  LM92Class::getHigh( uint8_t slave )
{
  int16_t ret = getReg(slave, 0x05);
  ret = ret >>3;
  return (ret * 25)/4;
}
void     LM92Class::setHigh( uint8_t slave, int16_t value )
{
  int16_t  wk= (value*4)/25;
  wk = (wk<<3)&0xfff8;
  setReg(slave,0x05, wk);
}

int16_t  LM92Class::getCrit( uint8_t slave )
{
  int16_t ret = getReg(slave, 0x03);
  ret = ret >>3;
  return (ret * 25)/4;
}
void     LM92Class::setCrit( uint8_t slave, int16_t value )
{
  int16_t  wk= (value*4)/25;
  wk = (wk<<3)&0xfff8;
  setReg(slave,0x03, wk);
}

uint8_t  LM92Class::getConfig(uint8_t slave)
{
  uint8_t mode =0;

  i2c.beginTransmission(slave);
  i2c.write((byte)0x01);
  i2c.endTransmission();

  delay(40);   

  i2c.requestFrom(slave, (uint8_t)1); 
  while(i2c.available()) {
    mode = i2c.read();   break;
  }
  return mode;

}
void  LM92Class::setConfig(uint8_t slave, uint8_t mode)
{
  i2c.beginTransmission(slave);
  i2c.write((byte)0x01);
  i2c.write((byte)mode);
  i2c.endTransmission();
}


uint16_t  LM92Class::getReg(uint8_t slave, uint8_t reg)
{
  uint16_t ret =0;

  i2c.beginTransmission(slave);
  i2c.write(reg);
  i2c.endTransmission();

  delay(40);   

  i2c.requestFrom(slave, (uint8_t)2); 
  while(i2c.available()) {
    ret = i2c.read();   break;
  }
  ret = ret<<8;
  while( i2c.available() ) {
    ret |= i2c.read();   break;
  }
  return ret;

}
 
 void  LM92Class::setReg(uint8_t slave, uint8_t reg, int16_t value )
{
  i2c.beginTransmission(slave);
  i2c.write(reg);
  i2c.write((value>>8)&0xff);
  i2c.write(reg&0xff);
  i2c.endTransmission();
}
  
