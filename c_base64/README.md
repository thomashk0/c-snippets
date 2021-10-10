# A Single Header C99 Base64 Encoding and Decoding Library

## Usage

Add `base64.h` in your project. Then, in one file of the project, instanciate
the implementation (only once).

```c
#define C_BASE64_IMPL
#include "base64.h"
```

Customizable macros (must be declared before the `#include "base64.h"`):

- `C_BASE64_ASSERT(prop)`: custom assertion macro.
- `C_BASE64_MALLOC(size)`: custom malloc function
- `C_BASE64_FREE(ptr)`: custom free function

## Developper guide

To run all tests, you need to install `libfiu` (e.g., `sudo apt install libfiu-dev`).
We also recommend using clang (which will allow tests with fuzzing), remove the `CC=clang` to use gcc or you default compiler.

Go in the `test` subdirectory:
```console
$ CC=clang meson build
$ cd build
$ ninja test
```

### Generate coverage report

```console
$ CC=clang meson -Db_coverage=true build-cov
$ cd build-cov
$ lcov --capture --directory . --output-file coverage.info
$ genhtml -o html coverage.info
```

## License

This project is licensed under a zlib/libpng.
See [LICENSE](./LICENSE) for more information.
