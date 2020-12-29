from setuptools import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy

setup(
    name="twed",
    version="1.0",
    license="GPL",
    author="Pierre-Francois Marteau, JZ, Marcus Voss",
    cmdclass={'build_ext': build_ext},
    ext_modules=[Extension('twed', sources = ['src/twed_wrap.c'], include_dirs=[numpy.get_include()])],
    include_dirs=[numpy.get_include()],
    description="This is a python wrapper for Marteau's implementation of his 2009 paper introducing TWED.",
   # packages=["twed"],
    url = 'https://github.com/jzumer/pytwed'
)
