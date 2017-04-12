/**********************************************************************************************************************
* File Name:		main.h
* Author:			EE391 Students & Professors
* Description:  	Used to set Config bits, #defines, variables, and function prototypes
* Version:			v1.0 - 03/10/17 - A.R.Taylor - Initial release version
**********************************************************************************************************************/
//#include <pic16f1829.h>				// PIC16F1829 model specific header
#include <htc.h>						// PIC16F1829 model specific header

//CONFIG WORD 1 register bits for the device are set here...
__CONFIG(	FCMEN_OFF		&			//Fail-Safe Clock Monitor Enable bit - FCM is OFF
		 	IESO_OFF		&			//Internal External Switchover bit - switchover mode is disabled
			CLKOUTEN_OFF	&			//CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
			BOREN_OFF		&			//Brown-Out Reset Selection bits - BOR is OFF
			CPD_OFF			&			//Data Code Protect bit - EEPROM memory protection is disabled
			CP_OFF			&			//Code Protection bit - Program memory protection is disabled
			MCLRE_OFF		&			//MCLR Pin Function Select bit - RA3 is input, MCLRE is tied to VDD
			PWRTE_OFF		&			//Power-up Timer Enable bit - PWRT is disabled
//			WDTE_SWDTEN		&			//WDT controlled by the SWDTEN bit in the WDTCON register
			WDTE_OFF		&			//WDT Enable bit - WDT disabled; can be enabled with SWDTEN
			FOSC_INTOSC);				//Oscillator Selection bits - Internal RC oscillator, RA4 & RA5 are IO pins

//CONFIG WORD 2 register bits for the device are set here...
__CONFIG(	LVP_OFF			&			//High-Voltage on MCLR must be used for programming
//			DEBUG_ON		&			//ICD is disabled; ICSPDAT & ICSPCLK are general I/O pins
			BORV_HI			&			//Brown-out Reset Voltage (VBOR) set to 2.7 V
//			BORV_LO			&			//Brown-out Reset Voltage (VBOR) set to 1.9 V
			STVREN_ON		&			//Stack Overflow or Underflow will cause a Reset
			PLLEN_OFF		&			//4x PLL Disabled; can be controlled via software
			WRT_OFF			);			//EEPROM Write-Protect is OFF

/*********************************************************************************************************************/
//Defines for compiler to use in main program...
#define ADC_GAIN			5.0/1024.0					//Conversion ratio to put ADC values into units of VOLTs

//These are used to make the ADC switch-case more human-readable
/*THESE SHOULD BE CHANGED BASED ON YOUR SCHEMATIC DESIGN*/
#define Ch_vBat				2
#define Ch_Temp				0
#define Ch_AccX				10
#define Ch_AccY				3


/*********************************************************************************************************************/
//Structures and Unions for custom data types
struct ADCval //These are used to hold ADC conversion values
{	unsigned int	raw;			// Hold the raw 10-bit ADC data
	float			volt;			// The voltage read on the pin
	float			RWV;			// The converted real-world value
};	

union CharFloat //These are used for Serial Port data manipulation
{	signed int 		word;			// Word value
	unsigned char 	byte[2];		// Byte values
};

/*********************************************************************************************************************/
//Function Prototypes
void main(void);								//Program entry point
void interrupt IntServRoutine(void);			//ISR entry point

void configSysClock(void);						//Sets the MCU clock & frequency
void configIOports(void);						//Configures I/O port directions
void configTimer2(void);						//Sets Timer2 as interrupt source
void configADC(void);							//Initializes the A/D Converter
void configSCI(void);							//Configures the Serial Port for Comm.


/**************************************************** End of File ****************************************************/

