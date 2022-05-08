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

#include "i2c/i2c.h"

#include <bitset>
#include <iostream>
#include <cassert>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

namespace rpigpiopp
{

// gpio is not used for now.
bool I2C::init(GPIO *gpio, int32_t dev, int32_t addr)
{
  std::cout << "I2C::init " << dev << ": 0x" << std::hex << addr << std::endl;

  // support i2c-1 ~ i2c-9
  if (dev < 1 || dev > 9)
    return false;

  char devfname[128];
  sprintf(devfname, "/dev/i2c-%d", dev);
  if ((_dev_fd = open(devfname, O_RDWR)) < 0)
  {
    std::cout << "failed to open " << devfname << std::endl;
    return false;
  }

  if (ioctl(_dev_fd, I2C_SLAVE, addr) < 0)
  {
    close(_dev_fd);
    _dev_fd = -1;

    std::cout << "failed to set i2d addr " << addr << std::endl;
    return false;
  }

  _gpio = gpio;
  _dev = dev;
  _addr = addr;

  return true;
}

void I2C::release(void)
{
  if (_dev_fd > 0)
  {
    close(_dev_fd);
    _dev_fd = -1;
  }
  _dev = 0;
  _addr = 0;
  _gpio = nullptr;

  std::cout << "I2C::release" << std::endl;
}

bool I2C::write_byte(uint8_t b)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  std::cout << "I2C " << std::hex << int32_t(b) << std::endl;

  if (::write(_dev_fd, &b, 1) != 1)
  {
    return false;
  }
  return true;
}

bool I2C::write_buffer(uint8_t *b, size_t s)
{
  assert(_dev_fd > 0);

  if (_dev_fd <= 0)
    return false;

  // std::cout << "I2C write_buffer" << s << std::endl;

  if ((size_t)::write(_dev_fd, b, s) != s)
  {
    return false;
  }
  return true;
}

} // namespace rpigpiopp
