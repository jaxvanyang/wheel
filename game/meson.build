link_args = []
name_suffix = []
if system == 'emscripten'
  shell_file = files('../src/minshell.html')[0].full_path()
  link_args += ['-s', 'USE_GLFW=3', '--shell-file', shell_file]
  name_suffix = 'html'
endif

names = [
  'double-pendulum',
  'palette',
  'pendulum',
  'snake',
]
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: raylib_and_deps,
    link_with: wheel,
    link_args: link_args,
    name_suffix: name_suffix,
    install: true,
  )
endforeach

# following games need assets/
if system == 'emscripten'
  link_args += ['--preload-file', meson.project_source_root() / 'assets@assets']
endif

names = [
  'jump',
  'poker',
]
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: raylib_and_deps,
    link_with: wheel,
    link_args: link_args,
    name_suffix: name_suffix,
    install: true,
  )
endforeach

game_deps = raylib_and_deps + sqlite
if system == 'windows'
  game_deps += ws2_32
endif
executable(
  'poker2',
  'poker2.c',
  include_directories: includes,
  dependencies: game_deps,
  link_with: wheel,
  link_args: link_args,
  name_suffix: name_suffix,
  install: true,
)
