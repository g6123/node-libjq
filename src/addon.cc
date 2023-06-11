#include "run.h"
#include <napi.h>

using namespace NodeJQ;

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "run"), Napi::Function::New(env, Run));
  return exports;
}

NODE_API_MODULE(NodeJQ, Init)
