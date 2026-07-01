project(
  'wheel',
  'c',
  version: '0.1.0',
  default_options: ['default_library=static', 'prefer_static=true'],
  meson_version: '>=1.4',
)

add_project_arguments(
  '-Wno-comment',
  '-Wno-initializer-overrides',
  '-Wno-override-init',
  language: 'c'
)

fs = import('fs')
system = host_machine.system()
prefer_static = get_option('prefer_static')
raylib_src_path = get_option('raylib_src_path')
raylib_path = get_option('raylib_path')
cc = meson.get_compiler('c')

# Dependencies
if system == 'emscripten'
  # SQLite depends on pthread which causes problems on Web, see #24
  sqlite = dependency('', required: false)
else
  sqlite = dependency('sqlite3', version: '>=3')
endif

# handle custom Raylib
if raylib_src_path != ''
  raylib_inc = include_directories(raylib_src_path)
  if not fs.is_absolute(raylib_src_path)
    raylib_src_path = meson.current_source_dir() / raylib_src_path
  endif
  if system == 'emscripten'
    link_args = [raylib_src_path / 'libraylib.web.a']
  elif prefer_static
    # force to use the static library on macOS
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

raylib_and_deps = [raylib]
if system == 'linux' and (raylib_src_path != '' or raylib_path != '')
  # pre-built Raylib on Linux depends on X11
  x11 = dependency('x11')
  raylib_and_deps += x11
endif
if system == 'darwin' and prefer_static
  raylib_and_deps += dependency('appleframeworks', modules: ['CoreVideo', 'IOKit', 'Cocoa', 'GLUT', 'OpenGL'])
endif
if system == 'windows'
  ws2_32 = cc.find_library('ws2_32')
  raylib_and_deps += cc.find_library('winmm')
endif

# Targets
subdir('src/wheel')
includes = include_directories('src')
wheel = library(
  'wheel',
  sources,
  include_directories: includes,
  dependencies: [raylib, sqlite],
  install: true,
)

if system != 'emscripten'
  subdir('bin')
  subdir('tests')
endif
subdir('game')
