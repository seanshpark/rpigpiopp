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

#include <Gpio/Gpio.h>

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
  // clang-format off
  Napi::Function funcGpio = DefineClass(env, "Gpio",
    {
      InstanceMethod("init", &Wrapper::API_Gpio_init),
      InstanceMethod("release", &Wrapper::API_Gpio_release),
      InstanceMethod("pin", &Wrapper::API_Gpio_pin),
      InstanceMethod("set", &Wrapper::API_Gpio_set)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(funcGpio);
  env.SetInstanceData(constructor);

  // GPIO
  exports.Set("Gpio", funcGpio);

  Napi::Object obj_def = Napi::Object::New(env);
  obj_def.Set(Napi::String::New(env, "IN"), Napi::Number::New(env, Gpio::PIN::IN));
  obj_def.Set(Napi::String::New(env, "OUT"), Napi::Number::New(env, Gpio::PIN::OUT));

  // DEF for GPIO
  exports.Set("DEF", obj_def);

  // TM1637 chip
  // clang-format off
  Napi::Function funcTM1637 = DefineClass(env, "TM1637",
    {
      InstanceMethod("init", &Wrapper::API_TM1637_init),
      InstanceMethod("release", &Wrapper::API_TM1637_release),
      InstanceMethod("write", &Wrapper::API_TM1637_write),
      InstanceMethod("writes", &Wrapper::API_TM1637_writes),
      InstanceMethod("bright", &Wrapper::API_TM1637_bright),
      InstanceMethod("test", &Wrapper::API_TM1637_test)
    }
  );
  // clang-format on

  Napi::FunctionReference *ctorTM3637 = new Napi::FunctionReference();
  *ctorTM3637 = Napi::Persistent(funcTM1637);
  env.SetInstanceData(ctorTM3637);

  exports.Set("TM1637", funcTM1637);
}

Wrapper::Wrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Wrapper>(info)
{
  _id = __get_uid();

  std::cout << "Wrapper::Wrapper " << this << ": " << _id << std::endl;
}

Napi::Value Wrapper::API_Gpio_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (!this->gpio().init())
    Napi::Error::New(env, "gpio init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_Gpio_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->gpio().release();

  return Napi::Number::New(env, 0);
}

// pin(number, attributes): Set pin attributes
//  number: port number
//  attributes: bitwise values
//    direction: IN or OUT
Napi::Value Wrapper::API_Gpio_pin(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 arguments(port, value)").ThrowAsJavaScriptException();
  }

  auto port = info[0].As<Napi::Number>();
  auto value = info[1].As<Napi::Number>();

  std::cout << "!!! pin: " << port.Int32Value() << ": " << value.Int32Value() << std::endl;
  this->gpio().cfg(port.Int32Value(), static_cast<Gpio::PIN>(value.Int32Value()));

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_Gpio_set(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 arguments(port, value)").ThrowAsJavaScriptException();
  }

  auto port = info[0].As<Napi::Number>();
  auto value = info[1].As<Napi::Boolean>();

  std::cout << "!!! set: " << port.Int32Value() << ": " << (value.Value() ? "T" : "F") << std::endl;
  this->gpio().set(port.Int32Value(), value.Value());

  return Napi::Number::New(env, 0);
}

// TM1637
Napi::Value Wrapper::API_TM1637_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(gpio, cmd, dio)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  Wrapper *wrapper = Napi::ObjectWrap<Wrapper>::Unwrap(gpioObj);
  std::cout << "!!! init with gpio " << wrapper << std::endl;

  _parent = wrapper;
  if (!_parent->gpio().initialized())
    Napi::Error::New(env, "gpio not initialized").ThrowAsJavaScriptException();
  _gpio_base = &_parent->gpio();

  auto cmd = info[1].As<Napi::Number>();
  auto dio = info[2].As<Napi::Number>();

  if (!this->tm1637().init(&this->gpio(), cmd.Int32Value(), dio.Int32Value()))
    Napi::Error::New(env, "TM1637 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_TM1637_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->tm1637().release();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_TM1637_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->tm1637().write(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_TM1637_writes(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  uint32_t length = info.Length();
  if (length != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(Buffer)").ThrowAsJavaScriptException();
  }

  Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();

  this->tm1637().writes(buffer.Data(), buffer.Length());

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_TM1637_bright(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::Number>();

  this->tm1637().bright(data.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_TM1637_test(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->tm1637().test();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
