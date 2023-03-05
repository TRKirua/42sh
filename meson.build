project('42sh',
        'c',
        version : '1.1.0',
        default_options : ['warning_level=3', 'c_std=c99', 'c_args=-D_POSIX_C_SOURCE=200809L -Wvla -Werror -Wall -Wextra -pedantic -fsanitize-recover -g']
)

src = [
  'src/cd_builtin.c',
  'src/dot_builtin.c',
  'src/echo_builtin.c',
  'src/exe.c',
  'src/exe_cmd.c',
  'src/export_builtin.c',
  'src/launch.c',
  'src/lexer.c',
  'src/main.c',
  'src/mmalloc.c',
  'src/parser.c',
  'src/parser_two.c'
]

project_dependencies = [
]

build_args = [
]

project_target = executable('42sh', src)
test('basic', project_target)
