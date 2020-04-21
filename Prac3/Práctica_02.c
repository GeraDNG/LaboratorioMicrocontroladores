#include <90USB1286.h>
#include <delay.h>
#include <stdio.h>
#include <ff.h>
#include "display.h"

char Archivo1[] = "0:screen1.txt";
char Archivo2[] = "0:screen2.txt";
char Archivo3[] = "0:screen3.txt";
char Archivo4[] = "0:screen4.txt";
char Archivo5[] = "0:screen5.txt";

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
disk_timerproc();
/* MMC/SD/SD HC card access low level timing function */
}
    

void main()
{
    unsigned int  br,j=0,k=0;
    unsigned char i;
    char buffer[50];
    
    /* FAT function result */
    FRESULT res;
    
    /* will hold the information for logical drive 0: */
    FATFS drive;
    FIL archivo; // file objects
    
    /*Configurar el PORTB I/O*/
    DDRB=0b11101101;
    
    // Código para hacer una interrupción periódica cada 10ms
    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 1000.000 kHz
    // Mode: CTC top=OCR1A
    // Compare A Match Interrupt: On
    TCCR1B=0x09;
    OCR1AH=0x27;
    OCR1AL=0x10;
    TIMSK1=0x02;
    ConfiguraLCD();
    #asm("sei")
    /* Inicia el puerto SPI para la SD */
    disk_initialize(0);
    delay_ms(200);
    
    /* mount logical drive 0: */
    if ((res=f_mount(0,&drive))==FR_OK){
        MoverCursor(0,1);
        StringLCD("Drive Detectado");
        delay_ms(1500);
        
        /*Lectura de Archivo*/
        res = f_open(&archivo, Archivo1, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if (res==FR_OK){
            MoverCursor(0,1);
            StringLCD("Archivos Encontrados ");
            delay_ms(1500);      
            
            f_read(&archivo, buffer, 50,&br); //leer archivo
            
            MoverCursor(0,1); 
            StringLCD("Leyendo archivos     ");
            delay_ms(1500);
            BorrarLCD();
            for(i=0; i<50;i++){          //Leyendo el screen1 e imprimiendo letra por letra en el LCD
                MoverCursor(k,j);
                //StringLCDVar(buffer[i]);
                k++;
                if(buffer[i]==0x0D || buffer[i]==0x0A ){
                    j++;                  //Cambio de renglon por cada retorno de carro encontrado
                    k=0;
                    i++;
                }
                LetraLCD(buffer[i]);
                
            }                                  
            delay_ms(3000); 
            BorrarLCD();
            f_close(&archivo);       
        }              
        else{
            StringLCD("Archivo NO Encontrado");  
        }
        res= f_open(&archivo, Archivo2, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      
        if (res==FR_OK){
            f_read(&archivo, buffer, 50,&br); //leer archivo 
            j=0;
            k=0;
            BorrarLCD();  
            for(i=0; i<50;i++){                         //Leyendo el screen2 e imprimiendo letra por letra en el LCD
                MoverCursor(k,j);
                //StringLCDVar(buffer[i]); 
                k++;
                if(buffer[i]==0x0D || buffer[i]==0x0A){
                    j++;                               //Cambio de renglon por cada retorno de carro encontrado
                    k=0;
                    i++;
                }
                LetraLCD(buffer[i]);
            }                                  
            delay_ms(3000); 
            BorrarLCD();   
            f_close(&archivo); 
        }
        else{
            StringLCD("Archivo NO Encontrado");
        }
        res= f_open(&archivo, Archivo3, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      
        if (res==FR_OK){
            f_read(&archivo, buffer, 50,&br); //leer archivo
            j=0;
            k=0;
            BorrarLCD();                                          //Leyendo el screen3 e imprimiendo letra por letra en el LCD
            for(i=0; i<50;i++){    
                MoverCursor(k,j);
                //StringLCDVar(buffer[i]);
                k++;
                if(buffer[i]==0x0D || buffer[i]==0x0A){
                    j++;                                //Cambio de renglon por cada retorno de carro encontrado
                    k=0;
                    i++;
                }
                LetraLCD(buffer[i]);
            }                                  
            delay_ms(3000);
            BorrarLCD();
            f_close(&archivo); 
        }
        else{
            StringLCD("Archivo NO Encontrado");
        }
        res= f_open(&archivo, Archivo4, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      
        if (res==FR_OK){
            f_read(&archivo, buffer, 50,&br); //leer archivo
            j=0;
            k=0; 
            BorrarLCD();                                                   //Leyendo el screen4 e imprimiendo letra por letra en el LCD
            for(i=0; i<50;i++){    
                MoverCursor(k,j);
                //StringLCDVar(buffer[i]);
                k++;
                if(buffer[i]==0x0D || buffer[i]==0x0A){
                    j++;                                 //Cambio de renglon por cada retorno de carro encontrado
                    k=0;
                    i++;
                }
                LetraLCD(buffer[i]);
            }                                  
            delay_ms(3000);
            BorrarLCD();
            f_close(&archivo); 
        }
        else{
            StringLCD("Archivo NO Encontrado");
        }
        res= f_open(&archivo, Archivo5, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);      
        if (res==FR_OK){
            f_read(&archivo, buffer, 50,&br); //leer archivo
            j=0;
            k=0; 
            BorrarLCD();  
            for(i=0; i<50;i++){                                              //Leyendo el screen5 e imprimiendo letra por letra en el LCD
                MoverCursor(k,j);
                //StringLCDVar(buffer[i]);
                k++;
                if(buffer[i]==0x0D || buffer[i]==0x0A){
                    j++;                                  //Cambio de renglon por cada retorno de carro encontrado
                    k=0;
                    i++;
                }
                LetraLCD(buffer[i]);
            }                                  
            delay_ms(3000);
            BorrarLCD();
            f_close(&archivo); 
        }
        else{
            StringLCD("Archivo NO Encontrado");
        }
        BorrarLCD();
    }
    else{
         StringLCD("Drive NO Detectado");
         while(1);
    }
    f_mount(0, 0); //Cerrar drive de SD
    while(1);
}
