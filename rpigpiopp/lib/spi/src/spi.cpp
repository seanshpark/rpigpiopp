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

#include "spi/spi.h"

#include <bitset>
#include <iostream>
#include <cassert>
#include <cstring> // memset

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

namespace rpigpiopp
{

// gpio is not used for now.
bool SPI::init(GPIO *gpio, int32_t chip, int32_t bus)
{
  std::cout << "SPI::init " << chip << "." << bus << std::endl;

  char devfname[128];
  sprintf(devfname, "/dev/spidev%d.%d", chip, bus);
  if ((_dev_fd = open(devfname, O_RDWR)) < 0)
  {
    std::cout << "failed to open " << devfname << std::endl;
    return false;
  }

  // check rw mode
  int ret = ioctl(_dev_fd, SPI_IOC_WR_MODE, &_mode);
  if (ret == -1)
  {
    std::cout << "failed to set SPI WR mode " << devfname << std::endl;
    return false;
  }
  ret = ioctl(_dev_fd, SPI_IOC_RD_MODE, &_mode);
  if (ret == -1)
  {
    std::cout << "failed to set SPI RD mode " << devfname << std::endl;
    return false;
  }

  // bits per word
  ret = ioctl(_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &_bits);
  if (ret == -1)
  {
    std::cout << "failed to set SPI RW " << (int32_t)_bits << " bits " << devfname << std::endl;
    return false;
  }
  ret = ioctl(_dev_fd, SPI_IOC_RD_BITS_PER_WORD, &_bits);
  if (ret == -1)
  {
    std::cout << "failed to set SPI RD " << (int32_t)_bits << " bits " << devfname << std::endl;
    return false;
  }

  // max speed hz
  ret = ioctl(_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &_speed);
  if (ret == -1)
  {
    std::cout << "failed to set SPI RW max speed " << _speed << " " << devfname << std::endl;
    return false;
  }

  ret = ioctl(_dev_fd, SPI_IOC_RD_MAX_SPEED_HZ, &_speed);
  if (ret == -1)
  {
    std::cout << "failed to set SPI RD max speed " << _speed << " " << devfname << std::endl;
    return false;
  }

  // set to WRITE mode
  ret = ioctl(_dev_fd, SPI_IOC_WR_MODE, &_mode);
  if (ret == -1)
  {
    std::cout << "failed to set SPI WR mode " << devfname << std::endl;
    return false;
  }

  std::cout << "spi: bits(" << (uint32_t)_bits << "), speed(" << _speed << ")" << std::endl;

  _initialized = true;
  _gpio = gpio;
  _chip = chip;
  _bus = bus;

  return true;
}

void SPI::release(void)
{
  _initialized = false;
  if (_dev_fd > 0)
  {
    close(_dev_fd);
    _dev_fd = -1;
  }

  _bus = 0;
  _chip = 0;
  _gpio = nullptr;

  std::cout << "SPI::release" << std::endl;
}

bool SPI::write_byte(uint8_t b)
{
  assert(_dev_fd > 0);
  if (_dev_fd <= 0)
    return false;

  std::cout << "SPI " << std::hex << int32_t(b) << std::endl;

  uint8_t tx[1];
  spi_ioc_transfer tr;

  std::memset(&tr, 0, sizeof(spi_ioc_transfer));
  tr.tx_buf = (unsigned long)tx;
  tr.rx_buf = 0;
  tr.len = 1;
  tr.speed_hz = _speed;
  tr.delay_usecs = 10; // TODO set ?
  tr.bits_per_word = _bits;

  int ret = ioctl(_dev_fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
  {
    std::cout << "SPI::write_byte failed " << ret << std::endl;
    return false;
  }
  return true;
}

bool SPI::write_buffer(uint8_t *b, size_t s)
{
  assert(_dev_fd > 0);
  if (_dev_fd <= 0)
    return false;

  spi_ioc_transfer tr;

  std::memset(&tr, 0, sizeof(spi_ioc_transfer));
  tr.tx_buf = (unsigned long)b;
  tr.rx_buf = 0;
  tr.len = s;
  tr.speed_hz = _speed;
  tr.delay_usecs = 10; // TODO set ?
  tr.bits_per_word = _bits;

  int ret = ioctl(_dev_fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
  {
    std::cout << "SPI::write_buffer failed " << ret << std::endl;
    return false;
  }
  return true;
}

} // namespace rpigpiopp
