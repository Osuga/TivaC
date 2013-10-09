/*
 ************************************************************************
 *	wiring.c
 *
 *	Arduino core files for MSP430
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */
#include "Energia.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


#define	TICKS_PER_SECOND	100

static unsigned long milliseconds = 0;

void timerInit()
{

#if F_CPU >= 80000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 50000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 40000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 25000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 16200000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);	//NOT PLL
#elif F_CPU >= 16100000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_OSC_INT|
                SYSCTL_OSC_MAIN);	//NOT PLL, INT OSC
#elif F_CPU >= 16000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_12_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 10000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_20|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#elif F_CPU >= 8000000
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_25|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#else
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                SYSCTL_OSC_MAIN);
#endif

    //
    //  SysTick is used for delay() and delayMicroseconds()
    //
//	ROM_SysTickPeriodSet(0x00FFFFFF);
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / TICKS_PER_SECOND);
    ROM_SysTickEnable();

    //
    //Initialize Timer5 to be used as time-tracker since beginning of time
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); //not tied to launchpad pin
    ROM_TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC_UP);

    ROM_TimerLoadSet(TIMER5_BASE, TIMER_A, ROM_SysCtlClockGet()/1000);

    ROM_IntEnable(INT_TIMER5A);
    ROM_TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

    ROM_TimerEnable(TIMER5_BASE, TIMER_A);

    ROM_IntMasterEnable();

}

unsigned long micros(void)
{
    unsigned int conMz = ROM_SysCtlClockGet()/1000/1000;
//	return (milliseconds * 1000) + (HWREG(TIMER5_BASE + TIMER_O_TAV) / 80);
	return (milliseconds * 1000) + (HWREG(TIMER5_BASE + TIMER_O_TAV) / conMz);
}

unsigned long millis(void)
{
	return milliseconds;
}

/* Delay for the given number of microseconds.  Assumes a 1, 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
    unsigned int conMz = ROM_SysCtlClockGet()/1000/1000;
	volatile unsigned long elapsedTime;
	unsigned long startTime = HWREG(NVIC_ST_CURRENT);
	
	do{
		elapsedTime = startTime-(HWREG(NVIC_ST_CURRENT) & 0x00FFFFFF);
	}
//	while(elapsedTime <= us*80);
	while(elapsedTime <= us*conMz );
}

void delay(uint32_t milliseconds)
{
		unsigned long i;
		for(i=0; i<milliseconds; i++){
			delayMicroseconds(1000);
		}
}

void Timer5IntHandler(void)
{
    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

	milliseconds++;
}
