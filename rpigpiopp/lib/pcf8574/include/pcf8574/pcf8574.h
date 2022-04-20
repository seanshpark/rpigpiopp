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

#ifndef __RPIGPIOPP_LIB_PCF8574_H__
#define __RPIGPIOPP_LIB_PCF8574_H__

#include "i2c/i2c.h"

namespace rpigpiopp
{

class PCF8574
{
public:
  PCF8574() = default;
  virtual ~PCF8574() = default;

public:
  bool init(I2C *i2c);
  void release(void);

  void write(uint8_t data);

public:
  bool initialized(void) { return _initalized; }

private:
  I2C *_i2c = nullptr;
  bool _initalized = false;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_PCF8574_H__
