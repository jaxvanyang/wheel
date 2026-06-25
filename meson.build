project(
  'wheel',
  'c',
  default_options: ['default_library=static', 'prefer_static=true'],
)
add_project_arguments('-Wno-initializer-overrides', language: 'c')

if host_machine.system() == 'darwin' and get_option('prefer_static')
  # for static linking on macOS
  # https://github.com/raysan5/raylib/wiki/Working-on-macOS#without-xcode---building-statically
  add_project_link_arguments(
    '-framework',
    'CoreVideo',
    '-framework',
    'IOKit',
    '-framework',
    'Cocoa',
    '-framework',
    'GLUT',
    '-framework',
    'OpenGL',
    language: 'c'
  )
endif

subdir('src/wheel')
includes = include_directories('src')
raylib = dependency('raylib', version: '>=5.5')
sqlite = dependency('sqlite3', version: '>=3')
deps = [raylib, sqlite]
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
