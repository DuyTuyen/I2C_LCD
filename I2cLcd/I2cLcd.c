#include "I2cLcd.h"
#include "print_debug.h"
#include "stm32f0xx_hal.h"

static uint8_t _Addr = 0x27;
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _numlines;
//static uint8_t _cols;
//static uint8_t _rows;
static uint8_t _backlightval = 0x08;

extern I2C_HandleTypeDef hi2c1;

static void I2cLcd_Write(uint8_t _data)
{
	uint8_t Dat = _data | _backlightval;
//	uint8_t ack;
//	
//	I2C_Start();
//	ack = I2C_Write(_Addr << 1);
//	if(ack == 1)
//	{
//		PrintDebug("%d - RECV NACK %d\r\n",__LINE__,ack);
//	}
//	ack = I2C_Write(_data | _backlightval);
//	if(ack == 1)
//	{
//		PrintDebug("%d - RECV NACK %d\r\n",__LINE__,ack);
//	}
//	I2C_Stop();
	if(HAL_I2C_Master_Transmit(&hi2c1,_Addr<<1,&Dat,1,1000) != HAL_OK)
	{
		PrintDebug("%d - I2C Write Fail\r\n",__LINE__);
	}
}

static void I2cLcd_Enable(uint8_t _data)
{
	I2cLcd_Write((_data | En));	// En high
	delay_us(50);		// enable pulse must be >450ns
	
	I2cLcd_Write(_data & ~En);	// En low
	delay_us(50);		// commands need > 37us to settle
}

static void I2cLcd_Write4Bit(uint8_t value)
{
	I2cLcd_Write(value);
	I2cLcd_Enable(value);
}

static void I2cLcd_Send(uint8_t value, uint8_t mode)
{
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
  I2cLcd_Write4Bit((highnib)|mode);
	I2cLcd_Write4Bit((lownib)|mode); 
}

static void I2cLcd_Command(uint8_t value)
{
	I2cLcd_Send(value,0);
}

/********** high level commands, for the user! */
void I2cLcd_Clear(void)
{
	I2cLcd_Command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	delay_us(2000);  // this command takes a long time!
}

void I2cLcd_Home(void)
{
	I2cLcd_Command(LCD_RETURNHOME);  // set cursor position to zero
	delay_us(2000);  // this command takes a long time!
}

void I2cLcd_SetCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x80, 0xC0, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	I2cLcd_Command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void I2cLcd_NoDisplay(void)
{
	_displaycontrol &= ~LCD_DISPLAYON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2cLcd_Display(void) 
{
	_displaycontrol |= LCD_DISPLAYON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void I2cLcd_NoCursor(void) 
{
	_displaycontrol &= ~LCD_CURSORON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2cLcd_Cursor(void) 
{
	_displaycontrol |= LCD_CURSORON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void I2cLcd_NoBlink(void) 
{
	_displaycontrol &= ~LCD_BLINKON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void I2cLcd_Blink(void) 
{
	_displaycontrol |= LCD_BLINKON;
	I2cLcd_Command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void I2cLcd_ScrollDisplayLeft(void) 
{
	I2cLcd_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void I2cLcd_ScrollDisplayRight(void)
{
	I2cLcd_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void I2cLcd_LeftToRight(void) 
{
	_displaymode |= LCD_ENTRYLEFT;
	I2cLcd_Command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void I2cLcd_RightToLeft(void) 
{
	_displaymode &= ~LCD_ENTRYLEFT;
	I2cLcd_Command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void I2cLcd_Autoscroll(void) 
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	I2cLcd_Command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void I2cLcd_NoAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	I2cLcd_Command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void I2cLcd_CreateChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	I2cLcd_Command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		I2cLcd_PutChar(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void I2cLcd_NoBacklight(void)
{
	_backlightval=LCD_NOBACKLIGHT;
	I2cLcd_Write(_backlightval);
}

void I2cLcd_Backlight(void) 
{
	_backlightval=LCD_BACKLIGHT;
	I2cLcd_Write(_backlightval);
}


void I2cLcd_PutChar(uint8_t chr)
{
	I2cLcd_Send(chr,1);
}

void I2cLcd_Puts(uint8_t *s)
{
	while(*s)
	{
		I2cLcd_PutChar(*s);
		s++;
	}
}


void I2cLcd_Init(void)
{
	_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
	_numlines = 2;
	HAL_Delay(50);
	I2cLcd_Write(_backlightval);
	HAL_Delay(1000);
	  // we start in 8bit mode, try to set 4 bit mode
   I2cLcd_Write4Bit(0x03 << 4);
   delay_us(4500); // wait min 4.1ms
   
   // second try
   I2cLcd_Write4Bit(0x03 << 4);
   delay_us(4500); // wait min 4.1ms
   
   // third go!
   I2cLcd_Write4Bit(0x03 << 4); 
   delay_us(150);
   
   // finally, set to 4-bit interface
   I2cLcd_Write4Bit(0x02 << 4); 


	// set # lines, font size, etc.
	I2cLcd_Command(LCD_FUNCTIONSET | _displayfunction);  
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	I2cLcd_Display();
	
	// clear it off
	I2cLcd_Clear();
	
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	I2cLcd_Command(LCD_ENTRYMODESET | _displaymode);
	
	I2cLcd_Home();
}












