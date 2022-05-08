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

#include <gpio/gpio.h>

#include <cassert>
#include <iostream>

namespace rpigpiopp
{

void Wrapper::InitI2C(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function funcI2C = DefineClass(env, "I2C",
    {
      InstanceMethod("init", &Wrapper::API_I2C_init),
      InstanceMethod("release", &Wrapper::API_I2C_release),
      InstanceMethod("write", &Wrapper::API_I2C_write)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(funcI2C);
  env.SetInstanceData(constructor);

  // I2C
  exports.Set("I2C", funcI2C);
}

Napi::Value Wrapper::API_I2C_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(gpio, dev, addr)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  Wrapper *wrapper = Napi::ObjectWrap<Wrapper>::Unwrap(gpioObj);
  std::cout << "!!! init i2c with gpio " << wrapper << std::endl;

  _parent = wrapper;
  if (!_parent->gpio().initialized())
    Napi::Error::New(env, "gpio not initialized").ThrowAsJavaScriptException();
  _gpio_base = &_parent->gpio();

  auto dev = info[1].As<Napi::Number>();
  auto addr = info[2].As<Napi::Number>();

  if (!this->i2c().init(&this->gpio(), dev.Int32Value(), addr.Int32Value()))
    Napi::Error::New(env, "i2c init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_I2C_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->i2c().release();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_I2C_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  if (not info[0].IsArrayBuffer())
  {
    Napi::Error::New(env, "Argument shoud be Buffer").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::ArrayBuffer>();
  auto buffer = reinterpret_cast<uint8_t *>(data.Data());
  if (!this->i2c().write_buffer(buffer, data.ByteLength()))
  {
    Napi::Error::New(env, "Failed to write I2C").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
