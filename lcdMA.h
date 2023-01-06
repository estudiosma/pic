/**
 ==============================================================================
 * @file    lcdMA.h
 * @author  Estudios MA | Ing. Danilo Molina
 * @contact estudiosma01@gmail.com
 * @site    https://linkfly.to/EstudiosMA
 * @version 1.0
 * @date    27-nov-2022
 * @brief   LCD 16x2 module driver.
 *          Este archivo proporciona funciones de firmware para las siguientes
 *          funcionalidades de una LCD 16x2:
 *            + Incializalizacion de la LCD.
 *            + Escribir caracteres, strings y numeros con funciones directas.
 *
 ==============================================================================
                            #### lcdMA.h ####
 ============================================================================== 
 * [..]
 * Probada en: 
 *  + MA-PIC version 3, entrenadora oficial de Estudios MA para pic de 40 pines.
 *  + PIC16F877A
 *  + PIC16F18875
 *  + PIC18F47Q10
 *  + PIC18F45K50
 * 
  ============================================================================== 
 * 
 * [..]
 * La libreria tiene por defecto la siguiente configuracion de pines:
 * @RS  RC4
 * @EN  RC5
 * @D7  RD7
 * @D6  RD6
 * @D5  RD5
 * @D4  RD4
 * 
 ============================================================================== 
 * 
 * @note1: En otras librerias los puertos restantes RD0-RD3 estan configurados
 * como entradas y no se puede cambiar para usar como salidas. 
 * 
 * 
 * 
 * @note2: los pines de RS, EN, D4-D7 tiene que ser declarados como salidas
 * en MCC o antes de usar la funcion LCD_Init()
 * 
 *
 * 
 ============================================================================== 
 * 
 * 
 * 
 * 
    [..]
 
    @Example:

    int num = 2;
    
    LCD_Init();
    LCD_Clear();
    LCD_Print("Estudios MA", 1, 3);
    
    LCD_Set_Cursor(1,1);
    LCD_Print_Int(num, 2, 3);
    
    char buffer[16];
    sprintf(buffer, "Temp: %i", 24);
    LCD_Print(buffer, 2, 6);
    __delay_ms(1000);
 *
 * 
 * 
 * 
 ****************************************************************************** 
 */

/*
 * @note Los pines definidos a continuacion tienen que ser configurados como 
 * salidas antes de inicializar la LCD.
 */
#define RS  LATCbits.LATC4
#define EN  LATCbits.LATC5

#define D4  LATDbits.LATD4
#define D5  LATDbits.LATD5
#define D6  LATDbits.LATD6
#define D7  LATDbits.LATD7

/* Librerias */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Funiones estaticas */
static void LCD_Port(char pin);
static void LCD_Cmd(char a);
static void LCD_Write_Char(char caracter);

/* Funiones publicas */
void LCD_Clear(void);
void LCD_Set_Cursor(char row, char col);
void LCD_Init(void);
void LCD_Write_String(char *txt);
void LCD_Shift_Right(void);
void LCD_Shift_Left(void);
void LCD_Print_Int(int num, char row, char col);
void LCD_Print(char *txt, char row, char col);

/*
 * @brief
 * @param   
 * @retval  None.
 */
static void LCD_Port(char pin) {
    if (pin & 1)
        D4 = 1;
    else
        D4 = 0;

    if (pin & 2)
        D5 = 1;
    else
        D5 = 0;

    if (pin & 4)
        D6 = 1;
    else
        D6 = 0;

    if (pin & 8)
        D7 = 1;
    else
        D7 = 0;
}

/*
 * @brief   Envia cmd a la LCD.
 * @param   Comando.
 * @retval  None.
 */
static void LCD_Cmd(char a) {
    RS = 0; // => RS = 0
    LCD_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

/*
 * @brief   Imprime un caracter.
 * @param   caracter: caracter a imprimir.
 * @retval  None.
 */
static void LCD_Write_Char(char caracter) {
    char temp, y;
    temp = caracter & 0x0F;
    y = caracter & 0xF0;
    RS = 1; 
    LCD_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    LCD_Port(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

/*
 * @brief   Limia toda la pantalla de la LCD.
 * @param   None.
 * @retval  None.
 */
void LCD_Clear(void) {
    LCD_Cmd(0);
    LCD_Cmd(1);
}

/*
 * @brief   Establece la posicion del cursor. Tiene que se llamada antes de 
 *          imprimir el mensaje.
 * @param   row: fila de la LCD.
 *              @arg para LCD 16x2 este valor puede ser 1 o 2.
 *          col: columna de la LCD
 *              @arg para LCD 16x2 este valor puede ser del 1 al 16.
 * @retval  None.
 */
void LCD_Set_Cursor(char row, char col) {
    char temp, z, y;
    if (row == 1) {
        temp = 0x80 + col - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        LCD_Cmd(z);
        LCD_Cmd(y);
    } else if (row == 2) {
        temp = 0xC0 + col - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        LCD_Cmd(z);
        LCD_Cmd(y);
    }
}

/*
 * @brief   Incializa la LCD 16x2.
 * @param   None.
 * @retval  None.
 */
void LCD_Init(void) {
    LCD_Port(0x00);
    __delay_ms(20);
    LCD_Cmd(0x03);
    __delay_ms(5);
    LCD_Cmd(0x03);
    __delay_ms(11);
    LCD_Cmd(0x03);
    //////////////
    LCD_Cmd(0x02);
    LCD_Cmd(0x02);
    LCD_Cmd(0x08);
    LCD_Cmd(0x00);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x00);
    LCD_Cmd(0x06);
}

/*
 * @brief   Imprime un string.
 * @param   txt: puntero del string (vector).
 * @retval  None.
 */
void LCD_Write_String(char *txt) {
    int i;
    for (i = 0; txt[i] != '\0'; i++)
        LCD_Write_Char(txt[i]);
}

/*
 * @brief   Desplaza todos los caracteres a la derecha.
 * @param   None.
 * @retval  None.
 */
void LCD_Shift_Right(void) {
    LCD_Cmd(0x01);
    LCD_Cmd(0x0C);
}

/*
 * @brief   Desplaza todos los caracteres a la izquierda.
 * @param   None.
 * @retval  None.
 */
void LCD_Shift_Left(void) {
    LCD_Cmd(0x01);
    LCD_Cmd(0x08);
}

/*
 * @brief   Imprime un numero de tipo Integer.
 * @param   num: numero tipo int
 *          row: fila de la LCD.
 *              @arg para LCD 16x2 este valor puede ser 1 o 2.
 *          col: columna de la LCD
 *              @arg para LCD 16x2 este valor puede ser del 1 al 16.
 * @retval  None.
 */
void LCD_Print_Int(int num, char row, char col) {
    LCD_Set_Cursor(row,col);
    char buff[16];
    sprintf(buff, "%i", num);
    LCD_Write_String(buff);
}

/*
 * @brief   Imprime un string.
 * @param   txt: puntero del string (vector).
 *          row: fila de la LCD.
 *              @arg para LCD 16x2 este valor puede ser 1 o 2.
 *          col: columna de la LCD
 *              @arg para LCD 16x2 este valor puede ser del 1 al 16.
 * @retval  None.
 */
void LCD_Print(char *txt, char row, char col){
    LCD_Set_Cursor(row,col);
    LCD_Write_String(txt);
}