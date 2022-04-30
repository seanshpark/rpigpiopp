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

#include "oled128x64/oled128x64.h"

#include <iostream>
#include <cassert>
#include <cstring> // memcpy

#include <unistd.h> // usleep

namespace rpigpiopp
{

bool OLED128x64::init(I2C *i2c)
{
  std::cout << "OLED128x64::init with I2C" << std::endl;

  _i2c = i2c;
  _buff_size = 1056; // 128*64/8 + 32
  _temp_buff = new uint8_t[_buff_size];
  _initalized = true;

  init_ssd1306();

  return true;
}

void OLED128x64::release(void)
{
  _i2c = nullptr;
  _initalized = false;
  if (_temp_buff)
  {
    delete _temp_buff;
    _temp_buff = nullptr;
  }
  _buff_size = 0;

  std::cout << "OLED128x64::release" << std::endl;
}

/**
 * @brief send one command
 *
 * @param v command of SSD1306
 * @return true when success
 * @return false when failed
 */
bool OLED128x64::cmd(uint8_t v)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }
  _temp_buff[0] = 0x00; // command for SSD1306
  _temp_buff[1] = v;
  return _i2c->write_buffer(_temp_buff, 2);
}

/**
 * @brief send one byte of data
 *
 * @param v datato send
 * @return true when success
 * @return false when failed
 */
bool OLED128x64::data(uint8_t v)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  _temp_buff[0] = 0x40; // data for SSD1306
  _temp_buff[1] = v;

  return _i2c->write_buffer(_temp_buff, 2);
}

/**
 * @brief send data from buffer with size
 *
 * @param b buffer address
 * @param s size of buffer
 * @return true when success
 * @return false when failed
 */
bool OLED128x64::data(uint8_t *b, size_t s)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  if (_buff_size < s + 1)
  {
    if (_temp_buff != nullptr)
    {
      delete _temp_buff;
    }
    _temp_buff = new uint8_t[s + 1];
    _buff_size = s + 1;
  }

  _temp_buff[0] = 0x40; // data for SSD1306
  memcpy(&_temp_buff[1], b, s);

  return _i2c->write_buffer(_temp_buff, s + 1);
}

bool OLED128x64::data(uint8_t *b, uint32_t s, uint32_t l)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return false;
  }

  if (_buff_size < s + 1)
  {
    if (_temp_buff != nullptr)
    {
      delete _temp_buff;
    }
    _temp_buff = new uint8_t[l + 1];
    _buff_size = l + 1;
  }

  _temp_buff[0] = 0x40; // data for SSD1306
  memcpy(&_temp_buff[1], b + s, l);

  return _i2c->write_buffer(_temp_buff, l + 1);
}

bool OLED128x64::addrMode(uint8_t v)
{
  if (v >= 3)
    return false;

  cmd(0x20);
  cmd(v);

  return true;
}

bool OLED128x64::colAddr(uint8_t s, uint8_t e)
{
  if (s > e)
    return false;
  if (s > 127 || e > 127)
    return false;

  cmd(0x21);
  cmd(s);
  cmd(e);

  return true;
}

bool OLED128x64::pageAddr(uint8_t s, uint8_t e)
{
  if (s > e)
    return false;
  if (s > 7 || e > 7)
    return false;

  cmd(0x22);
  cmd(s);
  cmd(e);

  return true;
}

void OLED128x64::clear()
{
  addrMode(0);
  colAddr(0, 127);
  pageAddr(0, 7);

  memset(_temp_buff, 0, 1025); // 1025 = 1(cmd/data) + 1024(buffer)
  _temp_buff[0] = 0x40;        // data for SSD1306
  _i2c->write_buffer(_temp_buff, 1025);
}

void OLED128x64::test(void)
{
  // TODO
}

void OLED128x64::init_ssd1306(void)
{
  // https://github.com/iliapenev/ssd1306_i2c/blob/master/ssd1306_i2c.c

  cmd(0xAE); // SSD1306_DISPLAYOFF

  cmd(0xD5); // SSD1306_SETDISPLAYCLOCKDIV
  cmd(0x80);
  cmd(0xA8); // SSD1306_SETMULTIPLEX, 64-1
  cmd(0x3F);
  cmd(0xD3); // SSD1306_SETDISPLAYOFFSET
  cmd(0x00);

  cmd(0x40 | 0x00); // SSD1306_SETSTARTLINE + line# 0

  cmd(0x20); // SSD1306_MEMORYMODE
  cmd(0x00);

  cmd(0xA0 | 0x01); // SSD1306_SEGREMAP
  cmd(0xC8);        // SSD1306_COMSCANDEC

  cmd(0xDA); // SSD1306_SETCOMPINS
  cmd(0x12); // SSD1306_SETCOMPINS

  cmd(0xA4); // SSD1306_DISPLAYALLON_RESUME
  cmd(0xA6); // SSD1306_NORMALDISPLAY <-> 0xA7(inverse)
  cmd(0x2E); // SSD1306_DEACTIVATE_SCROLL

  cmd(0x20); // SSD1306_SETCOMPINS
  cmd(0x02);
  // b00: Horz addressing mode
  // b01: vert addressing mode
  // b10: page addressing mode
  // b11: invalid

  cmd(0x81); // SSD1306_SETCONTRAST, internal vcc
  cmd(0xCF);
  cmd(0xD9); // SSD1306_SETPRECHARGE, internal vcc
  cmd(0xF1);
  cmd(0xDB); // SSD1306_SETVCOMDETECT, 0x00, 0x20(reset), 0x30
  cmd(0x20);
  cmd(0xD5); // Set OSC Freq, 0x80(reset)
  cmd(0x80);

  // chargepump sequence must be 0x8d, 0x14, 0xaf
  cmd(0x8D); // SSD1306_CHARGEPUMP,
  cmd(0x14);
  cmd(0xAF); // SSD1306_DISPLAYON
}

} // namespace rpigpiopp
