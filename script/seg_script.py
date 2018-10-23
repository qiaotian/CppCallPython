import os
import sys
import numpy as np

def seg_volume(data, origin, spacing):
    print("spacing: ", spacing)
    print("shape:   ", data.shape)

    #data = np.ones((100,100,100), dtype=np.uint8)
    #data = 100 * data

    width = data.shape[0]*spacing[0]
    height = data.shape[1]*spacing[1]
    depth = data.shape[2]*spacing[2]

    draw_sphere(data, spacing,  255, [0,0,0], 50)
    #draw_sphere(data, spacing,  20, [0,0,depth], 50)
    #draw_sphere(data, spacing,  30, [0,height,0], 50)
    #draw_sphere(data, spacing,  40, [0,height,depth], 50)
    #draw_sphere(data, spacing, 150, [width,0,0], 50)
    #draw_sphere(data, spacing, 180, [width,0,depth], 50)
    #draw_sphere(data, spacing, 220, [width,height,0], 50)
    #draw_sphere(data, spacing, 240, [width,height,depth], 50)

    print(data)

    return data.astype(np.uint8)
    #return data
    #print(np.random.randint((3,3)))
    #return np.random.randint(10,size=10).astype(np.uint8)


def draw_sphere(data, spacing, label, center, radius):
    '''
    label: label value
    center: the center position of the ball
    radius: the radius of the ball
    '''
    dims = data.shape
    for i in range(0, dims[0]):
        for j in range(0, dims[1]):
            for k in range(0, dims[2]):
                #delta_x = i * spacing[0] - center[0]
                #delta_y = j * spacing[1] - center[1]
                #delta_z = k * spacing[2] - center[2]
                #if delta_x*delta_x + delta_y*delta_y + delta_z*delta_z < radius*radius:
                #    data[i][j][k] = label
                data[i][j][k] = label

    


if __name__ == '__main__':
    
    pass
