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

Wrapper::Wrapper()
{
  _id = __get_uid();
  std::cout << "Wrapper::Wrapper " << this << ": " << _id << std::endl;
}

Wrapper::~Wrapper() { std::cout << "Wrapper::~Wrapper " << this << ": " << _id << std::endl; }

void Wrapper::Init(v8::Local<v8::Object> exports)
{
  v8::Isolate *isolate = exports->GetIsolate();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  // Prepare addon data template
  v8::Local<v8::ObjectTemplate> data_tpl = v8::ObjectTemplate::New(isolate);
  data_tpl->SetInternalFieldCount(1); // 1 field for the Wrapper::New()
  v8::Local<v8::Object> data = data_tpl->NewInstance(context).ToLocalChecked();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New, data);
  tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Gpio").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype methods
  // TODO add methods here
  NODE_SET_PROTOTYPE_METHOD(tpl, "init", Init);
  NODE_SET_PROTOTYPE_METHOD(tpl, "release", Release);

  v8::Local<v8::Function> constructor = tpl->GetFunction(context).ToLocalChecked();
  data->SetInternalField(0, constructor);
  exports->Set(context, v8::String::NewFromUtf8(isolate, "Gpio").ToLocalChecked(), constructor)
    .FromJust();
}

void Wrapper::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  if (args.IsConstructCall())
  {
    // Invoked as constructor: `new Gpio(...)`
    Wrapper *obj = new Wrapper();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `Gpio(...)`, turn into construct call.
    v8::Local<v8::Function> cons =
      args.Data().As<v8::Object>()->GetInternalField(0).As<v8::Function>();
    v8::Local<v8::Object> result = cons->NewInstance(context).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void Wrapper::Init(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  Wrapper *obj = node::ObjectWrap::Unwrap<Wrapper>(args.Holder());
  assert(obj != nullptr);

  obj->gpio().init();
}

void Wrapper::Release(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  Wrapper *obj = node::ObjectWrap::Unwrap<Wrapper>(args.Holder());
  assert(obj != nullptr);

  obj->gpio().release();
}

} // namespace rpigpiopp
