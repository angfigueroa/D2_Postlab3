#include "PORTB.h"
// Prototipos de funciones
void setup_PORTB(char pin);
int antiRebote(char pin);
int R;
// Funci�n para configurar el pin del puerto B
void setup_PORTB(char pin){
    switch (pin){
        case 0:// Configura el pin RB0 como entrada con pull-up y habilita interrupci�n por cambio
            TRISBbits.TRISB0 = 1;
            WPUBbits.WPUB0 = 1;
            IOCBbits.IOCB0 = 1;
            break;
        case 1:
            TRISBbits.TRISB1 = 1;
            WPUBbits.WPUB1 = 1;
            IOCBbits.IOCB1 = 1;
            break;
        case 2:
            TRISBbits.TRISB2 = 1;
            WPUBbits.WPUB2 = 1;
            IOCBbits.IOCB2 = 1;
            break;
        case 3:
            TRISBbits.TRISB3 = 1;
            WPUBbits.WPUB3 = 1;
            IOCBbits.IOCB3 = 1;
            break;
        case 4:
            TRISBbits.TRISB4 = 1;
            WPUBbits.WPUB4 = 1;
            IOCBbits.IOCB4 = 1;
            break;
        case 5:
            TRISBbits.TRISB5 = 1;
            WPUBbits.WPUB5 = 1;
            IOCBbits.IOCB5 = 1;
            break;
        case 6:
            TRISBbits.TRISB6 = 1;
            WPUBbits.WPUB6 = 1;
            IOCBbits.IOCB6 = 1;
            break;
        case 7:// ... Repite para los casos de los dem�s pines RB1, RB2, ...
            TRISBbits.TRISB7 = 1;
            WPUBbits.WPUB7 = 1;
            IOCBbits.IOCB7 = 1;
            break;
    }
    
}
// Funci�n para detectar el antirrebote de un bot�n conectado al pin del puerto B
int antiRebote(char pin){
    switch (pin){
        case 0:// Verifica si el bot�n en RB0 est� presionado
            if (PORTBbits.RB0){
                R = 1;
            }
            break;
        case 1:
            if (PORTBbits.RB1){
                R = 1;
            }
            break;
        case 2:
            if (!PORTBbits.RB2){
                R = 1;
            }
            break;
        case 3:
            if (!PORTBbits.RB3){
                R = 1;
            }
            break;
        case 4:
            if (!PORTBbits.RB4){
                R = 1;
            }
            break;
        case 5:
            if (!PORTBbits.RB5){
                R = 1;
            }
            break;
        case 6:
            if (!PORTBbits.RB6){
                R = 1;
            }
            break;
        case 7:// Verifica si el bot�n en RB7 est� presionado de manera similar a los casos anteriores
            if (!PORTBbits.RB7){
                R = 1;
            }
            break;
        default:
            R = 0;
            break;
    }
    return R;// Devuelve el valor de la variable R (1 si el bot�n est� presionado, 0 si no)
}
