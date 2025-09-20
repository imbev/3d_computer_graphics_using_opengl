project('3d_computer_graphics_using_opengl', 'c')

deps = [
    dependency('glew'),
    dependency('glfw3')
]

include_dirs = [
    include_directories('include')
]

sources = [
    'src/main.c',
    'src/me_gl_window.c'
]

fs = import('fs')
fs.copyfile('shaders/fragment_shader_code.glsl')
fs.copyfile('shaders/vertex_shader_code.glsl')

executable(
    'main',
    sources,
    dependencies: deps,
    include_directories: include_dirs
)