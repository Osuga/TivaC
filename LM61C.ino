//
//  LM61 TempSensor demo
//  on TivaC launchpad  
//		
//
#define    TEMP_ADC_CH   28    //PE2
#define    LED_BLUE      40    //PF2

int16_t  sensorValue;
int32_t  mVolt;
int32_t  mTemp;
double  value;

static const int16_t offset = -5;


void setup()
{
  pinMode( LED_BLUE,OUTPUT );
  Serial.begin(38400);
  sensorValue = analogRead( TEMP_ADC_CH );
  
}

void loop()
{
  // put your main code here, to run repeatedly:
  sensorValue += analogRead( TEMP_ADC_CH );
  sensorValue /= 2;
  mVolt = map(sensorValue+offset, 0, 4096, 0, 3300);
  mTemp = map(sensorValue+offset, 0, 4096, -600, 2700);
  value = ((double)mTemp)/10.0;
  
  Serial.print( "Output:");
  Serial.print( mVolt );
  Serial.print( "mV ");
  
  Serial.print( "Temp:");
  Serial.print( value );
  Serial.print( "\n" );
  
  tone( LED_BLUE,500,50 );
  
  delay( 1500 );
  
  
}
