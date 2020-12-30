from setuptools import setup
from distutils.extension import Extension
import numpy

setup(
    name="pytwed",
    version="1.0",
    license="GPLv3",
    author="Pierre-Francois Marteau, JZ, Marcus Voss",
    ext_modules=[Extension('twed', sources = ['pytwed/twed_wrap.c'], include_dirs=[numpy.get_include()])],
    description="This is a python wrapper for Marteau's implementation of his 2009 paper introducing TWED.",
    url = 'https://github.com/jzumer/pytwed'
)
