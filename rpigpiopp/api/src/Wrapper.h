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

namespace rpigpiopp
{

class Wrapper : public node::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

private:
  static void API_init(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void API_release(const v8::FunctionCallbackInfo<v8::Value> &args);

  static void API_pin(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void API_set(const v8::FunctionCallbackInfo<v8::Value> &args);

public:
  uint32_t id() { return _id; }
  Gpio &gpio() { return _gpio; }

private:
  explicit Wrapper();
  ~Wrapper();

  uint32_t _id = 0;
  Gpio _gpio;
};

} // namespace rpigpiopp

#endif // __RPIGPIOPP_API_WRAPPER_H__
