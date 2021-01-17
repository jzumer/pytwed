from setuptools import setup, find_packages
from distutils.extension import Extension
import numpy

with open("README.md", "r") as readme_file:
    readme = readme_file.read()

setup(
    name="pytwed",
    version="1.0.9",
    license="MIT License",
    author="Pierre-Francois Marteau, JZ, Marcus Voss",
    setup_requires=["numpy"],
    ext_modules=[Extension('twed', sources=['pytwed/fast_twed.c', 'pytwed/twed.c'], include_dirs=["pytwed/", numpy.get_include()], extra_compile_args=["-std=c11"], language="c")],
    description="This is a python wrapper for Marteau's reference implementation of his 2009 paper introducing the Time Warped Edit Distance (TWED).",
    long_description=readme,
    long_description_content_type="text/markdown",
    url='https://github.com/jzumer/pytwed',
    packages=find_packages(),
    include_package_data=True,
    classifiers=[
        "Programming Language :: Python :: 3",
        "Programming Language :: C",
        "License :: OSI Approved :: MIT License",
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering"
    ],
)
