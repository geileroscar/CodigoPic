
#INCLUDE <16F877a.H>
#device adc=10
#USE DELAY(CLOCK=20000000)
#Fuses HS, NOWDT, NOPROTECT, NOLVP
#use rs232(baud=9600,xmit=PIN_c6,rcv=PIN_c7,stream=PC, parity=N, bits=8)
#DEFINE USE_PORTB_KBD  
#INCLUDE <LCD.C>
#INCLUDE <KBD4x4_2.C>  //Incluir en el encabezado el driver para
#use standard_io(c)
#use standard_io(b)
#define  KEYHIT_DELAY   200    //Tiempo de espera del teclado en milisegundos
#byte PORTB= 6
#byte PORTC= 7
#byte trisd=0x88
#byte portd=0x08
#BYTE PORTA= 0x05
#include <stdio.h>  // Incluimos las librerias stdio.h y string.h
#include <stdlib.h>
#include <string.h> // Variable de lectura de cada caracter

char string[16];                                     // Variable donde se almacena la cadena de caracteres

char limp [16]={"L."};
CHAR valor;
int cont=0;
int i=1; //Posicion del dato recibido a mostrar en el LCD
int ban=0; //Bandera que indica que llego un dato
long bits;     //Variable almacena los bits
float tem;     //Almacena el voltaje 
char cadena1[4];                                     // Cadena para el valor del pwm 1
int valor_pwm1;                      // Variables para los valores de los pwm
///DECLARACION DE FUNCIONES
char tecla(VOID);
char tecla_time(VOID);
void print_tecla(CHAR K);

//Define la interrupción por recepción Serial
#INT_RDA 
RDA_isr()
{
   fgets (string, Pc);
   valor=fgetc ();
   ban=1;
}

VOID MAIN()
{
   CHAR K;
   INT x=1;
   enable_interrupts (GLOBAL);//Habilita las interrupcciones globales
   enable_interrupts (INT_RDA);//HAbilita interrupcion por puerto serial
   
   port_b_pullups (0xFF);//Utiliza las resistencias PULL UP internas del puerto B
   set_tris_a (0b00000001);//Pongo el RA0 como entrada
   SET_TRIS_B (0);
   
   setup_adc_ports (all_analog);//Pongo todo el puerto a analogo
   setup_adc (adc_clock_internal);//Selecciono reloj interno para conversion
   setup_adc_ports (AN0);//Pongo RA0 como analogo
   setup_adc (adc_clock_internal);//Selecciono reloj interno para conversion
   
   lcd_init ();//Inicializa el LCD
   
   KBD_INIT ();//Inicializar el driver del teclado
   
   setup_timer_2 (t2_div_by_16, 255, 1) ;
   setup_ccp1 (ccp_pwm);
   set_pwm1_duty (0);
 
   
   FOR (;;)
   {
      IF (ban==1)//Hay datos para mostrar
      {
         ban=0;

         IF (string[0]=='P')
         {
            sprintf (cadena1, "%c%c%c", string[1], string[2], string[3]); //Concatena los caracteres almacenados en las posiciones 2, 3 y 4 de la cadena principal
            valor_pwm1=atoi (cadena1);//Convierte la cadena 1 en valor entero
            set_pwm1_duty (valor_pwm1);//Escribe el valor de pwm en el ccp1
         }

         
         IF (string[0]=='L')
         {
            CHAR cadena[16]="";
            strcpy (cadena,&string[1]);
              lcd_gotoxy (1, 1) ;
               //printf (lcd_putc, "%s", string);
               fprintf (PC, "Caracter=%s",&cadena);
               printf (lcd_putc, "%s",&cadena);
            }
         IF (string[0]=='D')
            { 
               CHAR cadena2[16]="";
               strcpy (cadena2,&string[1]);
               lcd_gotoxy (1, 2) ;
              fprintf (PC, "Caracter=%s",&cadena2);
              printf (lcd_putc, "%s",&cadena2);
            }

            IF (strcmp (string, limp)==0)
            {
               LCD_PUTC ("\f");
            }
         
      }

      IF (k!='\0')
      {
         print_tecla (K);
         k='\0';
      }

      k=tecla_time ();//Lee el valor del teclado pero solo espera un tiempo determinad
      set_adc_channel (0);//Selecciono el canal 0 (RA0)
      delay_ms (1);//llamo retardo de 1 ms
      bits=read_adc ();//Guarde el dato del potenciometro
      
      tem=bits*0.0048828125;//Conversion de bits a voltios
      printf ("voltaje=%2.3f\r\n #", tem);
      delay_ms (100);
   }
}

//Funcion encargada de esperar a que se presione una tecla 
char tecla(VOID)
{
   CHAR c;

   DO
   {
      //espera hasta que se presione una tecla
      c=kbd_getc ();//Captura valor del teclado
   }

   WHILE (c=='\0');
   RETURN (c);
}

// Pregunta por una Tecla por un tiempo, si no hay actividad, deja de preguntar
// y deja que el PIC CONTINUE con su trabajo
char tecla_time(VOID) 
{
   CHAR c='\0';
   UNSIGNED int16 timeout;
   timeout=0;
   c=kbd_getc ();//Captura valor del teclado
   WHILE (c=='\0'&& (++timeout< (KEYHIT_DELAY*100)))
   {
      delay_us (10);
      c=kbd_getc ();//Captura valor del teclado
   }

   RETURN (c);
}

void print_tecla(CHAR k)
{
   SWITCH (k)
   {
      CASE 55:
      printf ("tecla=%c#\r\n", '7');
      BREAK;

      CASE 56:
      printf ("tecla=%c#\r\n", '8');
      BREAK;

      CASE 57:
      printf ("tecla=%c#\r\n", '9');
      BREAK;

      CASE 54:
      printf ("tecla=%c#\r\n", '6');
      BREAK;

      CASE 53:
      printf ("tecla=%c#\r\n", '5');
      BREAK;

      CASE 52:
      printf ("tecla=%c#\r\n", '4');
      BREAK;

      CASE 51:
      printf ("tecla=%c#\r\n", '3');
      BREAK;

      CASE 50:
      printf ("tecla=%c#\r\n", '2');
      BREAK;

      CASE 49:
      printf ("tecla=%c#\r\n", '1');
      BREAK;

      CASE 48:
      printf ("tecla=%c#\r\n", '0');
      BREAK;

      CASE 65:
      printf ("tecla=%c#\r\n", 'A');
      BREAK;

      CASE 66:
      printf ("tecla=%c#\r\n", 'B');
      BREAK;

      CASE 67:
      printf ("tecla=%c#\r\n", 'C');
      BREAK;

      CASE 68:
      printf ("tecla=%c#\r\n", 'D');
      BREAK;

      CASE 35:
      printf ("tecla=%c#\r\n", '#');
      BREAK;

      CASE 42:
      printf ("tecla=%c#\r\n", '*');
      BREAK;
   }
}

