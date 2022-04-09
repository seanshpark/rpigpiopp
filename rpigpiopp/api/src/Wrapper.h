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

#ifndef __RPIGPIOPP_API_WRAPPER_H__
#define __RPIGPIOPP_API_WRAPPER_H__

#include "node_wrap.h"

#include <Gpio/Gpio.h>
#include <tm1637/tm1637.h>

namespace rpigpiopp
{

class Wrapper : public Napi::ObjectWrap<Wrapper>
{
public:
  Wrapper(const Napi::CallbackInfo &info);

public:
  static void Init(Napi::Env &env, Napi::Object &exports);

private:
  static void InitGpio(Napi::Env &env, Napi::Object &exports);
  static void InitTM1637(Napi::Env &env, Napi::Object &exports);

private:
  Napi::Value API_Gpio_init(const Napi::CallbackInfo &info);
  Napi::Value API_Gpio_release(const Napi::CallbackInfo &info);
  Napi::Value API_Gpio_pin(const Napi::CallbackInfo &info);
  Napi::Value API_Gpio_set(const Napi::CallbackInfo &info);

  //
  Napi::Value API_TM1637_init(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_release(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_write(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_writes(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_bright(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_clear(const Napi::CallbackInfo &info);
  Napi::Value API_TM1637_test(const Napi::CallbackInfo &info);

public:
  uint32_t id() { return _id; }
  Gpio &gpio() { return _gpio_base == nullptr ? _gpio : *_gpio_base; }
  TM1637 &tm1637() { return _tm1637; }

private:
  uint32_t _id = 0;
  Gpio _gpio;
  TM1637 _tm1637;

private:
  Wrapper *_parent = nullptr;
  Gpio *_gpio_base = nullptr;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_API_WRAPPER_H__
