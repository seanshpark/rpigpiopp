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

#ifndef __RPIGPIOPP_LIB_LCD1602_H__
#define __RPIGPIOPP_LIB_LCD1602_H__

#include "pcf8574/pcf8574.h"

namespace rpigpiopp
{

class LCD1602
{
public:
  LCD1602() = default;
  virtual ~LCD1602() = default;

public:
  bool init(PCF8574 *pcf8574);
  void release(void);

public:
  bool initialized(void) { return _initalized; }

public:
  void test(void);

public:
  void clear();
  void home();
  void display(bool enable); // display on/off
  void cursor(bool enable);  // cursor on/off
  void blink(bool enable);   // blinking on/off
  void putc(const char c);
  void puts(const char *str);
  void move(uint8_t row, uint8_t col);

private:
  void function_set(uint8_t data);
  void cursor_set(uint8_t data);
  void display_set(void);
  void entrymode_set(uint8_t data);

private:
  void init_4bit(void);
  void send_4bits(uint8_t lcddata, uint32_t delay);
  void send_data(uint8_t data);
  void send_ctrl(uint8_t data);

private:
  void io_send_byte(uint8_t one);

private:
  PCF8574 *_pcf8574 = nullptr;
  bool _initalized = false;

  bool _back_light = false;
  bool _display = false;
  bool _cursor = false;
  bool _blink = false;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_LCD1602_H__
