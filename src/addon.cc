#include "calculate.h"
#include <napi.h>

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "calculateSync"),
              Napi::Function::New(env, CaculateSync));
  return exports;
}

NODE_API_MODULE(libjq, Init)
