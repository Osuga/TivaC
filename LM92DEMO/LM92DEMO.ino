//-----------------------------------------------------------------------------------
// I2C LM92 test program
// on TivaC Launchpad
//
//    SDA    PB3
//    SCL     PB2
//
//-----------------------------------------------------------------------------------
#include <Wire.h>

#define    SENSOR1    0x48
#define    SENSOR2    0x48+3


double    Temp1;
double    Temp2;


int    getTemp( int slave )
{
    int    wk;

    Wire.beginTransmission(slave);
    Wire.write(byte(0x00));
    Wire.endTransmission();
    
    delay(60);   

    Wire.requestFrom(slave, 2); 
    while(Wire.available()) {
        wk = Wire.read() <<8;
        break;
    }
    while( Wire.available() ) {
        wk |= Wire.read();
        break;
    }
    wk = wk >>3;    // LM92:13bit
  //wk = wk >>7;    // LM75: 9bit
    return wk;

}

void setup()
{
    Wire.setModule(0);
    Wire.begin();
    
    Serial.begin(38400);
    Serial.print("I2C LM92 TEST program.\n" );

    

}

void loop()
{
   Temp1 = getTemp( SENSOR1 )*0.0625;  // LM92
 //Temp1 = getTemp( SENSOR1 )*0.5;     // LM75
   Serial.print( Temp1 );
   Serial.print( "\n");
  
  delay(1000); 
}


