names = [
  'bigint',
  'core',
  'deque',
  'fib',
  'float',
  'game',
  'hello',
  'kmp',
  'list',
  'lol',
  'math',
  'net',
  'path',
  'poker',
  'sort',
  'str',
  'sys',
  'tree',
  'xray',
]
foreach name: names
  exe = executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: deps,
    link_with: wheel,
  )
  test(name, exe)
endforeach
