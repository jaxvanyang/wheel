names = [
  'double-pendulum',
  'jump',
  'palette',
  'pendulum',
  'player',
  'poker',
  'snake',
]
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: deps,
    link_with: wheel,
  )
endforeach
