project(
  'wheel',
  'c',
  default_options: ['default_library=static', 'prefer_static=true'],
  meson_version: '>=1.1',
)

fs = import('fs')

add_project_arguments(
  '-Wno-comment',
  '-Wno-initializer-overrides',
  '-Wno-override-init',
  language: 'c'
)

sqlite = dependency('sqlite3', version: '>=3')

prefer_static = get_option('prefer_static')
raylib_src_path = get_option('raylib_src_path')
raylib_path = get_option('raylib_path')
if raylib_src_path != ''
  raylib_inc = include_directories(raylib_src_path)
  if not fs.is_absolute(raylib_src_path)
    raylib_src_path = meson.current_source_dir() / raylib_src_path
  endif
  if prefer_static
    link_args = [raylib_src_path / 'libraylib.a']
  else
    link_args = ['-L' + raylib_src_path, '-lraylib']
  endif
  raylib = declare_dependency(
    include_directories: raylib_inc,
    link_args: link_args,
  )
elif raylib_path != ''
  raylib_inc = include_directories(raylib_path / 'include')
  if not fs.is_absolute(raylib_path)
    raylib_path = meson.current_source_dir() / raylib_path
  endif
  if prefer_static
    link_args = [raylib_path / 'lib' / 'libraylib.a']
  else
    link_args = ['-L' + (raylib_path / 'lib'), '-lraylib']
  endif
  raylib = declare_dependency(
    include_directories: raylib_inc,
    link_args: link_args,
  )
else
  raylib = dependency('raylib', version: '>=5.5')
endif

deps = [raylib, sqlite]
if host_machine.system() == 'darwin' and prefer_static
  deps += dependency('appleframeworks', modules: ['CoreVideo', 'IOKit', 'Cocoa', 'GLUT', 'OpenGL'])
endif
if host_machine.system() == 'windows'
  add_project_link_arguments('-lgdi32', '-lwinmm', '-lws2_32', language: 'c')
endif

subdir('src/wheel')
includes = include_directories('src')
wheel = library(
  'wheel',
  sources,
  include_directories: includes,
  dependencies: deps,
  install: true,
)

subdir('bin')
subdir('game')
subdir('tests')
