#include "calculate.h"

Napi::Value CaculateSync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  std::string program = info[0].As<Napi::String>();
  std::string input = info[1].As<Napi::String>();
  std::string output;

  jq_state *state = jq_init();
  jq_compile(state, program.c_str());
  jv_parser *parser = jv_parser_new(0);
  jv_parser_set_buf(parser, input.c_str(), input.size(), 1);
  jv value = jv_parser_next(parser);

  if (!jv_is_valid(value)) {
    return Napi::Number::New(env, -1);
  }

  jq_start(state, value, 0);

  value = jq_next(state);

  while (jv_is_valid(value)) {
    jv dump = jv_dump_string(jv_copy(value), 0);
    output.append(jv_string_value(dump));
    value = jq_next(state);
  }

  return Napi::String::New(env, output);
}
