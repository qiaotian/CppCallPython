import os
import sys
import numpy as np
import nibabel

newdata = np.random.random((3,3))

def seg_volume(origin, spacing, dims, data):
    print("origin:")
    print(origin)
    print("spacing")
    print(spacing)
    print("data")
    print(data[:100])

    return newdata

if __name__ == '__main__':
    
    pass
