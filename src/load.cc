#include "load.h"

jv NodeJQ::Load(const Napi::Value &source) {
  if (source.IsNull()) {
    return jv_null();
  }

  if (source.IsString()) {
    return LoadString(source.As<Napi::String>());
  }

  if (source.IsNumber()) {
    return LoadNumber(source.As<Napi::Number>());
  }

  if (source.IsBoolean()) {
    return LoadBoolean(source.As<Napi::Boolean>());
  }

  if (source.IsArray()) {
    return LoadArray(source.As<Napi::Array>());
  }

  if (source.IsObject()) {
    auto sourceObject = source.As<Napi::Object>();

    if (Napi::Value toJSON = sourceObject.Get("toJSON"); toJSON.IsFunction()) {
      return Load(toJSON.As<Napi::Function>().Call(source, {}));
    } else {
      return LoadObject(sourceObject);
    }
  }

  throw Napi::Error::New(source.Env(), "unknown input type");
}

jv NodeJQ::LoadString(const Napi::String &source) {
  return jv_string(std::string(source).c_str());
}

jv NodeJQ::LoadNumber(const Napi::Number &source) { return jv_number(source); }

jv NodeJQ::LoadBoolean(const Napi::Boolean &source) { return jv_bool(source); }

jv NodeJQ::LoadArray(const Napi::Array &source) {
  jv target = jv_array_sized(source.Length());

  for (auto [key, lvalue] : source) {
    int index = key.ToNumber().Int64Value();
    auto value = static_cast<Napi::Value>(lvalue);
    jv_array_set(target, index, value.IsUndefined() ? jv_null() : Load(value));
  }

  return target;
}

jv NodeJQ::LoadObject(const Napi::Object &source) {
  jv target = jv_object();

  for (auto [key, lvalue] : source) {
    auto value = static_cast<Napi::Value>(lvalue);

    if (value.IsUndefined()) {
      continue;
    }

    jv_object_set(target, Load(key), Load(value));
  }

  return target;
}
