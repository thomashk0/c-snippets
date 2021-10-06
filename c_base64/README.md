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

Go in the `test` subdirectory:
```console
$ meson build
$ cd build
$ ninja test
```

## License

This project is licensed under a zlib/libpng.
See [LICENSE](./LICENSE) for more information.
