/*
 * ADClib.h
 *
 * Created: 23.08.2020 18:47:53
 *  Author: dominik hellhake
 */
#ifndef ADCLIB_H_
#define ADCLIB_H_

#include <avr/io.h>

void InitADC();
uint16_t GetVin();

#endif /* ADCLIB_H_ */