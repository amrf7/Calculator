/*
 * File:   examen.c
 * Author: anita
 *
 * Created on 22 de octubre de 2020, 12:47 AM
 */
#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"
#include "Keypad.c"
#pragma config WDTE = OFF, PWRTE = OFF, BOREN=OFF, LVP=OFF,CPD=OFF, CP=OFF, FOSC=INTOSC
//FUNCTIONS
void clear();
void main(void) {
    OSCCONbits.IRCF = 0b1110; //Internal Oscillator Frequency Select bits
    OSCCONbits.SCS = 0b00; //System Clock Select bits
    ANSELD = 0;
    ANSELB=0;
    TRISC=0;
    PORTC=0;
    //initialize LCD and Keypad
    InitKeypad();
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR);
    //Lcd_Cmd(LCD_CURSOR_OFF);
    //variables
    int udc[3]={100,10,1},udc1[3]={100,10,1},number[4]={0},
            clc=0;
    float operation=0,num1=0,num2=0;
    char num[4]={0},op=' ';
    unsigned char buffer[20];
    //code
    do{
        for(int i=0;i<4;i++){
            //read number
            num[i]=switch_press_scan();
            number[i]=num[i]-48;
            if(number[i]==(int)('*'-48)){
                clc=1;
                break;
            }else if(number[i]<10&&number[i]>=0){
                sprintf(buffer,"%c",num[i]);
                Lcd_Out(1,i,buffer);
            }else{
                op=num[i];
                for(int j = 0; j < 2; j++){
                    if(j+i<3)
                        number[i+j]=0;
                    if(3-i+j<3)
                        udc[j]=udc[3-i+j];
                    else
                        udc[j]=1;
                }
                break;
            }
        }
        if(clc==1)
            break;
        for(int i = 0; i < 3; i++){
            num1+=udc[i]*number[i];
        }
        //op=switch_press_scan();
        switch(op){
            case 'A':
                sprintf(buffer,"%c",'/');
                break;
            case 'B':
                sprintf(buffer,"%c",'x');
                break;
            case 'C':
                sprintf(buffer,"%c",'-');
                break;
            case 'D':
                sprintf(buffer,"%c",'+');
                break;
        }
        Lcd_Out(1,3,buffer);
        for(int i=0;i<4;i++){
            //read number
            num[i]=switch_press_scan();
            number[i]=num[i]-48;
            if(number[i]==(int)('*'-48)){
                clc=1;
                break;
            }else if(number[i]<10&&number[i]>=0){
                sprintf(buffer,"%c",num[i]);
                Lcd_Out(1,4+i,buffer);
            }else{
                for(int j = 0; j < 2; j++){
                    if(j+i<3)
                        number[i+j]=0;
                    if(3-i+j<3)
                        udc1[j]=udc1[3-i+j];
                    else
                        udc1[j]=1;
                }
                break;
            }
        }
        if(clc==1)
            break;
        for(int i = 0; i < 3; i++){
            num2+=udc1[i]*number[i];
        }
        
        switch(op){
            case 'A':
                operation=num1/num2;
                sprintf(buffer,"%f",operation);
                break;
            case 'B':
                operation=num1*num2;
                sprintf(buffer, "%f", operation);
                break;
            case 'C':
                operation=num1-num2;
                sprintf(buffer, "%f", operation);
                break;
            case 'D':
                operation=num1+num2;
                sprintf(buffer,"%f",operation);
                break;
        }
        Lcd_Out(2,0,buffer);
        __delay_ms(2000);
        clear();
        break;
    }while(clc!=1);
    if(clc==1){
        clear();
        Lcd_Out(1,1,"CLEAR");
        __delay_ms(1000);
    }
    return;
}

void clear(){
    Lcd_Cmd(LCD_CLEAR);
    
    return;
}
