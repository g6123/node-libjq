#ifndef NODE_LIBJQ_CACULATE
#define NODE_LIBJQ_CACULATE

extern "C" {
#include <jq.h>
}

#include <napi.h>
#include <string>
#include <v8.h>

Napi::Value CaculateSync(const Napi::CallbackInfo &info);

#endif // NODE_LIBJQ_CACULATE
