names = [
  'casino',
  'echo',
  'hex2double',
  'hex2float',
  'hilb',
  'insert-words',
  'max-det',
  'nproc',
  'poker-prob',
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
    dependencies: deps,
    link_with: wheel,
    install: true,
  )
endforeach
