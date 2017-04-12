/**********************************************************************************************************************
* File Name:		main.c
* Author:			EE391 Students & Professors
* Description:  	Software development for EE391 Project Boards
* Version:			v1.0 - 03/10/17 - A.R.Taylor - Initial release version
**********************************************************************************************************************/
#include "main.h"

/*********************************************************************************************************************/
//Global variable declarations to be used in program
float						SwVer		= 1.00;			//Variable to store software version
unsigned char				RunState	= 0;			//Used to control state machine of MCU
unsigned char				sendSCI		= 0;			//Counter to schedule the sending of RS232 data
unsigned char				recvSCI		= 0;			//Counter for receiving RS232 Rx data

struct ADCval				vBat;						//ADC value; battery voltage
struct ADCval				Temp;						//ADC value; 
struct ADCval				AccX;						//ADC value; 
struct ADCval				AccY;						//ADC value; 

union CharFloat				SciTxData;					//Buffer used to bit-shift float data for SCI
union CharFloat				SciRxData;					//Buffer used to reconstruct float data from SCI

/*********************************************************************************************************************
*	 main() - Entry point on reset (init CPU, interrupts, etc.)
*********************************************************************************************************************/
void main(void)
{	configSysClock();						//Setup HF internal oscillator as clk source
	configIOports();						//Set port directions & states
	configTimer2();							//Configure Timer2 for main() timed-loop execution
	configADC();							//Enable the ADC module 
	configSCI();							//Enable the SCI serial port module

	INTCONbits.PEIE			= 1;			//Enable Peripheral Interrupts
	INTCONbits.GIE			= 1;			//Enable Global Interrupts

	while(1);								//Do nothing forever!
}/**************************************************** End of MAIN ***************************************************/

void interrupt IntServRoutine(void)
{	/*****************************************************************************************************************/
	//  Timer2 Interrupt - Main execution loop; tuned to 100Hz loop for routine tasks...
	/*****************************************************************************************************************/
	if (PIR1bits.TMR2IF)						//Check for Timer interrupt flag
	{	PIR1bits.TMR2IF = 0;					//Clear interrupt flag
		LATAbits.LATA1 = 1;						//Measure this pin to see the length of code execution time!
		
		LATC = 0b10000111;
		
		LATAbits.LATA5 = 1;
		LATBbits.LATB6 = 0;
		
		//The following switch case grabs the recent ADC data and selects the next channel to convert
		switch(ADCON0bits.CHS)
		{	case Ch_vBat: 
				vBat.raw = ADRES;					//Store raw ADC result
				vBat.volt = vBat.raw * ADC_GAIN;	//Compute the voltage on the pin
				ADCON0bits.CHS = Ch_Temp;			//Select the next ADC channel
				break;
			case Ch_Temp:
				Temp.raw = ADRES;					//Store raw ADC result
				Temp.volt = Temp.raw * ADC_GAIN;	//Compute the voltage on the pin
				ADCON0bits.CHS = Ch_AccX;			//Select the next ADC channel
				break;
			case Ch_AccX:
				AccX.raw = ADRES;					//Store raw ADC result
				AccX.volt = AccX.raw * ADC_GAIN;	//Compute the voltage on the pin
				ADCON0bits.CHS = Ch_AccY;			//Select the next ADC channel
				break;
			case Ch_AccY:
				AccY.raw = ADRES;					//Store raw ADC result
				AccY.volt = AccY.raw * ADC_GAIN;	//Compute the voltage on the pin
				ADCON0bits.CHS = Ch_vBat;			//Select the next ADC channel
				break;
			default:
				ADCON0bits.CHS = Ch_vBat;			//Not sure how we got here... set to vBat channel!
				break;
		}//close switch
		ADCON0bits.GO = 1;							//Start ADC converter!
		
		//This switch case selects a data variable to send through the serial port
		switch(sendSCI)
		{	case 0: SciTxData.word = RunState; break;
			case 1: SciTxData.word = (int)(SwVer * 100); break;
			case 2: SciTxData.word = (int)(vBat.volt * 100); break;
			case 3: SciTxData.word = (int)(Temp.volt * 100); break;
			case 4: SciTxData.word = (int)(AccX.volt * 100); break;
			case 5: SciTxData.word = (int)(AccY.volt * 100); break;
			case 6: SciTxData.word = 0; break;
			case 7: SciTxData.word = 0; break;
			case 8: SciTxData.word = 0; break;
			default:
				sendSCI = 255; 
				SciTxData.byte[1] 	= '!';			//Write data to Tx buffer
				SciTxData.byte[0] 	= '!';			//Write data to Tx buffer
		} sendSCI++;	
		
		TXREG = SciTxData.byte[1];					//Send data through SCI port
		TXREG = SciTxData.byte[0];					//Send data through SCI port

		LATAbits.LATA1 = 0;							//Measure this pin to see the length of code execution time!
	}//Close Timer2 Interrupt


	/*****************************************************************************************************************/
	//  SCI Rx Interrupt - Retrieves bytes from SCI Rx FIFO for debugging purposes...
	/*****************************************************************************************************************/
	if (PIR1bits.RCIF)									//If SCI recieved data...
	{	while (PIR1bits.RCIF)
		{	if (recvSCI == 0) SciRxData.byte[0] = RCREG;
			if (recvSCI == 1) SciRxData.byte[1] = RCREG;
			recvSCI++;
		}
		
		if (recvSCI >= 2) 								//After receiving two bytes of data... process command!
		{	signed char Cmd, Val;						//Temporary variables for switch-case statement
			Cmd = SciRxData.byte[0];					//Grab out command byte
			Val = SciRxData.byte[1];					//Get the integer value of data
		
			/*THESE SHOULD BE CHANGED BASED ON YOUR SYSTEM REQUIREMENTS*/
			switch (Cmd)								//Check which command was issued
			{	case 'S':	//RunState
					RunState = Val;
					break;
		
				default : break;
			}
			recvSCI = 0;								//Clear out our RX byte counter for next message
		}

	}//Close SCI Rx Interrupt


}/**************************************************** End of ISR ****************************************************/


/**************************************************** End of File ****************************************************/

