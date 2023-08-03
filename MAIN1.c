/*
 * File:   MAIN1.c
 * Author: ANGELA
 *
 * Created on 31 de julio de 2023, 22:11
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
#include "LCD.h"

//Definición de variables
#define _XTAL_FREQ 8000000
int POT1;
char cPOT1[4];
int POT2;
char cPOT2[4];
int CONT;
char cCONT[4];

//Prototipos
void setup(void);
void main(void);

//Setup General
void setup(void){
    //Oscilador
    OSCCON = 0B01110001;               //Oscilador a 8Mhz
    
    //SPI
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //Entradas y salidas
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    TRISD = 0;

    //Valores iniciales de variables y puertos
    PORTA = 0;
    PORTB = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTD = 0;
    PORTE = 0;    
}

//Loop
void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    while(1){
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("POT1");
        Lcd_Set_Cursor(1, 5);
        Lcd_Write_String("POT2");
        Lcd_Set_Cursor(1, 10);
        Lcd_Write_String("CONT");

        PORTCbits.RC2 = 0;    //Slave Select
        __delay_ms(1);
        spiWrite(0);
        POT1 = spiRead();
        __delay_ms(1);
        PORTCbits.RC2 = 1;  //Slave Deselect 
        __delay_ms(250);
        
        Lcd_Set_Cursor(2, 1);
        sprintf(cPOT1, "%d", POT1);   //Convertir ADC en tipo char
        if(POT1 < 10){
            cPOT1[2] = cPOT1[0];
            cPOT1[1] = '0';
            cPOT1[0] = '0';
        }
        else if(POT1 < 100){
            cPOT1[2] = cPOT1[1];
            cPOT1[1] = cPOT1[0];
            cPOT1[0] = '0';
        }
        Lcd_Write_String(cPOT1);

        PORTCbits.RC1 = 0;    //Slave Select
        __delay_ms(1);
        spiWrite(1);
        POT2 = spiRead();
        __delay_ms(1);
        PORTCbits.RC1 = 1;   //Slave Deselect 
        __delay_ms(250);
               
        Lcd_Set_Cursor(2, 5);
        sprintf(cPOT2, "%d", POT2);    //Convertir ADC en tipo char para el POT2
        if(POT2 < 10){
            cPOT2[2] = cPOT2[0];
            cPOT2[1] = '0';
            cPOT2[0] = '0';
        }
        else if(POT2 < 100){
            cPOT2[2] = cPOT2[1];
            cPOT2[1] = cPOT2[0];
            cPOT2[0] = '0';
        }
        Lcd_Write_String(cPOT2);
    
        PORTCbits.RC1 = 0;  //Slave Select
        __delay_ms(1);
        spiWrite(0);
        CONT = spiRead();
        __delay_ms(1);
        PORTCbits.RC1 = 1;  //Slave Deselect 
        __delay_ms(250);
               
        Lcd_Set_Cursor(2, 10);
        sprintf(cCONT, "%d", CONT);  //Convertir ADC en tipo char para el conta
        if(CONT < 10){
            cCONT[2] = cCONT[0];
            cCONT[1] = '0';
            cCONT[0] = '0';
        }
        else if(CONT < 100){
            cCONT[2] = cCONT[1];
            cCONT[1] = cCONT[0];
            cCONT[0] = '0';
        }
        Lcd_Write_String(cCONT);
    }
    return;
}