//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_VULKANMARCOS_H
#define ALITA_VULKANMARCOS_H

#include <android/log.h>

// Vulkan call wrapper
#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "RHI ",                    \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
  }

#endif //ALITA_VULKANMARCOS_H
