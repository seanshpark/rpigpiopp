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

#include "node_wrap.h"

#include "Wrapper.h"

#include <base/base.h>

Napi::Value Hello(const Napi::CallbackInfo &info)
{
  rpigpiopp::hello();
  rpigpiopp::world();

  return Napi::Value(); // undefined
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Hello));

  return rpigpiopp::Wrapper::Init(env, exports);
}

NODE_API_MODULE(rpigpiopp, Initialize);

// https://github.com/nodejs/node-addon-api/tree/main/doc
