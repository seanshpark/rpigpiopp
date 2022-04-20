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

#include <lcd1602/lcd1602.h>

#include <cassert>
#include <iostream>

namespace rpigpiopp
{

void Wrapper::InitLCD1602(Napi::Env &env, Napi::Object &exports)
{
  // clang-format off
  Napi::Function functions = DefineClass(env, "LCD1602",
    {
      InstanceMethod("init", &Wrapper::API_LCD1602_init),
      InstanceMethod("release", &Wrapper::API_LCD1602_release),

      InstanceMethod("clear", &Wrapper::API_LCD1602_clear),
      InstanceMethod("home", &Wrapper::API_LCD1602_home),
      InstanceMethod("display", &Wrapper::API_LCD1602_display),
      InstanceMethod("cursor", &Wrapper::API_LCD1602_cursor),
      InstanceMethod("blink", &Wrapper::API_LCD1602_blink),
      InstanceMethod("puts", &Wrapper::API_LCD1602_puts),
      InstanceMethod("move", &Wrapper::API_LCD1602_move),

      InstanceMethod("test", &Wrapper::API_LCD1602_test)
    }
  );
  // clang-format on

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(functions);
  env.SetInstanceData(constructor);

  // LCD1602
  exports.Set("LCD1602", functions);
}

Napi::Value Wrapper::API_LCD1602_init(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
    Napi::Error::New(env, "Requre 1 arguments(pcf8574)").ThrowAsJavaScriptException();

  auto pcf8574Obj = info[0].As<Napi::Object>();
  Wrapper *wrapper = Napi::ObjectWrap<Wrapper>::Unwrap(pcf8574Obj);
  std::cout << "!!! init lcd1602 with pcf8574 " << wrapper << std::endl;

  _parent = wrapper;
  if (!_parent->pcf8574().initialized())
    Napi::Error::New(env, "lcd1602: pcf8574 not initialized").ThrowAsJavaScriptException();
  _gpio_base = &_parent->gpio();
  _i2c_base = &_parent->i2c();
  _pcf8574_base = &_parent->pcf8574();

  if (!this->lcd1602().init(&this->pcf8574()))
    Napi::Error::New(env, "lcd1602 init failed").ThrowAsJavaScriptException();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_release(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().release();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_clear(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().clear();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_home(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().home();

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_display(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  std::cout << "display " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().display(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::Wrapper::API_LCD1602_cursor(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  std::cout << "cursor " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().cursor(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_blink(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 arguments(enable)").ThrowAsJavaScriptException();
  }

  auto enable = info[0].As<Napi::Boolean>();
  std::cout << "blink " << (enable.Value() ? "T" : "F") << std::endl;
  this->lcd1602().blink(enable.Value());

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_puts(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1)
  {
    Napi::Error::New(env, "Requre 1 argument(data)").ThrowAsJavaScriptException();
  }

  auto data = info[0].As<Napi::String>();

  std::string value = std::string(data);
  this->lcd1602().puts(value.c_str());

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_move(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::Error::New(env, "Requre 2 argument(row, col)").ThrowAsJavaScriptException();
  }

  auto row = info[0].As<Napi::Number>();
  auto col = info[1].As<Napi::Number>();

  this->lcd1602().move(row.Int32Value() & 0xff, col.Int32Value() & 0xff);

  return Napi::Number::New(env, 0);
}

Napi::Value Wrapper::API_LCD1602_test(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  this->lcd1602().test();

  return Napi::Number::New(env, 0);
}

} // namespace rpigpiopp
