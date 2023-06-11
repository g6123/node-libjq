#include "run.h"
#include <iostream>

Napi::Value Run(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  std::string program = info[0].As<Napi::String>();
  std::string input = info[1].As<Napi::String>();
  std::string output;

  jq_state *state = jq_init();
  jq_set_error_cb(state, HandleError, nullptr);

  int ret = jq_compile(state, program.c_str());
  if (!ret) {
    throw Napi::Error::New(env, "invalid program");
  }

  jv_parser *parser = jv_parser_new(0);
  jv_parser_set_buf(parser, input.c_str(), input.size(), 1);

  jv value = jv_parser_next(parser);
  if (!jv_is_valid(value)) {
    throw Napi::Error::New(env, "invalid input");
  }

  jq_start(state, value, 0);

  while (jv_is_valid(value = jq_next(state))) {
    jv dump = jv_dump_string(jv_copy(value), 0);
    output.append(jv_string_value(dump));
  }

  return Napi::String::New(env, output);
}

void HandleError(void *ctx, jv value) {}
