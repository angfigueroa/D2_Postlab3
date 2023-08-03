/*
 * File:   MAIN1.c
 * Author: ANGELA
 *
 * Created on 31 de julio de 2023, 22:10
 */

// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//Librerías
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "SPI.h"
#include "ADC.h"

//Definición de variables
#define _XTAL_FREQ 8000000
int ADC;

//Prototipos
void setup(void);
void main(void);

//Setup General
void setup(void){
    //Oscilador
    OSCCON = 0B01110001;   //Oscilador a 8Mhz
    
    //Interrupciones
    INTCON = 0B11000000;   //Int globales, PIE1 activadas
    PIE1 = 0B01010000;  //Int ADC y SPI activadas
    PIR1 = 0B00000000;
    OPTION_REG = 0;
    
    //ADC
    setup_ADC(0);
    
    //SPI
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    TRISAbits.TRISA5 = 1;
    
    //Entradas y salidas
    TRISB = 0;
    TRISCbits.TRISC2 = 0;
    TRISD = 0;
    TRISE = 0;
    
    //Valores iniciales de variables y puertos
    PORTA = 0;
    PORTB = 0;
    PORTCbits.RC2 = 1;
    PORTD = 0;
    PORTE = 0;    
}

//Interrupcion
void __interrupt() isr(void) {
    if (PIR1bits.ADIF){
        ADC = read_ADC();
    }
    
    if (SSPIF == 1){
        spiWrite(ADC);
    }
    
    PIR1bits.ADIF = 0;  //Limpia la bandera de interrupción
    SSPIF = 0;
}

//Loop
void main(void) {
    setup();
    while(1){
        if (ADCON0bits.GO == 0){
            __delay_ms(5);
            ADCON0bits.GO = 1;
        }

    return;
    }
}