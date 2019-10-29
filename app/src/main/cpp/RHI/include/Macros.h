//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_MACROS_H
#define ALITA_MACROS_H

#include <android/log.h>

#define NS_RHI_BEGIN namespace RHI {
#define NS_RHI_END }

#define RHI_ASSERT(cond) assert(cond)

#define RHI_SAFE_RETAIN(obj)    \
do {                            \
if (obj) obj->Retain();         \
} while (0)

#define RHI_SAFE_RELEASE(obj)   \
do {                            \
    if (obj) obj->Release();    \
    (obj) = nullptr;              \
} while (0)

#define RHI_CAST(TYPE, obj) (static_cast<TYPE>(obj))

// Android log function wrappers
static const char* kTAG = "RHI";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

#endif //ALITA_MACROS_H
