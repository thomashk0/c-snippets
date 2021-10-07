project('c_base64_tests', 'c',
    version: '0.1.0',
    default_options : [
      'warning_level=3',
      'c_std=gnu11',
    ])

base64_unittests = executable('base64-unittests', 'base64_unittests.c')
test('base64-unittests', base64_unittests)

cc = meson.get_compiler('c')
fuzz_src = '''
int foo() { return 0; }
'''
has_fuzzer = cc.compiles(fuzz_src, name: 'LLVM LibFuzzer is supported', args: ['-fsanitize=fuzzer'])

if has_fuzzer and get_option('enable-fuzzing')
  base64_fuzz = executable('base64-fuzz',
    'base64_fuzz.c',
    c_args: ['-fsanitize=fuzzer'],
    link_args: ['-fsanitize=fuzzer'])
  test('base64-fuzz', base64_fuzz, args: ['-max_total_time=5'])
endif
