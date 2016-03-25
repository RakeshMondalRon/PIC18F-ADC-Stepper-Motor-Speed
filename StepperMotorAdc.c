/* 
 * File:   StepperMotorAdc.c
 * Author: Rakesh Mondal Ron
 *
 * Free Source code under GPL. Free to distribute
 * Created on 3 April, 2015, 7:27 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <delays.h>

#define _XTAL_FREQ   20000000UL // This one is just for __delay_ms
    /* Setting Compiler Directives */
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (Divide by 5 (20 MHz oscillator input))
#pragma config CPUDIV = OSC2_PLL3 // System Clock Postscaler Selection bits ([Primary Oscillator Src: /2][96 MHz PLL Src: /3])
#pragma config USBDIV = 2
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

void ADConf() { /* ADC Configuration FunctProt*/ 
   
/*  Channel Selection Bits*/    
    ADCON0bits.CHS3 = 0; // Analog channel to AN0 or RA0 or pin #2
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0; 
/* Volatge Reference 5V - 0 V */
    ADCON1bits.VCFG1 = 0; // set A/D converter negative reference to gnd
    ADCON1bits.VCFG0 = 0; // set A/D converter positive reference to pwr (5V - 10 bit operation - Good Resolution)
/* Analog Pin Selection */   
    ADCON1bits.PCFG3 = 1; // set RA0 / AN0 to analog input, all other pins digital input
    ADCON1bits.PCFG2 = 1; // Attach the pot only to RA0
    ADCON1bits.PCFG1 = 1; //
    ADCON1bits.PCFG0 = 0; //s
/* Format for ADC Result -- Right Justified  */ 
   ADCON2bits.ADFM = 1; // right justify A/D result, "8 bit" A/D result in reg ADRESH
/* Acquisition time */ 
    ADCON2bits.ACQT2 = 0; // setting the  A/D Acquisition time
    ADCON2bits.ACQT1 = 0; // 2tad (100) as written in data sheet
    ADCON2bits.ACQT0 = 1; //
 /* ADC Clock*/    
    ADCON2bits.ADCS2 = 0; // set A/D conversion clock
    ADCON2bits.ADCS1 = 1; // fosc / 32 ( F/16 would be very close)
    ADCON2bits.ADCS0 = 0; //
    
    ADCON0bits.ADON = 1; // Turn on ADC
}
/*  */ 
void main(void)
{
   
    TRISBbits.TRISB7=  0;  // Setting 4 pins to output
    TRISBbits.TRISB6 = 0;  // Pins Which are going to ULN2003
    TRISBbits.TRISB5 = 0; 
    TRISBbits.TRISB4 = 0;
    ADConf();
 while(1)
 { 
        unsigned int spd1, ADCres; //Variables for Speed and ADC
        ADCON0bits.GO_DONE = 1; // Start Conversion
        while (ADCON0bits.GO_DONE == 1) {  }
 /* Store ADC op to ADCres */ 
     ADCres = ADRES; 
 /*  Speed Tuning based on ADC Result 0-1023*/
    if (ADCres <= 100) { spd1 =10;}
        else if (ADCres <= 205) { spd1=15;  }
        else if (ADCres<=310)   { spd1=25;  }
        else if (ADCres<=410)   { spd1=40;  }
        else if (ADCres<=510)   { spd1=50;  }
        else if (ADCres<=610)   { spd1=60;  }
        else if (ADCres<=700)   { spd1=70;  }
        else if (ADCres<=800)   { spd1=80;  }
        else if (ADCres<=900)   { spd1=90;  }
        else if (ADCres<=1000)  { spd1=100; }
            else {spd1=130;}
                      
        LATB=0x80; /*  */
         for ( int x=0; x<=spd1; x++ )
          {   __delay_ms(1);   }
        LATB=0x40; /*  */
         for ( int x=0; x<=spd1; x++ )
          {   __delay_ms(1);   }
        LATB=0x20; /*  */
         for ( int x=0; x<=spd1; x++ )
          {   __delay_ms(1);   }
        LATB=0x10; /*  */
         for ( int x=0; x<=spd1; x++ )
          {   __delay_ms(1);   }

    }
}
