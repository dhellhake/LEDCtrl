/*
 * ADClib.c
 *
 * Created: 23.08.2020 18:47:30
 *  Author: dominik hellhake
 */
#include "ADClib.h"

void InitADC()
{
	ADMUX =
		(0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
		(0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
		(0 << ADLAR) |     // left shift result
		(0 << REFS2) |     // Sets ref. voltage to VCC, bit 0
		(0 << MUX3)  |     // use ADC2 for input (PB3), MUX bit 3
		(0 << MUX2)  |     // use ADC2 for input (PB3), MUX bit 2
		(1 << MUX1)  |     // use ADC2 for input (PB3), MUX bit 1
		(1 << MUX0);       // use ADC2 for input (PB3), MUX bit 0

	ADCSRA =
		(1 << ADEN)  |     // Enable ADC
		(1 << ADPS2) |     // set prescaler to 128, bit 2
		(1 << ADPS1) |     // set prescaler to 128, bit 1
		(1 << ADPS0);      // set prescaler to 128, bit 0
}

uint16_t GetVin()
{
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
	
	return ADC;
}
