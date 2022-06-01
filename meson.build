project(
    'byteboard',
    'c',
    default_options: [
        'warning_level=3', 'werror=true', 'optimization=3', 'c_std=c17',
        'debug=false', 'b_ndebug=if-release'
    ],
)

###############################################################################

source_files = [
    'src/main.c', 'src/bitboard.c', 'src/rays.c',
    'src/patterns.c', 'src/masks.c', 'src/position.c',
    'src/evaluate.c',
]

incdir = include_directories('include')

target = executable(
    'bb', source_files, include_directories : incdir
)

###############################################################################

doxygen = find_program('doxygen', required : false)

if doxygen.found()
    message('doxygen Doxyfile')
else
    warning('Documentation disabled without doxygen')
endif
