/*
 * Copyright 2022 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lcd1602/lcd1602.h"

#include <iostream>
#include <cassert>

#include <unistd.h> // usleep

namespace
{

// clang-format off
#define HD44780_LCD_CMD_DDRAMADDR     0b10000000
#define HD44780_LCD_CMD_CGRAMADDR     0b01000000
#define HD44780_LCD_CMD_FUNCSET       0b00100000
#define HD44780_LCD_CMD_CURSOR        0b00010000
#define HD44780_LCD_CMD_DISPLAY       0b00001000
#define HD44780_LCD_CMD_ENTRY         0b00000100
#define HD44780_LCD_CMD_RETHOME       0b00000010
#define HD44780_LCD_CMD_CLEAR         0b00000001

// HD44780_LCD_CMD_FUNCSET
#define HD44780_LCD_FUNCSET_8BIT      0b00010000
#define HD44780_LCD_FUNCSET_4BIT      0b00000000
#define HD44780_LCD_FUNCSET_2LINES    0b00001000
#define HD44780_LCD_FUNCSET_1LINES    0b00000000
#define HD44780_LCD_FUNCSET_5x11      0b00000100
#define HD44780_LCD_FUNCSET_5x8       0b00000000

// HD44780_LCD_CMD_CURSOR
#define HD44780_LCD_CURSOR_SHIFT_DIS  0b00001000 // display shift
#define HD44780_LCD_CURSOR_SHIFT_CUR  0b00000000 // cursor move
#define HD44780_LCD_CURSOR_RIGHT      0b00000100 // shift to the right
#define HD44780_LCD_CURSOR_LEFT       0b00000000 // shift to the left

// HD44780_LCD_CMD_DISPLAY
#define HD44780_LCD_DISPLAY_ON        0b00000100
#define HD44780_LCD_DISPLAY_OFF       0b00000000
#define HD44780_LCD_DISPLAY_CUR       0b00000010
#define HD44780_LCD_DISPLAY_CUR_OFF   0b00000000
#define HD44780_LCD_DISPLAY_BLINK     0b00000001
#define HD44780_LCD_DISPLAY_BLINK_OFF 0b00000000

// HD44780_LCD_CMD_ENTRY
#define HD44780_LCD_ENTRY_INC         0b00000010 // increment vs decrement
#define HD44780_LCD_ENTRY_SHIFT       0b00000001 // entire shift on vs off

#define PCF8574_LCD1604_RS 0b00000001 // LCD160x_RS (Register Select: Inst / Data)
#define PCF8574_LCD1604_RW 0b00000010 // LCD160x_RW (R or /W)
#define PCF8574_LCD1604_EN 0b00000100 // LCD160x_EN (Enable)
#define PCF8574_LCD1604_BL 0b00001000 // LCD160x_BL (BackLignt)

// clang-format on

} // namespace

namespace rpigpiopp
{

bool LCD1602::init(PCF8574 *pcf8574)
{
  std::cout << "LCD1602::init with PCF8574" << std::endl;

  _pcf8574 = pcf8574;

  _display = true;
  _cursor = true;
  _blink = true;

  // turn on back-light
  _back_light = true;

  init_4bit();
  usleep(100);

  function_set(HD44780_LCD_FUNCSET_4BIT | HD44780_LCD_FUNCSET_2LINES | HD44780_LCD_FUNCSET_5x8);
  usleep(100);

  cursor_set(HD44780_LCD_CURSOR_SHIFT_CUR | HD44780_LCD_CURSOR_RIGHT);
  usleep(100);

  display_set();
  usleep(100);

  entrymode_set(HD44780_LCD_ENTRY_INC);
  usleep(100);

  _initalized = true;

  clear();
  usleep(100);

  return true;
}

void LCD1602::release(void)
{
  _pcf8574 = nullptr;
  _initalized = false;
  _back_light = false;

  std::cout << "LCD1602::release" << std::endl;
}

void LCD1602::test(void) { puts("Hello !!!"); }

void LCD1602::clear()
{
  uint8_t cmd = HD44780_LCD_CMD_CLEAR;
  send_ctrl(cmd);
  usleep(5000); // wait 5ms
}

void LCD1602::home()
{
  uint8_t cmd = HD44780_LCD_CMD_RETHOME;
  send_ctrl(cmd);
  usleep(1600); // spec says 1.52ms
}

void LCD1602::display(bool enable)
{
  _display = enable;
  display_set();
  usleep(50);
}

void LCD1602::cursor(bool enable)
{
  _cursor = enable;
  display_set();
  usleep(50);
}

void LCD1602::blink(bool enable)
{
  _blink = enable;
  display_set();
  usleep(50);
}

void LCD1602::putc(const char c)
{
  send_data(c);
  usleep(50);
}

void LCD1602::puts(const char *str)
{
  while (*str != '\x0')
  {
    putc(*str++);
  }
}

void LCD1602::move(uint8_t row, uint8_t col)
{
  uint8_t cmd = HD44780_LCD_CMD_DDRAMADDR;
  uint8_t ram_offset;

  // row 0 : 0x00 ~ 0x27
  // row 1 : 0x40 ~ 0x67
  if (row > 0x01)
    row = 0x01; // use provide 2 lines
  if (col > 0x27)
    col = 0x27;

  ram_offset = row * 0x40 + col;
  ram_offset &= 0b01111111;

  send_ctrl(cmd + ram_offset);
  usleep(50);
}

} // namespace rpigpiopp

namespace rpigpiopp
{

void LCD1602::init_4bit(void)
{
  uint8_t lcddata;
  uint8_t data;

  // send RS=0, RW=0, DB7~DB4=0011 as 8bit 3 times
  data = HD44780_LCD_CMD_FUNCSET | HD44780_LCD_FUNCSET_8BIT;

  lcddata = data & 0xf0;
  send_4bits(lcddata, 4500);

  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);

  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);

  // send RS=0, RW=0, DB7~DB4=0010 as 4bit 1 time
  data = HD44780_LCD_CMD_FUNCSET;
  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);
}

void LCD1602::send_4bits(uint8_t lcddata, uint32_t delay)
{
  // this sends 4bits to DB4~DB7 of HD44780
  // lower 4bits are used for control.
  // to write, send bits + EN high, wait 2usec, drop EN low, wait delay
  // data will be written falling edge
  lcddata |= PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  io_send_byte(lcddata);
  usleep(2);

  lcddata &= ~PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  io_send_byte(lcddata);
  usleep(delay);
}

void LCD1602::send_data(uint8_t data)
{
  uint8_t lcddata;

  // TODO support send data with 8bits

  // RS high is to select DATA
  // bit 7~4
  lcddata = (data & 0xf0) | PCF8574_LCD1604_RS;
  send_4bits(lcddata, 10);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4) | PCF8574_LCD1604_RS;
  send_4bits(lcddata, 10);
}

void LCD1602::send_ctrl(uint8_t data)
{
  uint8_t lcddata;

  // TODO support send data with 8bits

  // RS low is to select CONTROL
  // bit 7~4
  lcddata = (data & 0xf0);
  send_4bits(lcddata, 10);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4);
  send_4bits(lcddata, 10);
}

void LCD1602::function_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_FUNCSET;
  cmd |= data;
  send_ctrl(cmd);
}

void LCD1602::cursor_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_CURSOR;
  cmd |= data;
  send_ctrl(cmd);
}

void LCD1602::display_set(void)
{
  uint8_t cmd = HD44780_LCD_CMD_DISPLAY;
  cmd |= _display ? HD44780_LCD_DISPLAY_ON : 0;
  cmd |= _cursor ? HD44780_LCD_DISPLAY_CUR : 0;
  cmd |= _blink ? HD44780_LCD_DISPLAY_BLINK : 0;
  send_ctrl(cmd);
}

void LCD1602::entrymode_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_ENTRY;
  cmd |= data;
  send_ctrl(cmd);
}

} // namespace rpigpiopp

namespace rpigpiopp
{

void LCD1602::io_send_byte(uint8_t one)
{
  if (_pcf8574)
    _pcf8574->write(one);
  else
    assert(false);
}

} // namespace rpigpiopp
