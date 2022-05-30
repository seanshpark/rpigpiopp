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

void Wrapper::InitSPI(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function funcSPI = DefineClass(env, "SPI",
    {
      InstanceMethod("init", &Wrapper::API_SPI_init),
      InstanceMethod("release", &Wrapper::API_SPI_release),
      InstanceMethod("write", &Wrapper::API_SPI_write)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(funcSPI);
  env.SetInstanceData(constructor);

  // SPI
  exports.Set("SPI", funcSPI);
}

Napi::Value Wrapper::API_SPI_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 3)
    Napi::Error::New(env, "Requre 3 arguments(gpio, chip, bus)").ThrowAsJavaScriptException();

  auto gpioObj = info[0].As<Napi::Object>();
  Wrapper *wrapper = Napi::ObjectWrap<Wrapper>::Unwrap(gpioObj);
  std::cout << "!!! init SPI with GPIO " << wrapper << std::endl;

  _parent = wrapper;
  if (!_parent->gpio().initialized())
    Napi::Error::New(env, "GPIO not initialized").ThrowAsJavaScriptException();
  _gpio_base = &_parent->gpio();

  auto chip = info[1].As<Napi::Number>();
  auto bus = info[2].As<Napi::Number>();

  if (!this->spi().init(&this->gpio(), chip.Int32Value(), bus.Int32Value()))
    Napi::Error::New(env, "SPI init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_SPI_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->spi().release();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_SPI_write(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  if (info[0].IsArrayBuffer())
  {
    auto data = info[0].As<Napi::ArrayBuffer>();
    auto buffer = reinterpret_cast<uint8_t *>(data.Data());
    if (!this->spi().write_buffer(buffer, data.ByteLength()))
    {
      Napi::Error::New(env, "Failed to write SPI").ThrowAsJavaScriptException();
    }
  }
  else if (info[0].IsNumber())
  {
    auto data = info[0].As<Napi::Number>();
    if (!this->spi().write_byte(data.Int32Value() & 0xff))
    {
      Napi::Error::New(env, "Failed to write SPI").ThrowAsJavaScriptException();
    }
  }
  else if (info[0].IsTypedArray())
  {
    // const data = new Uint8Array([1,2,...]);
    // spi.write(data);
    if (info.Length() != 1 && info.Length() != 3)
    {
      Napi::Error::New(env, "Requre 1 or 3 arguments").ThrowAsJavaScriptException();
    }
    auto typeddata = info[0].As<Napi::TypedArray>();
    auto data = typeddata.ArrayBuffer();
    auto buffer = reinterpret_cast<uint8_t *>(data.Data());

    if (info.Length() == 1)
    {
      // std::cout << "SPI data typed array " << data.ByteLength() << std::endl;
      if (!this->spi().write_buffer(buffer, data.ByteLength()))
      {
        Napi::Error::New(env, "Failed to SPI send data").ThrowAsJavaScriptException();
      }
    }
    else if (info.Length() == 3)
    {
      auto start = info[1].As<Napi::Number>();
      auto length = info[2].As<Napi::Number>();

      // std::cout << "SPI data typed array " << data.ByteLength() << ": "
      //          << start.Uint32Value() << ", " << length.Uint32Value() << std::endl;
      if (start.Uint32Value() + length.Uint32Value() >= data.ByteLength())
      {
        Napi::Error::New(env, "SPI write buffer overflow").ThrowAsJavaScriptException();
      }
      if (!this->spi().write_buffer(buffer + start.Uint32Value(), length.Uint32Value()))
      {
        Napi::Error::New(env, "Failed to SPI send data").ThrowAsJavaScriptException();
      }
    }
  }
  else
  {
    Napi::Error::New(env, "Argument shoud be Buffer or Number").ThrowAsJavaScriptException();
  }

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
