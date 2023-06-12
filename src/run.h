#pragma once

#include <list>
#include <napi.h>
#include <string>

extern "C" {
#include <jq.h>
}

namespace NodeJQ {
Napi::Value Run(const Napi::CallbackInfo &info);

void HandleError(void *ctx, jv value);

class RunContext {
public:
  RunContext(const Napi::Env &env);
  void HandleError(jv message);
  Napi::Error NewError(const std::string &message);

private:
  const Napi::Env &env;
  std::list<std::string> messages;
};
} // namespace NodeJQ
