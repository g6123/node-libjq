#include "run.h"
#include "dump.h"
#include "load.h"
#include <iterator>
#include <sstream>
#include <string>

Napi::Value NodeJQ::Run(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  RunContext context(env);
  std::string program = info[0].As<Napi::String>();
  Napi::Value input = info[1];

  // initialize jq instance
  jq_state *state = jq_init();

  jq_set_error_cb(state, HandleError, &context);

  // compile program
  int ret = jq_compile(state, program.c_str());

  if (!ret) {
    throw context.NewError("invalid program");
  }

  // load input
  jv value = Load(input);

  if (!jv_is_valid(value)) {
    throw context.NewError("invalid input");
  }

  // process input & dump output
  auto output = Napi::Array::New(env);
  auto push = output.Get("push").As<Napi::Function>();

  jq_start(state, value, 0);

  while (jv_is_valid(value = jq_next(state))) {
    push.Call(output, {Dump(env, value)});
    jv_free(value);
  }

  jq_teardown(&state);
  return output;
}

void NodeJQ::HandleError(void *data, jv msg) {
  static_cast<RunContext *>(data)->HandleError(msg);
}

NodeJQ::RunContext::RunContext(const Napi::Env &env) : env(env) {}

void NodeJQ::RunContext::HandleError(jv message) {
  messages.push_back(jv_string_value(jq_format_error(message)));
  jv_free(message);
}

Napi::Error NodeJQ::RunContext::NewError(const std::string &message) {
  auto error = Napi::Error::New(env, message);

  std::ostringstream detail;
  std::copy(messages.begin(), messages.end(),
            std::ostream_iterator<std::string>(detail, "\n"));

  std::string detailMessage = detail.str();
  error.Set("detail", detailMessage);

  return error;
}
