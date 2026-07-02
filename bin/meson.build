names = [
  'hex2double',
  'hex2float',
  'hilb',
  'max-det',
  'nproc',
  'runtest',
  'sort',
  'uniq',
]
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    link_with: wheel,
    install: true,
  )
endforeach

names = [
  'echo',
  'udpnet',
]
bin_deps = []
if system == 'windows'
  bin_deps += ws2_32
endif
foreach name: names
  executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: bin_deps,
    link_with: wheel,
    install: true,
  )
endforeach
executable(
  'casino',
  'casino.c',
  include_directories: includes,
  dependencies: bin_deps + sqlite,
  link_with: wheel,
  install: true,
)

executable(
  'insert-words',
  'insert-words.c',
  include_directories: includes,
  dependencies: sqlite,
  link_with: wheel,
  install: true,
)

executable(
  'poker-prob',
  'poker-prob.c',
  include_directories: includes,
  dependencies: raylib_and_deps,
  link_with: wheel,
  install: true,
)
