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

#ifndef __RPIGPIOPP_LIB_SPI_H__
#define __RPIGPIOPP_LIB_SPI_H__

#include "gpio/gpio.h"

namespace rpigpiopp
{

class SPI
{
public:
  SPI() = default;
  virtual ~SPI() = default;

public:
  bool init(GPIO *gpio, int32_t chip, int32_t bus);
  void release(void);

public:
  bool initialized(void) { return _initialized; }

public:
  bool write_byte(uint8_t b);
  bool write_buffer(uint8_t *b, size_t s);

private:
  GPIO *_gpio = nullptr; // not used for now
  int _dev_fd = -1;
  int32_t _chip = 0; // /dev/spidevC.B where C is for chip, B for bus
  int32_t _bus = 0;
  uint8_t _mode = 0;
  uint8_t _bits = 8;
  uint32_t _speed = 500000;
  bool _initialized = false;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_SPI_H__
