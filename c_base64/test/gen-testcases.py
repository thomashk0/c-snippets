#!/usr/bin/env python3

import base64
import random

def c_repr(b):
    return b.decode('ascii', 'backslashreplace')

def main():
    for i in range(50):
        s = random.randbytes(random.randint(1, 13))
        s_b64 = base64.b64encode(s)
        print(f'ASSERT_EQ(check_base64("{c_repr(s)}", "{c_repr(s_b64)}"), 0);')


if __name__ == "__main__":
    main()
