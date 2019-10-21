

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
        ${SRCS}
        ./VKTexture.cpp
        ./VKSampler.cpp
        ./VKBindGroupLayout.cpp
        ./VKPipelineLayout.cpp
        ./VKBindingResources.cpp
        ./VKBindGroup.cpp
        ./VKTextureView.cpp
        ./VKCommandEncoder.cpp
        ./VKCommandBuffer.cpp
        ./VKRenderPassEncoder.cpp
        ./VKQueue.cpp
        ./VKSwapChain.cpp
        )