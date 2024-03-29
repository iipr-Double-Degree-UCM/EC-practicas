/*******************************************************************************************
* File:		keyboard.c
* Author:	
* Desc:		Funciones de teclado
* History:	
*********************************************************************************************/

/*--- Ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "keyboard.h"
#include "buffer.h"
#include "8seg.h"
#include <stdio.h>
#include <stdlib.h>


/*--- Variables globales ---*/
volatile UCHAR *keyboard_base = (UCHAR *)0x06000000;
int key;
int modo = 1;

void Eint1_ISR(void) __attribute__ ((interrupt ("IRQ")));
void init_keyboard(void);
int key_read();
/*--- Codigo de las funciones ---*/

void EliminaRebotes(void) {
	int i,j,k;
	i=200000;
	j=0;
	while (j!=i){
		k=k*j;
		j+=1;
	}
}

void init_keyboard()
{
// A implementar por los alumnos
	 //Configuracion del controlador de interrupciones
		rI_ISPC=0xFFFFFFFF	;	   // Borra INTPND
		rEXTINTPND = 0xf; 		   // Borra EXTINTPND
		rINTMOD=0x0;	   // Configura las lineas como de tipo IRQ mediante rINTMOD
		rINTCON  =0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no) mediante rINTCON
			// Emascara todas las lineas excepto eint1y el bit global mediante rINTMSK
		rINTMSK = ~(BIT_GLOBAL|BIT_EINT1);
		/*Establece la rutina de servicio para Eint1 */
		pISR_EINT1 = (int)Eint1_ISR;
	    /* Configuracion del puerto G */
	    rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
		rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
		rEXTINT = rEXTINT|0x22222222;   // Configura las lineas de int. como de flanco de bajada

	    /* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
		rI_ISPC=0xFFFFFFFF	;	   // Borra INTPND
	 	rEXTINTPND = 0xf;	   // Borra EXTINTPND


}
void Eint1_ISR(){
	key = key_read();
	if (key == 15 || key == -1)
		EliminaRebotes();
	else
		insertarClave(key);


	EliminaRebotes();
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT1;		// borra el bit pendiente en INTPND
}
// Funcion que devuelve un entero (0..15) que
// representa la tecla pulsada
// Debe invocarse desde la rutina de tratamiento de interrupci�n
int key_read()
{
	int value;
	char temp;
	/* leer linea 1 */
	temp = *(keyboard_base+0xfd);
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 3;
		else if( (temp&0x2) == 0 )
			value = 2;
		else if( (temp&0x4) == 0 )
			value = 1;
		else if( (temp&0x8) == 0 )
			value = 0;
		return value;
	}
	/* linea 2 */
	temp = *(keyboard_base+0xfb);
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 7;
		else if( (temp&0x2) == 0 )
			value = 6;
		else if( (temp&0x4) == 0 )
			value = 5;
		else if( (temp&0x8) == 0 )
			value = 4;
		return value;
	}
	/* linea 3 */
	temp = *(keyboard_base+0xf7);
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xb;
		else if( (temp&0x2) == 0 )
			value = 0xa;
		else if( (temp&0x4) == 0 )
			value = 9;
		else if( (temp&0x8) == 0 )
			value = 8;
		return value;
	}
	/* linea 4 */
	temp = *(keyboard_base+0xef);
	if(( temp & KEY_VALUE_MASK) != KEY_VALUE_MASK)
	{
		if( (temp&0x1) == 0 )
			value = 0xf;
		else if( (temp&0x2) == 0 )
			value = 0xe;
		else if( (temp&0x4) == 0 )
			value = 0xd;
		else if( (temp&0x8) == 0 )
			value = 0xc;
		return value;
	}
	return -1;
}


