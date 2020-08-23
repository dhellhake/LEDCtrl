/*
 * LEDCtrl.c
 *
 * Created: 23.08.2020 18:44:27
 * Author : dominik hellhake
 */
#define F_CPU 16000000UL
#define ADC_LSB			5.0f / 1024.0f
#define LED_COUNT		(30 * 6)
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "WS2812/WS2812.h"
#include "ADC/ADClib.h"

typedef enum
{
	INIT,
	DAY,
	NIGHT
} daytime_t ;


volatile uint8_t tgl = 0;
volatile uint16_t divider = 0xFFFF;

uint8_t led_on[1 * 3] = { 0x66, 0x66, 0x66 };
uint8_t led_off[1 * 3] = { 0x00, 0x00, 0x00 };

volatile daytime_t daytime = INIT;	



ISR(TIMER0_COMPA_vect)
{
	if (divider < 60)
		divider++;
	else
	{
		float adc_val = ((float)GetVin()) * ADC_LSB;
		
		switch (daytime)
		{
			case INIT:
			if (adc_val > 2.5f)
			{
				ws2812_sendarray_mask(led_on, LED_COUNT * 3);
				daytime = NIGHT;
			}
			else
			{
				ws2812_sendarray_mask(led_off, LED_COUNT * 3);
				daytime = DAY;
			}
			break;
			case DAY:
				if (adc_val > 2.5f)
					tgl++;
				else
					tgl = 0;
				
				if (tgl > 1)
				{
					ws2812_sendarray_mask(led_on, LED_COUNT * 3);
					tgl = 0;
					daytime = NIGHT;
				}
			break;
			case NIGHT:
				if (adc_val < 2.2f)
					tgl++;
				else
					tgl = 0;
			
				if (tgl > 1)
				{
					ws2812_sendarray_mask(led_off, LED_COUNT * 3);
					tgl = 0;
					daytime = DAY;
				}
			break;
		}
		divider = 0;
	}
}


int main(void)
{	
	DDRB |= (1 << PB4);
	
	InitADC();
	
	TCCR0A = (1 << WGM01);						//CTC mode
	TCCR0B = (1 << CS00) | (1 << CS02);         //div1024
	OCR0A = 255;								// 50us compare value
	TIMSK |= (1<<OCIE0A);              //if you want interrupt
	sei();
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

