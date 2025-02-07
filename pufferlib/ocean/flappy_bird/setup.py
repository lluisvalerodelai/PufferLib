from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        name="flappy_bird",
        sources=["flappy_bird.pyx", "flappy_bird.c"],  
        libraries=[],
    ),
]

setup(
    name = "testing",
    ext_modules=cythonize(ext_modules)
)
