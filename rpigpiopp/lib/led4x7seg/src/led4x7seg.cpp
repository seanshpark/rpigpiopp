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

#include "led4x7seg/led4x7seg.h"
#include "tm1637/tm1637_def.h"

#include <iostream>
#include <bitset>
#include <cassert>

namespace
{

static uint8_t _seg_data[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

} // namespace

namespace rpigpiopp
{

bool LED4x7Seg::init(TM1637 *tm1637)
{
  _tm1637 = tm1637;

  return true;
}

void LED4x7Seg::release(void)
{
  _tm1637 = nullptr;
  // TODO more cleanup if needed
}

void LED4x7Seg::show(const std::string value)
{
  if (_tm1637 == nullptr)
  {
    assert(false);
    return;
  }

  uint32_t val_length = value.length();
  assert(val_length == 5);
  // value should be format of "xx:xx" or "xx xx"
  // where x is '0' ~ '9'

  uint8_t tm1637_data[5];
  uint32_t data_idx = 0;
  bool colon = false;
  const uint32_t col_idx = 2;

  tm1637_data[data_idx++] = TM1637_CMD_ADDR | TM1637_ADDR_C0H;

  for (uint32_t i = 0; i < val_length; ++i)
  {
    auto c = value.at(i);
    if (i == col_idx)
    {
      colon = (c == ':');
    }
    else
    {
      auto cidx = static_cast<int32_t>(c - '0');
      if (cidx < 0 || cidx > 9)
        cidx = 0;

      auto cdata = _seg_data[cidx];
      tm1637_data[data_idx++] = cdata;
    }
  }
  if (colon)
  {
    tm1637_data[col_idx] |= TM1637_DBIT_COLON;
  }
  _tm1637->writes(tm1637_data, 5);

  // std::cout << "led4x7seg show " << value << std::endl;
}

void LED4x7Seg::bright(uint8_t value)
{
  if (_tm1637 == nullptr)
  {
    assert(false);
    return;
  }

  _tm1637->bright(value);
}

void LED4x7Seg::clear(void)
{
  if (_tm1637 == nullptr)
  {
    assert(false);
    return;
  }

  _tm1637->clear();
}

} // namespace rpigpiopp
