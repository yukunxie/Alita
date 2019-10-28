cmake_minimum_required(VERSION 3.4.1)

include_directories(
        .
        ./..
        RHI/backend/Vulkan
)

#file(GLOB aux_srcs "aux/*.cpp")

file(GLOB_RECURSE HEADERS
        "aux/*.h"
        "aux/*.hpp"
        "Base/*.h"
        "World/*.h"
        "render/*.h"
        "drivers/*.h"
        "types/*.h"
        )

file(GLOB_RECURSE SRCS
        "aux/*.cpp"
        "Base/*.cpp"
        "types/*.cpp"
        "World/*.cpp"
        "render/*.cpp"
        "drivers/*.cpp"
        )

add_library( # Sets the name of the library.
        engine-lib-static

        # Sets the library as a shared library.
        STATIC

        ${HEADERS}
        ${SRCS}
        )


target_link_libraries( # Specifies the target library.
        engine-lib-static
        vulkan-lib-static
        )