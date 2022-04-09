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

#ifndef __RPIGPIOPP_LIB_SEG8x4LED_H__
#define __RPIGPIOPP_LIB_SEG8x4LED_H__

#include "tm1637/tm1637.h"

#include <string>

namespace rpigpiopp
{

class LED4x7Seg
{
public:
  LED4x7Seg() = default;
  virtual ~LED4x7Seg() = default;

public:
  bool init(TM1637 *tm1637);
  void release(void);

  void show(const std::string value);
  void bright(uint8_t value);
  void clear(void);

private:
  TM1637 *_tm1637 = nullptr;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_SEG8x4LED_H__
