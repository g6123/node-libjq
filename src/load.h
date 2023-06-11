#pragma once
#include <napi.h>

extern "C" {
#include <jv.h>
}

namespace NodeJQ {
jv Load(const Napi::Value &source);
jv LoadString(const Napi::String &source);
jv LoadNumber(const Napi::Number &source);
jv LoadBoolean(const Napi::Boolean &source);
jv LoadArray(const Napi::Array &source);
jv LoadObject(const Napi::Object &source);
} // namespace NodeJQ
