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

namespace rpigpiopp
{

void Wrapper::InitGpio(Napi::Env &env, Napi::Object &exports)
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

} // namespace rpigpiopp