#pragma once
#include <napi.h>

extern "C" {
#include <jv.h>
}

namespace NodeJQ {
Napi::Value Dump(const Napi::Env &env, const jv &source);
Napi::String DumpString(const Napi::Env &env, const jv &source);
Napi::Number DumpNumber(const Napi::Env &env, const jv &source);
Napi::Array DumpArray(const Napi::Env &env, const jv &source);
Napi::Object DumpObject(const Napi::Env &env, const jv &source);
} // namespace NodeJQ
