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

#ifndef __RPIGPIOPP_LIB_GPIO_H__
#define __RPIGPIOPP_LIB_GPIO_H__

#include <sys/mman.h>
#include <cstdint>

namespace rpigpiopp
{

class Gpio
{
public:
  enum PIN
  {
    IN = 1,
    OUT = 2,
  };

public:
  Gpio();
  virtual ~Gpio();

public:
  bool init(void);
  void release(void);

private:
  int _mem_fd = -1;
  void *_gpio_map = MAP_FAILED;
  size_t _gpio_size = 0;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_GPIO_H__