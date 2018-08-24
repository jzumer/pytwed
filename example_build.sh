#!/bin/bash
gcc -std=c99 twed_wrap.c -lm -lpython2.7 -shared -o twed.so -fPIC -I /usr/lib64/python2.7/site-packages/numpy/core/include/ -I /usr/include/python2.7
