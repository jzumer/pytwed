from setuptools import setup, find_packages
from distutils.extension import Extension
import numpy

with open("README.md", "r") as readme_file:
    readme = readme_file.read()

setup(
    name="pytwed",
    version="1.0",
    license="GPLv3",
    author="Pierre-Francois Marteau, JZ, Marcus Voss",
    ext_modules=[Extension('twed', sources = ['pytwed/twed_wrap.c'], include_dirs=[numpy.get_include()])],
    description="This is a python wrapper for Marteau's implementation of his 2009 paper introducing TWED.",
    long_description=readme,
    long_description_content_type="text/markdown",
    url = 'https://github.com/jzumer/pytwed',
    packages=find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3.8",
        "Programming Language :: C",
        "License :: OSI Approved :: MIT License",
    ],
)
