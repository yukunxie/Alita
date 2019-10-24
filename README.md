

# Alita

This is a Vulkan demo, and offer some abstraction for low level render api.

## Compile shader

Vulkan only uses the shader with SPIR-V format, while we can't write or eidt this format directly, it's not readable for human.

So, we need to write shader with other shader languages, here, we write shader with glsl, and translate glsl to SPIR-V。

- Put your shaders in the directory `$(root)/assets/shaders/`
- Install `Lunarg-G` from https://www.lunarg.com
- Run the python script: `python compile_glsl_to_spirv.py`
