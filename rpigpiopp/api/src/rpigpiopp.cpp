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

#include <iostream>

void init(const v8::FunctionCallbackInfo<v8::Value>&)
{
  // TODO do init
}

void release(const v8::FunctionCallbackInfo<v8::Value>&)
{
  // TODO do release
}

void hello(const v8::FunctionCallbackInfo<v8::Value>&)
{
  rpigpiopp::hello();
  rpigpiopp::world();
}

extern "C" NODE_MODULE_EXPORT
void Initialize(v8::Local<v8::Object> exports, v8::Local<v8::Value>, void *)
{
  rpigpiopp::Wrapper::Init(exports);

   NODE_SET_METHOD(exports, "init", init);
   NODE_SET_METHOD(exports, "release", release);
   NODE_SET_METHOD(exports, "hello", hello);
}

NODE_MODULE(rpigpiopp, Initialize);
