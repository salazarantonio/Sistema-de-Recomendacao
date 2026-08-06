from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "meu_solver",
        ["bindings.cpp", "lista_compras.cpp", "similaridade.cpp", "recomendacao.cpp"],
        include_dirs=[pybind11.get_include()],
        language="c++",
        extra_compile_args=["/std:c++17"],
    ),
]

setup(name="meu_solver", ext_modules=ext_modules)