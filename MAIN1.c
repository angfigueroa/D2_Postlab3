/*
 * File:   MAIN1.1
 * Author: ANGELA
 *
 * Created on 2 de agosto de 2023, 14:51
 */


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

//Librer�as
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "SPI.h"
#include "PORTB.h"
#include "ADC.h"

//Definici�n de variables
#define _XTAL_FREQ 8000000
int rebote1;
int rebote2;
int contador;
int ADC;
int in;

//Prototipos
void setup(void);
void main(void);

//Setup General
void setup(void){
    //Oscilador
    OSCCON = 0B01110001;   //Oscilador a 8Mhz
    
    //Interrupciones
    INTCON = 0B11001000;   //Int globales, PIE1, PORTB activadas
    PIE1 = 0B01010000;  //Int ADC, SPI activadas
    PIR1 = 0B00000000;
    
    //ADC
    setup_ADC(0);
    
    //SPI
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    TRISAbits.TRISA5 = 1;
    
    //PORTB
    OPTION_REG = 0;
    TRISB = 0;
    setup_PORTB(6);
    setup_PORTB(7);

    //Entradas y salidas
    TRISCbits.TRISC2 = 0;
    TRISD = 0;
    TRISE = 0;
    
    //Valores iniciales de variables y puertos
    PORTA = 0;
    PORTB = 0;
    PORTCbits.RC2 = 1;
    PORTD = 0;
    PORTE = 0;
    
    rebote1 = 0;
    rebote2 = 0;
    contador = 0;
    in = 0;
    
    return;
}

//Interrupcion
void __interrupt() isr(void) {
    if (PIR1bits.ADIF){// Verifica si la interrupci�n es del ADC
        ADC = read_ADC();// Lee el valor del conversor anal�gico a digital
    }
    
    if (INTCONbits.RBIF){// Verifica si la interrupci�n es por cambio en PORTB
        if (!PORTBbits.RB6){
            rebote1 = 1; // Activa la variable de rebote 1 si se detecta el bot�n
        }
        else if (!PORTBbits.RB7){
            rebote2 = 1;// Activa la variable de rebote 2 si se detecta el bot�n
        }
    }
    
    if (SSPIF == 1){// Verifica si la interrupci�n es del m�dulo SPI
        in = spiRead();// Lee los datos recibidos por SPI
        if (in == 0){
            spiWrite(ADC);// Env�a el valor del ADC por SPI si se recibe un 0
        }
        else if (in == 1){
            spiWrite(contador);// Env�a el valor del contador por SPI si se recibe un 1
        }
    }
    INTCONbits.RBIF = 0;   //Limpia la bandera de interrupci�n
    PIR1bits.ADIF = 0;// Limpia la bandera de interrupci�n del ADC
    SSPIF = 0;// Limpia la bandera de interrupci�n del m�dulo SPI
}

//Loop
void main(void) {
    setup();// Llama a la funci�n de configuraci�n
    while(1){// Bucle infinito
        if (ADCON0bits.GO == 0){// Verifica si la conversi�n del ADC est� en curso
            __delay_ms(5);
            ADCON0bits.GO = 1;// Inicia una nueva conversi�n ADC
        }

        if (PORTBbits.RB6 && rebote1 == 1){// Verifica el estado del bot�n 1 y la variable de rebote
            contador += 1;// Incrementa el contador
            rebote1 = 0;// Desactiva la variable de rebote
        }
        else if (PORTBbits.RB7 && rebote2 == 1){// Verifica el estado del bot�n 2 y la variable de rebote
            contador -= 1;// Decrementa el contador
            rebote2 = 0;// Desactiva la variable de rebote
        }
    }
}
