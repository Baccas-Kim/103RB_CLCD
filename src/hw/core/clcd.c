#include <stdio.h>
#include "clcd.h"
// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

//72MHz Operation mode intended, this function included Risk
void delay_us(uint32_t us){  // 72MHz
    if(us>1){
        uint32_t count=us*8-6;
        while(count--); //asm volatile("nop");
    }
    else{
        uint32_t count=2;
        while(count--); // asm volatile("nop");
    }
}
//[출처] STM32 Nucleo F103RB - us delay 만들기|작성자 제플린
//git test
//#define delay_ms HAL_Delay




_LCD_4BITMODE_
   Lcd_PortType data_ports[] = {
		D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
   Lcd_PinType  data_pins[] = {
		D4_Pin, D5_Pin, D6_Pin, D7_Pin};


 // Create new Lcd_HandleTypeDef and initialize the hlcd
Lcd_HandleTypeDef hlcd;


void command(uint8_t value);
size_t write(uint8_t value);
void send(uint8_t value, GPIO_PinState mode);
void pulseEnable(void);
void write4bits(uint8_t value);
void write8bits(uint8_t value);

void lcd_Init(uint8_t cols, uint8_t lines)
{
  hlcd.data_ports = data_ports;  // LCD Data Pins ������ ��Ʈ ������ ���� �迭�� hlcd.data_ports �� ����Ʈ �Ҵ�
  hlcd.data_pins = data_pins;    // LCD Data Pins ������ pins ������ ���� �迭�� hlcd.data_pins �� ����Ʈ �Ҵ�
  hlcd.displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;


  if (lines > 1) {
    hlcd.displayfunction |= LCD_2LINE;
  }
  hlcd.numlines = lines;

  // for some 1 line displays you can select a 10 pixel high font
  if ((LCD_DOTSIZE != LCD_5x8DOTS) && (lines == 1)) {
    hlcd.displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  HAL_Delay(50-1);
  // Now we pull both RS and R/W low to begin commands
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);  // digitalWrite(_rs_pin, LOW);
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);  // digitalWrite(_enable_pin, LOW);
    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delay_us(4500);    //delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delay_us(100);    //delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delay_us(100);    //delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02);
    delay_us(100);
  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | hlcd.displayfunction);  

  // turn the display on with no cursor or blinking default
  hlcd.displaycontrol = LCD_DISPLAYOFF | LCD_CURSOROFF | LCD_BLINKOFF;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);

  // clear it off
  lcd_clear();
  HAL_Delay(3-1);

  // Initialize to default text direction (for romance languages)
  hlcd.displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | hlcd.displaymode);

  lcd_display();
}


/********** high level commands, for the user! */
void lcd_clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  HAL_Delay(2-1);                //  this command takes a long time!
}

void lcd_home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  HAL_Delay(2-1);              // this command takes a long time!
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
  uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > hlcd.numlines ) {
    row = hlcd.numlines-1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void lcd_noDisplay() {
  hlcd.displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}

void lcd_display() {
  hlcd.displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor() {
  hlcd.displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}
void lcd_cursor() {
  hlcd.displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink() {
  hlcd.displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}
void lcd_blink() {
  hlcd.displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | hlcd.displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcd_scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void lcd_scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
  hlcd.displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | hlcd.displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
  hlcd.displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | hlcd.displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
  hlcd.displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | hlcd.displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
  hlcd.displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | hlcd.displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

//====================== ���� �� TEXT ó�� �Լ� ======================================
void lcd_putchar(char c) {
	write(c);
}

void lcd_string(char * str_data) {
   while(*str_data) {
	  write(*str_data++);
   }
}

void lcd_setCurStr(uint8_t col, uint8_t row,  char * str) {
	lcd_setCursor(col, row);
	lcd_string(str);
}

/*********** mid level commands, for sending data/cmds */

void command(uint8_t value) {
  send(value, GPIO_PIN_RESET);
}

size_t write(uint8_t value) {
  send(value, GPIO_PIN_SET);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send(uint8_t value, GPIO_PinState mode) {
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, mode);   // digitalWrite(_rs_pin, mode);
  write4bits(value>>4);
  write4bits(value);
}

void pulseEnable(void) {
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
  delay_us(1);              //
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
  delay_us(1);   			   // enable pulse must be >450ns
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
  delay_us(100);          // commands need > 37us to settle
}

void write4bits(uint8_t value) {
  for (uint8_t i = 0; i < 4; i++) {
    HAL_GPIO_WritePin(hlcd.data_ports[i], hlcd.data_pins[i],  (value >> i) & 0x01 );
  }

  pulseEnable();
}
