names = [
  'bigint',
  'core',
  'deque',
  'fib',
  'float',
  'hello',
  'kmp',
  'list',
  'lol',
  'math',
  'net',
  'path',
  'sort',
  'str',
  'sys',
  'tree',
]
foreach name: names
  exe = executable(
    name,
    name + '.c',
    include_directories: includes,
    link_with: wheel,
  )
  test(name, exe)
endforeach

names = [
  'poker',
  'xray',
]
foreach name: names
  exe = executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: raylib_and_deps,
    link_with: wheel,
  )
  test(name, exe)
endforeach
