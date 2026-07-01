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
    dependencies: raylib_and_deps,
    link_with: wheel,
    install: true,
  )
endforeach

executable(
  'poker2',
  'poker2.c',
  include_directories: includes,
  dependencies: raylib_and_deps + sqlite,
  link_with: wheel,
  install: true,
)
