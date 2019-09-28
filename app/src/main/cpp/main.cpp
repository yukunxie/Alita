
#include <android/log.h>
#include <android_native_app_glue.h>

#include "render/RealRenderer.h"
#include "aux/AFileSystem.h"

#include "drivers/vulkan/vulkan_wrapper.h"

void terminate(void);
void handle_cmd(android_app* app, int32_t cmd);

void android_main(struct android_app* app)
{
    app->onAppCmd = handle_cmd;

    AFileSystem::getInstance()->initAssetManager(app->activity->assetManager);

    AFileSystem::getInstance()->readData("shaders/shader.vert");

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
}

void terminate(void) {
    return ;
}

// Process the next main command.
void handle_cmd(android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            RealRenderer::getInstance()->initVulkanContext(app->window);
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