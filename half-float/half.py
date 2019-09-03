#!/usr/bin/python

import numpy as np

arr = np.array([1.12345, 2.3456])
arr_fp16 = arr.astype(np.float16)
fp = open("half.bin", "wb")
fp.write(arr_fp16.tobytes())
fp.close()