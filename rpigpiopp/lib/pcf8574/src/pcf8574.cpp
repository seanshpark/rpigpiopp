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

#include "pcf8574/pcf8574.h"

#include <iostream>
#include <bitset>
#include <cassert>

#include <unistd.h> // usleep

namespace rpigpiopp
{

bool PCF8574::init(I2C *i2c)
{
  std::cout << "PCF8574::init" << std::endl;

  _i2c = i2c;
  _initalized = true;

  return true;
}

void PCF8574::release(void)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }

  _initalized = false;
  _i2c = nullptr;

  std::cout << "PCF8574::release" << std::endl;
}

void PCF8574::write(uint8_t data)
{
  if (_i2c == nullptr)
  {
    assert(false);
    return;
  }
  // std::cout << "pcf8574 write " << std::bitset<8>(data) << std::endl;
  _i2c->write_byte(data);
}

} // namespace rpigpiopp

//
// PCF1637 privates
//
namespace rpigpiopp
{

// TODO

} // namespace rpigpiopp
