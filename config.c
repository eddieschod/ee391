/*********************************************************************************************************************
* File Name:		config.c
* Author:			EE391 Students & Professors
* Description:  	Used to initialize peripherals of the PIC16 MCU
* Version:			v1.0 - 03/09/17 - A.R.Taylor - Initial release versions
/*********************************************************************************************************************/
//#include <pic16f1825.h>				// PIC16F1825 model specific header
#include <htc.h>						// PIC16F1825 model specific header


/*********************************************************************************************************************/
//   configSysClock() - Initialization code to setup oscillator selection and clock frequency
/*********************************************************************************************************************/
void configSysClock(void)
{	OSCCONbits.SPLLEN		= 0;			//Ensure the 4x PLL is disabled (save power)
//	OSCCONbits.SPLLEN		= 1;			//Enable the 4x PLL for SPEEEEED!
	OSCCONbits.IRCF			= 0b1111;		//Set HF INT OSC to 16MHz HF
//	OSCCONbits.IRCF			= 0b1110;		//Set HF INT OSC to 8MHz / 32MHz (w/PLL)
//	OSCCONbits.IRCF			= 0b1011;		//Set HF INT OSC to 1MHz HF	
	OSCCONbits.SCS			= 0b10;			//Force the use of internal oscillators
//	OSCTUNE					= 12;			//Slight adjust of MF/HF OSC...
	while(!OSCSTATbits.HFIOFS);				//Wait for HF INT OSC to become stable
}


/*********************************************************************************************************************/
//   configTimer2() - Initialization code to setup timer
/*********************************************************************************************************************/
void configTimer2(void)
{	T2CONbits.TMR2ON		= 0;			//Turn OFF Timer
//	T2CONbits.T2CKPS		= 0b00;			//Timer Clock prescaler to 1:1  (16MHz / (4*1) = 4MHz
//	T2CONbits.T2CKPS		= 0b01;			//Timer Clock prescaler to 1:4  (16MHz / (4*4) = 1MHz
	T2CONbits.T2CKPS		= 0b10;			//Timer Clock prescaler to 1:16 (16MHz / (4*16) = 250kHz
//	T2CONbits.T2CKPS		= 0b11;			//Timer Clock prescaler to 1:64  (16MHz / (4*64) = 62.5kHz
	T2CONbits.T2OUTPS		= 0b1001;		//Set Timer output postscaler to 1:10 --> 25kHz
	PR2						= 125;			//Set Timer compare value (freq) (25kHz / 250 = 100Hz)
	TMR2					= 0x00;			//Clear out the value of Timer
	PIR1bits.TMR2IF			= 0;			//Clear the Timer Interrupt Flag
	PIE1bits.TMR2IE			= 1;			//Enable Timer Interrupts
	T2CONbits.TMR2ON		= 1;			//Turn ON Timer
}	


/*********************************************************************************************************************/
//	 configADC() - Configure ADC peripheral device
/*********************************************************************************************************************/
void configADC(void)
{	ADCON0bits.ADON			= 0;			//Ensure ADC module is powered OFF
	ADCON0bits.CHS			= 0b00011;		//Initial channel select set to AN3 (vPV)

//	ADCON1bits.ADFM			= 0;			//ADC results are left-justified
	ADCON1bits.ADFM			= 1;			//ADC results are right-justified

//	ADCON1bits.ADCS			= 0b101;		//Set A/D conversion clock to Fosc/16
//	ADCON1bits.ADCS			= 0b010;		//Set A/D conversion clock to Fosc/32
	ADCON1bits.ADCS			= 0b110;		//Set A/D conversion clock to Fosc/64
//	ADCON1bits.ADCS			= 0b111;		//Set A/D conversion clock to FRC; Tad ~ 1.6us

	ADCON1bits.ADNREF		= 0;			//Negative voltage reference connected to GND
//	ADCON1bits.ADPREF		= 0b11;			//Positive voltage reference connected to FVRef module
	ADCON1bits.ADPREF		= 0b00;			//Positive voltage reference connected to VDD supply

	PIR1bits.ADIF			= 0;			//Clear the ADC interrupt flag
//	PIE1bits.ADIE			= 1;			//Enable ADC interrupts
	ADCON0bits.ADON			= 1;			//Enable the ADC module

//	ADCON0bits.GO			= 1;			//Start first conversion!
}


/*********************************************************************************************************************/
//	 configSCI() - Setup serial port in Async mode; used for debugging
/*********************************************************************************************************************/
void configSCI(void) 
{	TXSTAbits.SYNC			= 0;			//Operate in Async mode
//	TXSTAbits.CSRC			= 1;			//Master Mode; Clock from internal BRG
	TXSTAbits.TX9			= 0;			//Operate in 8-bit mode
	TXSTAbits.SENDB			= 0;			//No Sync Break transmission
	TXSTAbits.BRGH			= 1;			//Run High-Speed Baud Rates

	RCSTAbits.RX9			= 0;			//Eight bit reception
	RCSTAbits.ADDEN			= 0;			//Disables address detection

	BAUDCONbits.SCKP		= 0;			//Clock polarity; Transmit non-inverted data to TX/CK pin
	BAUDCONbits.BRG16		= 0;			//An eight-bit baudrate generator is used
	BAUDCONbits.WUE			= 0;			//Wake-up Enable bit; disabled, reciever operating normally
	BAUDCONbits.ABDEN		= 0;			//Auto-BAUD detect disabled

//	SPBRGL					= 51;			//Low-byte BAUD rate generation; At 16MHz Clk, 51 --> ~19.2kBd
	SPBRGL					= 25;			//Low-byte BAUD rate generation; At 16MHz Clk, 25 --> ~38.6kBd
	SPBRGH					= 0;			//High-byte BAUD rate generation

	PIE1bits.RCIE			= 1;			//Enable SCI Rx interrupts
	RCSTAbits.SPEN			= 1;			//Enable the serial port
	TXSTAbits.TXEN			= 1;			//Enable the Trasmit circuits
	RCSTAbits.CREN			= 1;			//Enable continous recieve; enables reciever
}


/*********************************************************************************************************************/
//   configIOports() - Sets port directions, disable Analog input structure, set default states
/*********************************************************************************************************************/
void configIOports(void)
{	OPTION_REGbits.nWPUEN	= 0;			//Weak pull-ups are enabled by individual WPUx latch values

	/*THESE SHOULD BE CHANGED BASED ON YOUR SCHEMATIC DESIGN*/
	//--------------------------------------CONFIGURE PORT A-----------------------------------------------------------
	TRISAbits.TRISA0		= 1;			//RA0 INPUT: pin not connected, used for debugging
	ANSELAbits.ANSA0		= 1;			//Analog input Enabled
	WPUAbits.WPUA0			= 0;			//Pull-Up Disabled
	
	TRISAbits.TRISA1		= 0;			//RA1 OUTPUT: pin not connected, used for debugging
	ANSELAbits.ANSA1		= 0;			//Analog input Disabled
	WPUAbits.WPUA1			= 0;			//Pull-Up Disabled
	LATAbits.LATA1			= 0;			//Set LOW initially
	
	TRISAbits.TRISA2		= 1;			//RA2 INPUT: analog input for battery voltage sense
	ANSELAbits.ANSA2		= 1;			//Analog input Enabled
	WPUAbits.WPUA2			= 0;			//Pull-Up Disabled

	TRISAbits.TRISA3		= 1;			//RA3 INPUT: push-button switch input
	WPUAbits.WPUA3			= 1;			//Pull-Up Enabled
//	WPUAbits.WPUA3			= 0;			//Pull-Up Disabled
	
	TRISAbits.TRISA4		= 1;			//RA4 INPUT: analog input for HPF peak-detect
	ANSELAbits.ANSA4		= 1;			//Analog input Enabled
	WPUAbits.WPUA4			= 0;			//Pull-Up Disabled
	
	TRISAbits.TRISA5		= 0;			//RA5 OUTPUT: Select line for LED segments
	WPUAbits.WPUA5			= 0;			//Pull-Up Disabled
	LATAbits.LATA5			= 0;			//Set LOW initially

	INLVLA					= 0x3F;			//Use Schmitt Trigger levels for PortA reads
	
	/*THESE SHOULD BE CHANGED BASED ON YOUR SCHEMATIC DESIGN*/
	//--------------------------------------CONFIGURE PORT B-----------------------------------------------------------
	TRISBbits.TRISB4		= 1;			//RB4 INPUT: analog input accel X
	ANSELBbits.ANSB4		= 1;			//Analog input Enabled
	WPUBbits.WPUB4			= 0;			//Pull-Up Disabled
	
	TRISBbits.TRISB5		= 1;			//RB5 INPUT: serial port RX line
	ANSELBbits.ANSB5		= 0;			//Analog input Disabled
	WPUBbits.WPUB5			= 1;			//Pull-Up Enabled
	APFCON0bits.RXDTSEL		= 0;			//Map the RX pin to RB5
	
	TRISBbits.TRISB6		= 0;			//RB6 OUTPUT: Select line for LED segments
	ANSELBbits.ANSB6		= 0;			//Analog input Disabled
	WPUBbits.WPUB6			= 0;			//Pull-Up Disabled
	LATBbits.LATB6			= 0;			//Set LOW initially

	TRISBbits.TRISB7		= 0;			//RB7 OUTPUT: serial port TX line
	ANSELBbits.ANSB7		= 0;			//Analog input Disabled
	WPUBbits.WPUB7			= 0;			//Pull-Up Disabled
	LATBbits.LATB7			= 0;			//Set LOW initially
	APFCON0bits.TXCKSEL		= 0;			//Map the TX pin to RB7
	
	INLVLB					= 0xF0;			//Use Schmitt Trigger levels for PortB reads
	
	/*THESE SHOULD BE CHANGED BASED ON YOUR SCHEMATIC DESIGN*/
	//--------------------------------------CONFIGURE PORT C-----------------------------------------------------------
	TRISC					= 0x00;			//Set all PortC pins to OUTPUT: LED matrix driving lines!
	ANSELC					= 0x00;			//Disable all Analog inputs
	WPUC					= 0x00;			//Disable all weak-pull-ups
	INLVLC					= 0xFF;			//Use Schmitt Trigger levels for PortC reads
	LATC					= 0x00;			//Clear out Latch C

}
/**************************************************** End of File ****************************************************/
