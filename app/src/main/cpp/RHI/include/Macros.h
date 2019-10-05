//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_MACROS_H
#define ALITA_MACROS_H

#include <android/log.h>

#define NS_RHI_BEGIN namespace RHI {
#define NS_RHI_END }

#define RHI_ASSERT(cond) assert(cond)


// Android log function wrappers
static const char *kTAG = "RHI";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))


#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "Tutorial ",               \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
  }


#endif //ALITA_MACROS_H
