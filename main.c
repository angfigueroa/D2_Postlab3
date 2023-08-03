/* 
 * File:   main.c
 * Author: ANGELA
 *
 * Created on 27 de julio de 2023, 8:18
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici?n e importaci?n de librer?as
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "SPI.h"
//*****************************************************************************
// Definici?n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t temporal = 0;
//*****************************************************************************
// Definici?n de funciones para que se puedan colocar despu?s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// C?digo de Interrupci?n 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        //PORTD = spiRead();
        spiWrite(PORTB);
        SSPIF = 0;
        
    }
}
//*****************************************************************************
// C?digo Principal
//*****************************************************************************
void main(void) {
    
    setup();
    // Configuración de los puertos
    TRISB = 0b11111111;  // RB1 como entrada
    TRISC = 0;              // Puerto A como salida
    PORTC = 0;              // Inicializar Puerto A en 0
    
    // Configuración de las resistencias pull-up
    OPTION_REGbits.nRBPU = 0;   // Habilitar resistencias pull-up en Puerto B
    WPUBbits.WPUB6 = 1;         // Habilitar resistencia pull-up en RB0
    WPUBbits.WPUB7 = 1;  
    // Habilitar resistencia pull-up en RB1
    
    // Variables
    uint8_t contador = 0;
   
    uint16_t potValue = 0;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
        // Incrementar contador si se presiona RB0
        if (PORTBbits.RB6 == 0) {
                while (!PORTBbits.RB6); // Esperar a que se suelte el botón
                contador++;
        }
        
        // Decrementar contador si se presiona RB1
        if (PORTBbits.RB7 == 0) {
           
                while (!PORTBbits.RB7); // Esperar a que se suelte el botón
                contador = contador - 1;
            
        }
        
        // Mostrar contador en Puerto A
        PORTC = contador;
      
       // Leer el valor analógico del potenciómetro en el puerto E (AN0) usando la función ADC
        ADCON0bits.CHS = 0; // Seleccionar canal AN0
        ADCON0bits.GO = 1; // Iniciar la conversión
        while(ADCON0bits.GO); // Esperar a que la conversión termine
        potValue = ADRESH; // Leer el resultado de la conversión analógica de 8 bits
        
        // Enviar el valor del potenciómetro al puerto B
        PORTB = (uint8_t)potValue;
        
        __delay_ms(250);
        
        
    }
    
    return;
}
//*****************************************************************************
// Funci?n de Inicializaci?n
//*****************************************************************************
void setup(void){
    
    OSCCONbits.IRCF2 = 1;            //Oscilador a 4MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    
    OSCCONbits.SCS = 1;
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1;
    
    TRISA0 = 1;
   
    TRISB6 = 1;
    TRISB7 = 1;
    TRISD = 0;
    PORTD = 0x00;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci?n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci?n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    
    INTCONbits.RBIE = 1;
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB7 = 1;
    WPUBbits.WPUB6 = 1;
    IOCBbits.IOCB7 = 1;
    IOCBbits.IOCB6 = 1;
    INTCONbits.RBIF = 0;
    
    
    // Configuración del módulo ADC
    ADCON0bits.ADCS = 0b01; // Fosc/8 (2 microsegundos para una conversión)
    ADCON0bits.CHS = 0; // Seleccionar canal AN0
    ADCON0bits.ADON = 1; // Habilitar el módulo ADC
    
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}



