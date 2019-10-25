cmake_minimum_required(VERSION 3.4.1)

include_directories(
        .
        ./..
        RHI/backend/Vulkan
)

file(GLOB aux_srcs "aux/*.cpp")

add_library( # Sets the name of the library.
        engine-lib-static

        # Sets the library as a shared library.
        STATIC

        # Base
        Base/ObjectBase.cpp

        # World
        World/World.cpp

        # Render
        render/RealRenderer.cpp

        # Others
        drivers/vulkan/vulkan_wrapper.cpp

        ${aux_srcs}
        )


target_link_libraries( # Specifies the target library.
        engine-lib-static
        vulkan-lib-static
        )