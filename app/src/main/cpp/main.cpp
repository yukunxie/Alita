
#include <android/log.h>
#include <android_native_app_glue.h>

#include "VulkanDemo.h"

#include "vulkan_wrapper.h"

void terminate(void);
void handle_cmd(android_app* app, int32_t cmd);

void android_main(struct android_app* app)
{
    app->onAppCmd = handle_cmd;

    int events;
    android_poll_source* source;
    do {
        if (ALooper_pollAll(RealRenderer::getInstance()->isReady() ? 1 : 0, nullptr, &events,
                            (void**)&source) >= 0) {
            if (source != NULL) source->process(app, source);
        }
        if (RealRenderer::getInstance()->isReady())
        {
            RealRenderer::getInstance()->drawFrame();
        }
    } while (app->destroyRequested == 0);

    initVulkanInstance(app);
}

void terminate(void) {
    return ;
//    vkDestroySurfaceKHR(tutorialInstance, tutorialSurface, nullptr);
//    vkDestroyDevice(tutorialDevice, nullptr);
//    vkDestroyInstance(tutorialInstance, nullptr);
}

// Process the next main command.
void handle_cmd(android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            RealRenderer::getInstance()->initVulkanContext(app);
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            terminate();
            break;
        default:
            break;
//            LOGI("event not handled: %d", cmd);
    }
}