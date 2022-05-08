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

#include "gpio/gpio.h"
#include "gpio/AddrInfo.h"

#include <iostream>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace rpigpiopp
{

bool GPIO::init(void)
{
  std::cout << "GPIO::init" << std::endl;

  _mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
  if (_mem_fd < 0)
  {
    std::cerr << "fail open /dev/gpiomem" << std::endl;
    return false;
  }
  // TODO autodetect device
  AddrRPi3 rpi3;
  uint32_t gpio_base = rpi3.base() + rpi3.gpio();

  _gpio_size = rpi3.size();
  _gpio_map = mmap(NULL, _gpio_size, PROT_READ | PROT_WRITE, MAP_SHARED, _mem_fd, gpio_base);
  if (_gpio_map == MAP_FAILED)
  {
    std::cerr << "fail mmap" << std::endl;
    return false;
  }

  return true;
}

void GPIO::release(void)
{
  if (_gpio_map != MAP_FAILED)
  {
    munmap(_gpio_map, _gpio_size);
  }
  if (_mem_fd > 0)
  {
    close(_mem_fd);
  }

  _gpio_map = MAP_FAILED;
  _gpio_size = 0;
  _mem_fd = 0;

  std::cout << "GPIO::release" << std::endl;
}

void GPIO::cfg(int32_t pin, PIN dir)
{
  const uint32_t FSEL_MASK = 0x07; // set as input with mask to clear bit
  const uint32_t FSEL_ALT0 = 0x04; // alternative function 0
  const uint32_t FSEL_SOUT = 0x01; // set as output
  uint32_t offset = pin / 10;
  uint32_t shift = (pin % 10) * 3;

  switch (dir)
  {
    case PIN::IN:
      *(addr(offset)) &= ~(FSEL_MASK << shift);
      break;

    case PIN::OUT:
      *(addr(offset)) |= (FSEL_SOUT << shift);
      break;

    case PIN::ALT0:
      *(addr(offset)) |= (FSEL_ALT0 << shift);
      break;
  }
}

void GPIO::set(int32_t pin, bool val)
{
  // TODO move to header
  const uint32_t GPSET0 = 0x001c;
  const uint32_t GPCLR0 = 0x0028;

  uint32_t setclr = val ? GPSET0 : GPCLR0;
  uint32_t offset = setclr / 4 + pin / 32;
  uint32_t shift = pin % 32;

  *(addr(offset)) = 1 << shift;
}

} // namespace rpigpiopp
