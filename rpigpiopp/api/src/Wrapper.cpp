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

#include "Wrapper.h"

#include <cassert>
#include <iostream>

namespace
{

// Temporary unique ID for testing
volatile uint32_t __id = 0;

uint32_t __get_uid(void)
{
  __id++;
  return __id;
}

} // namespace

namespace rpigpiopp
{

void Wrapper::Init(Napi::Env &env, Napi::Object &exports)
{
  Wrapper::InitGpio(env, exports);
  Wrapper::InitI2C(env, exports);

  Wrapper::InitTM1637(env, exports);
  Wrapper::InitPCF8574(env, exports);

  Wrapper::InitLED4x7Seg(env, exports);
}

Wrapper::Wrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Wrapper>(info)
{
  _id = __get_uid();

  std::cout << "Wrapper::Wrapper " << this << ": " << _id << std::endl;
}

} // namespace rpigpiopp
