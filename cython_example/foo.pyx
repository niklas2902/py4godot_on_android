# distutils: language = c++
from cython_example.foo cimport foo


def call_foo():
    foo()