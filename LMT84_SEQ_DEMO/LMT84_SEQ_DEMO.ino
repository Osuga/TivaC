/*
 *
 *  LMT84 analog sensor demo
 *
 *
 */

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "Energia.h"

//#define  CH   ADC_CTL_TS    //INTERNAL SENSOR
#define  CH   ADC_CTL_CH2    //INTERNAL SENSOR

uint32_t   adcValue[4];

double TempValueC;

void  adcInit( void )
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    ROM_SysCtlADCSpeedSet(SYSCTL_ADCSPEED_250KSPS); // 250
    ADCResolutionSet(ADC1_BASE, ADC_RES_12BIT );    //12 or 10 bit

    ADCSequenceDisable(ADC1_BASE, 1);
    ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 0, CH);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 1, CH);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 2, CH);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 3, CH | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 1);
}

uint32_t  adcGetValue( void )
{
  ADCIntClear(ADC1_BASE, 1);
  ADCProcessorTrigger(ADC1_BASE, 1);
  while(!ADCIntStatus(ADC1_BASE, 1, false)) ;

  ADCSequenceDataGet(ADC1_BASE, 1, adcValue);

  return (adcValue[0] + adcValue[1] + adcValue[2] + adcValue[3] + 2)/4;
}

void  setup()
{
  adcInit();
  Serial.begin(38400);

}

uint32_t  TempAvg;
uint32_t  mVolt;
void loop()
{  
  TempAvg = adcGetValue();
//  TempValueC = (147.5 - ((75*3.3 * TempAvg)) / 4096);
  mVolt = ((3300 * TempAvg)/4096);
  TempValueC = (1035-mVolt)/5.5;
  
  Serial.print(mVolt);
  Serial.print("[mV] ");
  Serial.print(TempValueC);
  Serial.print("[C]\n");
  
  delay(1000);
}
