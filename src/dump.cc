#include "dump.h"

Napi::Value NodeJQ::Dump(const Napi::Env &env, const jv &source) {
  switch (jv_get_kind(source)) {
  case JV_KIND_NULL:
    return env.Null();

  case JV_KIND_STRING:
    return DumpString(env, source);

  case JV_KIND_NUMBER:
    return DumpNumber(env, source);

  case JV_KIND_ARRAY:
    return DumpArray(env, source);

  case JV_KIND_OBJECT:
    return DumpObject(env, source);

  case JV_KIND_TRUE:
    return Napi::Boolean::New(env, true);

  case JV_KIND_FALSE:
    return Napi::Boolean::New(env, false);

  default:
    throw Napi::Error::New(env, "invalid output type");
  }
}

Napi::String NodeJQ::DumpString(const Napi::Env &env, const jv &source) {
  return Napi::String::New(env, jv_string_value(source));
}

Napi::Number NodeJQ::DumpNumber(const Napi::Env &env, const jv &source) {
  return Napi::Number::New(env, jv_number_value(source));
}

Napi::Array NodeJQ::DumpArray(const Napi::Env &env, const jv &source) {
  auto target = Napi::Array::New(env);
  auto push = target.Get("push").As<Napi::Function>();

  jv_array_foreach(source, index, value) {
    push.Call(target, {Dump(env, value)});
  }

  return target;
}

Napi::Object NodeJQ::DumpObject(const Napi::Env &env, const jv &source) {
  auto target = Napi::Object::New(env);

  jv_object_foreach(source, key, value) {
    target.Set(Dump(env, key), Dump(env, value));
  }

  return target;
}
