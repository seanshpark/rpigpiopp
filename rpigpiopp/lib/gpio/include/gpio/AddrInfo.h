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

#ifndef __RPIGPIOPP_LIB_GPIO_ADDRINFO_H__
#define __RPIGPIOPP_LIB_GPIO_ADDRINFO_H__

#include <sys/mman.h>
#include <cstdint>

namespace rpigpiopp
{

struct AddrInfo
{
  // peripheral base
  virtual uint32_t base(void) = 0;
  // gpio base
  virtual uint32_t gpio(void) = 0;
  // gpio size
  virtual uint32_t size(void) = 0;
};

struct AddrRPi3 : public AddrInfo
{
  uint32_t base(void) override { return 0x3F000000; }
  uint32_t gpio(void) override { return 0x00200000; }
  uint32_t size(void) override { return 0x00000100; }
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_LIB_GPIO_ADDRINFO_H__
