#ifndef CLCD_STM32_h
#define CLCD_STM32_h

/*
 *

#include "clcd.h"

 *
 */

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "def.h"



// commands
#define LCD_CLEARDISPLAY        ((uint8_t)0x01U)
#define LCD_RETURNHOME          ((uint8_t)0x02U)
#define LCD_ENTRYMODESET        ((uint8_t)0x04U)
#define LCD_DISPLAYCONTROL      ((uint8_t)0x08U)
#define LCD_CURSORSHIFT         ((uint8_t)0x10U)
#define LCD_FUNCTIONSET         ((uint8_t)0x20U)
#define LCD_SETCGRAMADDR        ((uint8_t)0x40U)
#define LCD_SETDDRAMADDR        ((uint8_t)0x80U)

// flags for display entry mode
#define LCD_ENTRYRIGHT          ((uint8_t)0x00U)
#define LCD_ENTRYLEFT           ((uint8_t)0x02U)
#define LCD_ENTRYSHIFTINCREMENT ((uint8_t)0x01U)
#define LCD_ENTRYSHIFTDECREMENT ((uint8_t)0x00U)

// flags for display on/off control
#define LCD_DISPLAYON           ((uint8_t)0x04U)
#define LCD_DISPLAYOFF          ((uint8_t)0x00U)
#define LCD_CURSORON            ((uint8_t)0x02U)
#define LCD_CURSOROFF           ((uint8_t)0x00U)
#define LCD_BLINKON             ((uint8_t)0x01U)
#define LCD_BLINKOFF            ((uint8_t)0x00U)

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         ((uint8_t)0x08U)
#define LCD_CURSORMOVE          ((uint8_t)0x00U)
#define LCD_MOVERIGHT           ((uint8_t)0x04U)
#define LCD_MOVELEFT            ((uint8_t)0x00U)

// flags for function set
#define LCD_2LINE               ((uint8_t)0x08U)
#define LCD_1LINE               ((uint8_t)0x00U)
#define LCD_5x10DOTS            ((uint8_t)0x04U)
#define LCD_5x8DOTS             ((uint8_t)0x00U)


#define LCD_8BITMODE            ((uint8_t)0x10U)
#define LCD_4BITMODE            ((uint8_t)0x00U)

//#define _LCD_8BITMODE_        // 8bit ����, 4bit ���� �ּ�ó��
#define	_LCD_4BITMODE_          // 4bit ����, 8bit ���� �ּ�ó��

#define LCD_DOTSIZE             LCD_5x8DOTS   // CLCD DOT SIZE �� ���� ����

/************************************** Helper macros **************************************/


/************************************** LCD typedefs **************************************/
#define Lcd_PortType GPIO_TypeDef*
#define Lcd_PinType uint16_t

/*************************** LCD GPIO Port, Pin Definition *********************************/
// CubeMx GPIO_OUT Pin ������ ���� ����, CubeMX ���� USER LABEL �� �����Ȱ��
// main.h ���� Ȯ���� ������ �ʵǾ����� �Ʒ��� ���� ����
  //  *********  define hard wired pin  *********
  #define EN_Pin GPIO_PIN_9
  #define EN_GPIO_Port GPIOA

  #define RS_Pin GPIO_PIN_7
  #define RS_GPIO_Port GPIOC

  #define D4_Pin GPIO_PIN_5
  #define D4_GPIO_Port GPIOB

  #define D5_Pin GPIO_PIN_4
  #define D5_GPIO_Port GPIOB

  #define D6_Pin GPIO_PIN_10
  #define D6_GPIO_Port GPIOB

  #define D7_Pin GPIO_PIN_8
  #define D7_GPIO_Port GPIOA
  //  *********  define hard wired pin  *********
  /*
   * note : if user want other types of CLCD change this defines
   */

//
//#define EN_Pin GPIO_PIN_13
//#define EN_GPIO_Port GPIOB
//#define RS_Pin GPIO_PIN_14
//#define RS_GPIO_Port GPIOB
//#define D4_Pin GPIO_PIN_4
//#define D4_GPIO_Port GPIOB
//#define D5_Pin GPIO_PIN_5
//#define D5_GPIO_Port GPIOB
//#define D6_Pin GPIO_PIN_6
//#define D6_GPIO_Port GPIOB
//#define D7_Pin GPIO_PIN_7
//#define D7_GPIO_Port GPIOB


// #define D4_GPIO_Port       GPIOB
// #define D5_GPIO_Port       GPIOB
// #define D6_GPIO_Port       GPIOB
// #define D7_GPIO_Port       GPIOB

// #define D4_Pin
// #define D5_Pin
// #define D6_Pin
// #define D7_Pin

// #define RS_GPIO_Port       GPIOB
// #define EN_GPIO_Port       GPIOB

// #define RS_Pin             ((uint16_t) 14)
// #define EN_Pin             ((uint16_t) 13)


typedef struct {
	Lcd_PortType * data_ports;
	Lcd_PinType  * data_pins;

	uint8_t displayfunction;
	uint8_t displaycontrol;
	uint8_t displaymode;

	uint8_t numlines;

} Lcd_HandleTypeDef;


extern Lcd_HandleTypeDef hlcd;
/*************************** LCD Function Definition *********************************/

void lcd_Init(uint8_t cols, uint8_t lines);

void lcd_clear();
void lcd_home();

void lcd_noDisplay();
void lcd_display();
void lcd_noBlink();
void lcd_blink();
void lcd_noCursor();
void lcd_cursor();
void lcd_scrollDisplayLeft();
void lcd_scrollDisplayRight();
void lcd_leftToRight();
void lcd_rightToLeft();
void lcd_autoscroll();
void lcd_noAutoscroll();

void lcd_createChar(uint8_t, uint8_t[]);
void lcd_setCursor(uint8_t, uint8_t);

void lcd_putchar(char c);
void lcd_string(char * str_data);
void lcd_setCurStr(uint8_t col, uint8_t row, char * str);
#define lcd_put2digitNum(col, row, val)      lcd_putdigitsNum(col, row, val, 2)
void lcd_putdigitsNum(uint8_t col, uint8_t row, int val, uint8_t digits);

void lcd_dateStr(uint8_t col, uint8_t row, RTC_DateTypeDef const * date, char sep);
void lcd_timeStr(uint8_t col, uint8_t row, RTC_TimeTypeDef const * time, char sep);


void delay_us(uint32_t us);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
