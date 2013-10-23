//
//  LM92 lib TEST
//
//  PB3  LM92 SDA
//  PB2  LM92 SCL
/*
 * step 1:
 * copy file [org]'Wire.cpp' and 'Wire.h' to [dest]folder.
 *    [org]:   energia\hardware\lm4f\libraries\Wire
 *   [dest]:   energia\hardware\lm4f\cores\lm4f\usrlib
*/

#include  "LM92.h"
#include  "OLED.h"

LM92Class  lm92;
uint8_t    mode;

OLEDClass  OLED;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin( 38400 );
  lm92.begin();
  OLED.begin();
  
  Serial.print("Manufacturer's Id:0x");
  Serial.println(lm92.getReg(SENSOR_ADDR1,0x07),HEX);
  
  mode = lm92.getConfig(SENSOR_ADDR1);
  
  lm92.setConfig(SENSOR_ADDR1,mode &&~0x01);

  lm92.setCrit(SENSOR_ADDR1,32*100);
  lm92.setHigh(SENSOR_ADDR1,28*100);

  Serial.print("Low Temp:");
  Serial.println(((float)lm92.getLow(SENSOR_ADDR1))/100.0);
  Serial.print("High Temp:");
  Serial.println(((float)lm92.getHigh(SENSOR_ADDR1))/100.0);
  Serial.print("Crit Temp:");
  Serial.println(((float)lm92.getCrit(SENSOR_ADDR1))/100.0);

  tone(BLUE_LED,500,100);
  
}
int32_t  value;
uint8_t  status;

void loop()
{
  // put your main code here, to run repeatedly:
  value = lm92.getTempValue( SENSOR_ADDR1 )/10;
  OLED.dspSetFontSize( FONT_SEG7A );
  OLED.dspSetBackColor( COLOR_BLACK );
  OLED.dspSetForeColor( COLOR_CYAN );
  OLED.dspPutTemp(0,0,value);
  


  status = lm92.getStatus(SENSOR_ADDR1);
  OLED.dspSetFontSize( FONT_SMALL );
  if( status & 0x01 ) {
    OLED.dspSetBackColor( COLOR_RED );
    OLED.dspSetForeColor( COLOR_BLACK );
    OLED.dspPutString(88,10,"L");
  } else {
    OLED.dspSetBackColor( COLOR_BLACK );
    OLED.dspSetForeColor( COLOR_WHITE );
    OLED.dspPutString(88,10,"_");
  }
  if( status & 0x02 ) {
    OLED.dspSetBackColor( COLOR_RED );
    OLED.dspSetForeColor( COLOR_BLACK );
    OLED.dspPutString(88, 0,"H");
  } else {
    OLED.dspSetBackColor( COLOR_BLACK );
    OLED.dspSetForeColor( COLOR_WHITE );
    OLED.dspPutString(88, 0,"_");
  }
  if( status & 0x04 ) {
    OLED.dspSetBackColor( COLOR_RED );
    OLED.dspSetForeColor( COLOR_BLACK );
    OLED.dspPutString(88,20,"C");
  } else {
    OLED.dspSetBackColor( COLOR_BLACK );
    OLED.dspSetForeColor( COLOR_WHITE );
    OLED.dspPutString(88,20,"_");
  }
  
  status = lm92.getConfig(SENSOR_ADDR1);
  OLED.dspSetBackColor( COLOR_BLACK );
  OLED.dspSetForeColor( COLOR_WHITE );
  OLED.dspPutHex(0,25,status);
    
  
  Serial.println( value );
  Serial.println( status );

  tone(BLUE_LED,500,100);
  delay( 500 );
  
}
