from setuptools import setup
from Cython.Build import cythonize
from setuptools.extension import Extension
from os.path import relpath, dirname, join as path_join
from typing import List

PACKAGE_DIR = relpath(dirname(__file__))


def get_extension() -> List[Extension]:
    sources = [
        path_join(PACKAGE_DIR, "cfactorypython", "_dataset.pyx"),
        path_join(PACKAGE_DIR, "src", "dataset.c"),
    ]
    return [
        Extension(
            name="cfactorypython._dataset",
            sources=sources,
            include_dirs=["includes"],
            extra_compile_args=["-fPIC"],
            extra_link_args=["-lm"],
        )
    ]


setup(
    name="dataset",
    ext_modules=cythonize(get_extension())
)
