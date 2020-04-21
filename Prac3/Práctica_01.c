#include <90USB1286.h>
#include <delay.h>
#include <stdio.h>
#include <ff.h>
#include "display.h"
#include <string.h>
#include <math.h>

char NombreArchivo[]  = "0:numeros.txt";
char Acomodados[] = "0:ordenado.txt";
int numeros[60];


interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
disk_timerproc();
/* MMC/SD/SD HC card access low level timing function */
}
    

void main(){
    unsigned int  br, i=0, j=0, k=0, aux, AUX=0;
    char buffer[400];    
    char auxiliar[7];
 
    
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
        /*Lectura de Archivo*/ 
        MoverCursor(0,1);
        StringLCD("Drive Detectado");
        delay_ms(1500);
        res = f_open(&archivo, NombreArchivo, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if (res==FR_OK){  
            MoverCursor(0,1);
            StringLCD("Archivo Encontrado ");
            delay_ms(1500);           
            f_read(&archivo, buffer, 400,&br); //leer archivo  
            
            i=0;
            while(buffer[i]!=NULL){ //hasta que no termine el archivo seguir
                if(buffer[i]>47 && buffer[i]<58){  //ascii numeros 0 al 9 
                    k++;
                    i++;  
                }
                     
                else if(buffer[i]==44) {  //algoritmo desarrollado para poder guardar los numeros separados en buffer en un solo elemento de la matriz llamada numeros
                    switch(k){  
                        case 1: 
                            AUX = (int)((buffer[i-1])-48); //unidades
                            break;                 
                        case 2:
                            AUX = (int)((((buffer[i-2])-48)*10)+(((buffer[i-1])-48)));  //decenas
                            break;                             
                        case 3: 
                            AUX = (int)((((buffer[i-3])-48)*100)+(((buffer[i-2])-48)*10)+((buffer[i-1])-48));      //centenas
                            break;          
                        case 4:   
                            AUX = (int)((((buffer[i-4])-48)*1000)+(((buffer[i-3])-48)*100)+(((buffer[i-2])-48)*10)+((buffer[i-1])-48));          //millares
                            break;
                    }   
                    numeros[j] = AUX;       
                    i++;
                    j++;
                    k=0; 
                    AUX=0;
                }
                else{
                i++;
                }         
                }
            
            
            f_close(&archivo);
                                                                 
            for (i=0; i<60; i++){   //algoritmo bubble sort ascendente (menor a mayor)
                    for (j=0; j<(60-1); j++){
                        if (numeros[j] > numeros[j+1]) /* For decreasing order use < */{
                        aux = numeros[j];
                        numeros[j]   = numeros[j+1];
                        numeros[j+1] = aux;
                        }
                    }             
            }                
               
        }
           
       f_open(&archivo, Acomodados, FA_OPEN_ALWAYS  | FA_WRITE | FA_READ);  //escritura del archivo ordenado
       i=0;
            while(i<60){
                sprintf(auxiliar,"%d,",numeros[i]);   //guardar en una matriz auxiliar uno a uno para imprimir en el archivo el decimal entero con una coma al final 
                f_write(&archivo,auxiliar,sizeof(auxiliar),&br); //escritura del archivo
                i++;
            }   
       
       f_close(&archivo);           

    }
    else{
        while(1);
    }   
    f_mount(0, 0); //Cerrar drive de SD
    while(1);
}
