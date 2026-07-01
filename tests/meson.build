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

test_deps = []
if system == 'windows'
  test_deps += ws2_32
endif
exe = executable(
  'net',
  'net.c',
  include_directories: includes,
  dependencies: test_deps,
  link_with: wheel,
)
test(name, exe)

names = [
  'poker',
  'xray',
]
foreach name: names
  exe = executable(
    name,
    name + '.c',
    include_directories: includes,
    dependencies: raylib_and_deps + math,
    link_with: wheel,
  )
  test(name, exe)
endforeach
