    #include <stdio.h>
    #include <90USB1286.h>
    #include <delay.h>
    #include "display.h"                     
    #include <DS1302.h> 
    
    #asm
    .equ __ds1302_port=0x0B
    .equ __ds1302_io=2
    .equ __ds1302_sclk=1
    .equ __ds1302_rst=0
    #endasm
         
    unsigned char h,m,s,hr=12,min=0,i=0,j=0,a=0,b=0,x=0,y=0,o=0,d=0;
    char cadena[10];  
  
    
void main()
{
    DDRF.6=1;
    ConfiguraLCD(); 
    DDRB.5=1;
    DDRC=0xF0; 
    PORTC=0x0F;
    rtc_init(0,0,0);   //Se inicializa el DS1302
    
    while(1){
        rtc_get_time(&h,&m,&s); //Se toma el tiempo en el que va el DS1302         
        if(PINC.0==0){   
            i=1;//se usan banderas para determinar cuando esta presionado el boton     
        }                  
        if(PINC.0==1 && i==1){
            j=1;
        }       
        if(j==1 && i==1){
            h++; //Se modifican las horas cuando se cumple la condicion 
            j=0;
            i=0;       
            if(h==24){
                h=0; //Se resetean las horas 
            }
            rtc_set_time(h,m,s);
        }
            
           
        if(PINC.1==0){
           a=1;  //Se realiza lo mismo que el primer boton
        }      
        if(PINC.1==1 && a==1){
            b=1;
        }
        if(a==1 && b==1){
            m++;  //Se suman minutos cuando se presiona el boton
            a=0;
            b=0;       
            if(m==60){
                m=0; //Se resetean los minutos
            }   
            rtc_set_time(h,m,s);
        }
        if(PINC.2==0){ 
            x=1; //Se realiza lo mismo que el primer boton 
        }                  
        if(PINC.2==1 && x==1){
            y=1;
        }       
        if(y==1 && x==1){
            hr++;  //Se suman las horas de la alarma cuando se presiona el boton
            y=0;
            x=0;       
            if(hr==24){
                hr=0; //Se resetea las horas de la alarma
            }
        }
        if(PINC.3==0){ 
            o=1;       //Se realiza lo mismo que el primer boton
        }                  
        if(PINC.3==1 && o==1){
            d=1;
        }       
        if(d==1 && o==1){
            min++;          //Se modifican los minutos de la alarma
            d=0;
            o=0;       
            if(min==60){
                min=0;    //Se resetea los minutos de la alarma
            }
        }
        
        if(h==hr && m==min){
         PORTB.5=1;           //se enciende la alarma y el Backlight del display
         BacklightON();
        }
        if(PINC.0==0 || PINC.1==0 || PINC.2==0 || PINC.3==0 || m!=min){
                PORTB.5=0;
                BacklightOFF();        //se apaga
        }
        MoverCursor(2,1);
        StringLCD("Reloj:");    
        sprintf(cadena,"%02d:%02d:%02d  ",h,m,s);
        MoverCursor(10,1);
        StringLCDVar(cadena);
        sprintf(cadena,"%02d:%02d  ",hr,min);
        MoverCursor(2,2);
        StringLCD("Alarma:"); 
        MoverCursor(12,2);
        StringLCDVar(cadena);
         
    }             
}
