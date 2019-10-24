

include_directories(
        ./
)

file(GLOB SRCS "./*.cpp")
file(GLOB HEADS "./*.h")

add_library( # Sets the name of the library.
        vulkan-lib-static

        # Sets the library as a shared library.
        STATIC

        ${HEADS}
        ./VKRenderPipeline.cpp
        ./VKDevice.cpp
        ./VKCommandBuffer.cpp
        ./VKTextureView.cpp
        ./VKBuffer.cpp
        ./VKShader.cpp
        ./VKPipelineLayout.cpp
        ./VKRenderQueue.cpp
        ./VKQueue.cpp
        ./VKBindingResources.cpp
        ./VKBindGroupLayout.cpp
        ./VKRenderPassEncoder.cpp
        ./VKTexture.cpp
        ./VKSampler.cpp
        ./VKBindGroup.cpp
        ./VKRenderPass.cpp
        ./VKSwapChain.cpp
        ./VKTypes.cpp
        ./VKCommandEncoder.cpp
        )