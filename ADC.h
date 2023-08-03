#ifndef ADC_H
#define	ADC_H


#include <xc.h> // include processor files - each processor file is guarded.  

void setup_ADC(int channel);
int read_ADC();
void change_channel_ADC(int channel);
int get_channel_ADC();

#endif	/* ADC_int_H */