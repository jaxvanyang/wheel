names = [
  'casino',
  'echo',
  'hex2double',
  'hex2float',
  'hilb',
  'insert-words',
  'max-det',
  'nproc',
  'runtest',
  'sort',
  'udpnet',
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

executable(
  'poker-prob',
  'poker-prob.c',
  include_directories: includes,
  dependencies: raylib,
  link_with: wheel,
  install: true,
)
