#include  <stdio.h>
//
//  analog sensor LMT84 demo
//
uint16_t  sensorValue;
int32_t  mVolt;
int32_t  dTemp;

int32_t  mVolt2;
double  TempValueC;

void setup()
{
  Serial.begin(38400);
  sensorValue = analogRead(A2);
  pinMode( BLUE_LED,OUTPUT);
}


char  str[30];

// Vtemp = 870.6mV - 5.506mV/C*(T-30)-0.00176mV/C*(T-30)
//V = (-5.50 mV/C) x T + 1035 mV
//T = (1035mV -V)/(-5.5mV/C)
void loop()
{
    sensorValue += analogRead(A2);
    sensorValue /=2;
    mVolt = map(sensorValue, 0, 4096, 0, 3300);
   //   mVolt = debugTemp((++idx)&0x07);
//    dTemp = map(mVolt, 182, 1299, 1500, -500)+25;
    dTemp = map(mVolt, 182-3, 1299+3, 1500, -500)+30;
    
    mVolt2 = ((3300 * sensorValue)/4096);
    TempValueC = (1035-mVolt2)/5.5;
    
    snprintf( str, sizeof(str),"map[%5.2lf] cal[%5.2lf]",(double)dTemp/10,TempValueC);
    Serial.println(str);
 
//    snprintf( str, sizeof(str),"map[%04X ] cal[%04X ]",mVolt,mVolt2);
//    Serial.println(str);
 
    tone(BLUE_LED,500,100);
    delay(500);
   
  
}
/*
-46.8
-5.5
3.0
15.8
26.1
81.6
127.3
155.8

*/
// for debug
int32_t  debugTemp( uint16_t mode )
{
  int32_t  ret=0;
  switch (mode) {
    case 0: break;
    case 1: ret = 1299;  break;  //-50
    case 2: ret = 1034;  break;  //  0   (-2.5)
    case 3: ret =  980;  break;  // 10   ( 7.2)
    case 4: ret =  898;  break;  // 25   (21.9)
    case 5: ret =  832;  break;  // 37   (33.7)
    case 6: ret =  476;  break;  //100   (97.5)
    case 7: ret =  183;  break;  //150
    default:  break;
  }
  return ret;
}
