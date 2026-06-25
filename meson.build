project('wheel', 'c')

subdir('src/wheel')
includes = include_directories('src')
raylib = dependency('raylib', version: '>=5.5')
sqlite = dependency('sqlite3', version: '>=3')
deps = [raylib, sqlite]
wheel = library(
  'wheel',
  sources,
  include_directories: includes,
  dependencies: deps,
)

subdir('bin')
subdir('tests')
