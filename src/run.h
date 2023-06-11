#pragma once

extern "C" {
#include <jq.h>
}

#include <napi.h>
#include <string>
#include <v8.h>

Napi::Value Run(const Napi::CallbackInfo &info);
void HandleError(void *ctx, jv value);
