/*
 * WS2812.h
 *
 * Created: 23.08.2020 18:46:25
 *  Author: dominik hellhake
 */
#ifndef WS2812_H_
#define WS2812_H_


inline void ws2812_sendarray_mask(uint8_t *data, uint16_t datlen)
{
	volatile uint8_t curbyte,ctr,masklo;
	uint8_t maskhi = 0b00010000;
	
	masklo	=~maskhi&PORTB;
	maskhi |= PORTB;
	
	curbyte=*data++;
	while (datlen--) {
		
		asm volatile(
		"       ldi   %0,8  \n\t"
		"loop%=:            \n\t"
		"       out   %2,%3 \n\t"    //  '1' [01] '0' [01] - re
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"       sbrs  %1,7  \n\t"    //  '1' [03] '0' [02]
		"       out   %2,%4 \n\t"    //  '1' [--] '0' [03] - fe-low
		"       lsl   %1    \n\t"    //  '1' [04] '0' [04]
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"       out   %2,%4 \n\t"    //  '1' [+1] '0' [+1] - fe-high
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"nop      \n\t"
		"       dec   %0    \n\t"    //  '1' [+2] '0' [+2]
		"       brne  loop%=\n\t"    //  '1' [+3] '0' [+4]
		:	"=&d" (ctr)
		:	"r" (curbyte), "I" (_SFR_IO_ADDR(PORTB)), "r" (maskhi), "r" (masklo)
		);
	}
}

#endif /* WS2812_H_ */