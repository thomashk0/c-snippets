project('c_base64_tests', 'c',
    version: '0.1.0',
    default_options : [
      'warning_level=3',
      'c_std=gnu11',
    ])

base64_unittests = executable('base64-unittests', 'base64_unittests.c')
test('base64-unittests', base64_unittests)
