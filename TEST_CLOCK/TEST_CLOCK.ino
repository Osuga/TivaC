//
//    Check System Clock
//
//

#include "Energia.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"



void setup()
{
  Serial.begin(38400);
  
  pinMode( PUSH1, INPUT_PULLUP );  // not use
  pinMode( PUSH2, INPUT_PULLUP );  // not use
    
}

uint32_t system_clock;
uint32_t tick_value;

void loop()
{
  // put your main code here, to run repeatedly:
  system_clock = SysCtlClockGet()/1000;
  
  Serial.println( system_clock );
  tick_value = ROM_SysTickPeriodGet();
  Serial.println( tick_value );
  
  tone( BLUE_LED,500,100);
  delay(1000);
  
}
