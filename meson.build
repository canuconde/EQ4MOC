project('EQ4MOC', 'cpp', default_options : ['cpp_std=c++17'])
ncurses_dep = dependency('ncurses')

my_inc = include_directories('include')
my_dep = declare_dependency(include_directories : my_inc)

src = ['src/eq4moc.cpp', 'src/eqPresets.cpp', 'src/eqSet.cpp', 'src/mTheme.cpp', 'src/ncFunctions.cpp']

executable('eq4moc', src, dependencies : [ncurses_dep, my_dep], install: true)
