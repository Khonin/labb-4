/*
 * lcd.c
 *
 *  Created on: Apr 27, 2021
 *      Author: axeand
 */
#include "lcd.h"
// D7 D6 D5 D4 BT E  RW RS

void TextLCD_Strobe(TextLCDType *lcd)
{
	// Set bit 2 which corresponds to E (strobe) and send data
	lcd->data |= 0x04;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	delay_us(50);
	//HAL_Delay(1);
	// Clear bit 2 which corresponds to E (strobe) and send data
	lcd->data &= 0xFB;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	//HAL_Delay(1);
}

void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd)
{
	// for command RS is set to 0
	lcd->data = (lcd->data & 0x0C) | (cmd & 0xF0);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	lcd->data = (lcd->data & 0x0C) | (cmd << 4);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
}

void TextLCD_Data(TextLCDType *lcd, uint8_t data)
{
	// for data RS is set to 1
	lcd->data = (lcd->data & 0x0D) | (data & 0xF0) | 0x01;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	lcd->data = (lcd->data & 0x0D) | (data << 4) | 0x01;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
}

void TextLCD_Init(TextLCDType *lcd, I2C_HandleTypeDef *hi2c, uint8_t DevAddress)
{
	lcd->hi2c = hi2c;
	lcd->DevAddress = DevAddress;
	lcd->data = 0x38;

	// Do init setting LCD controller into 4-bit mode
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	lcd->data = 0x28;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);

	// Finished setting up 4-bit mode. Let's configure display

	TextLCD_Cmd(lcd, 0x28); //N=1 (2 line), F=0 (5x8)
	TextLCD_Cmd(lcd, 0x08); //Display off, Cursor Off, Blink off
	TextLCD_Cmd(lcd, 0x01); //Clear
	//HAL_Delay(5);
	delay_us(50);
	TextLCD_Cmd(lcd, 0x06); //ID=1(increment), S=0 (no shift)
	TextLCD_Cmd(lcd, 0x0C); //Display on, Cursor Off, Blink off
}

void TextLCD_Home(TextLCDType *lcd)
{
	TextLCD_Cmd(&lcd, 0x2);
}

void TextLCD_Clear(TextLCDType *lcd)
{
	TextLCD_Cmd(&lcd, 0x1);
}

void TextLCD_Position(TextLCDType *lcd, int x, int y)
{
	for(int i=0;i<x +40*y;i++){
	TextLCD_Data(lcd, 0x14);		//shift cursor right 1 step
	delay_us(50);
	}
	TextLCD_Cmd(lcd, 0xF); // Turn display & cursor on, set cursor blinking
	delay_us(50);


}

void TextLCD_Putchar(TextLCDType *lcd, uint8_t data)
{

}

void TextLCD_Puts(TextLCDType *lcd, char *string)
{
}
#if 0
void TextLCD_Printf(TextLCDType *lcd, char *message, …)
{

}
#endif
