#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//Prototipo de funcion
int readADC(int canal);
void setupADC(void);

#endif	/* XC_HEADER_TEMPLATE_H */