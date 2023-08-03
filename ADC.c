#include "adc.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000 

float ADC_Voltaje;

//***********************************
//FUNCION DE LECTURA ADC
//***********************************
int readADC(int canal){
    //Canal AN0
    if (canal == 0){
        //Lectura del canal AN0
        ADCON0bits.CHS = 0b0000; //Se usa AN0
        __delay_us(100);
        ADCON0bits.GO = 1; //Iniciamos la conversion del ADC
        
        while(ADCON0bits.GO == 1){
        ;    
        }
        PIR1bits.ADIF = 0;
        ADC_Voltaje = ADRESH;
        __delay_us(100);
    }
    
    //Canal AN1
    else if (canal == 1){
        //Lectura del canal AN1
        ADCON0bits.CHS = 0b0001; //Se usa AN1
        __delay_us(100);
        ADCON0bits.GO = 1; //Iniciamos la conversion del ADC
        
        while(ADCON0bits.GO == 1){
        ;    
        }
        PIR1bits.ADIF = 0;
        ADC_Voltaje = ADRESH;
        __delay_us(100);
    }
    return ADC_Voltaje;    
}

void setupADC(void){
    PORTAbits.RA0 = 0;
    TRISAbits.TRISA0 = 1; //RA0 es entrada
    ANSELbits.ANS0 = 1;//RA0 es analogico
    
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;//Habilitar interrupcion del ADC 
    PIR1bits.ADIF = 0;
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;//Fosc/8
    
    ADCON1bits.VCFG1 = 0;//Referencia VSS
    ADCON1bits.VCFG0 = 0;//Referencia VDD
    
    ADCON1bits.ADFM = 0;//se justifica en la izquierda
    
    ADCON0bits.ADON = 1;//Hablitar el convertidor ADC
    __delay_us(100);
}
