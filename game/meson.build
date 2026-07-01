game_deps = [raylib]
if host_machine.system() == 'linux'
  x11 = dependency('x11')
  game_deps += x11
endif

names = [
  'double-pendulum',
  'jump',
  'palette',
  'pendulum',
  'poker',
  'snake',
]
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: game_deps,
    link_with: wheel,
    install: true,
  )
endforeach

executable(
  'poker2',
  'poker2.c',
  include_directories: includes,
  dependencies: game_deps + sqlite,
  link_with: wheel,
  install: true,
)
