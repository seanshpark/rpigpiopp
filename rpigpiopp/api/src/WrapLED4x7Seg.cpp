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

namespace rpigpiopp
{

void Wrapper::InitLED4x7Seg(Napi::Env &env, Napi::Object &exports)
{
  // LED4x7Seg module
  // clang-format off
  Napi::Function funcLED4x7Seg = DefineClass(env, "LED4x7Seg",
    {
      InstanceMethod("init", &Wrapper::API_LED4x7Seg_init),
      InstanceMethod("release", &Wrapper::API_LED4x7Seg_release),
      InstanceMethod("show", &Wrapper::API_LED4x7Seg_show),
      InstanceMethod("bright", &Wrapper::API_LED4x7Seg_bright),
      InstanceMethod("clear", &Wrapper::API_LED4x7Seg_clear)
    }
  );
  // clang-format on

  Napi::FunctionReference *ctorLED4x7Seg = new Napi::FunctionReference();
  *ctorLED4x7Seg = Napi::Persistent(funcLED4x7Seg);
  env.SetInstanceData(ctorLED4x7Seg);

  exports.Set("LED4x7Seg", funcLED4x7Seg);
}

Napi::Value Wrapper::API_LED4x7Seg_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(tm1637)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  Wrapper *wrapper = Napi::ObjectWrap<Wrapper>::Unwrap(gpioObj);
  std::cout << "!!! init led4x7seg with tm1637 " << wrapper << std::endl;

  _parent = wrapper;
  if (!_parent->tm1637().initialized())
    Napi::Error::New(env, "tm1637 not initialized").ThrowAsJavaScriptException();
  _gpio_base = &_parent->gpio();
  _tm1637_base = &_parent->tm1637();

  if (!this->led4x7seg().init(&this->tm1637()))
    Napi::Error::New(env, "LED4x7Seg init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LED4x7Seg_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->led4x7seg().release();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LED4x7Seg_show(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::String>();

  std::string value = std::string(data);
  if (value.length() != 5)
  {
    Napi::Error::New(env, "data length not 5)").ThrowAsJavaScriptException();
  }

  this->led4x7seg().show(value);

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LED4x7Seg_bright(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->led4x7seg().bright(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LED4x7Seg_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->led4x7seg().clear();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
