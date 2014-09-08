
//system headers
#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

//local headers
#include "mybls.h"

//constants
#define NDMAX 30000
#define NPMAX 50000

//docstrings
#define MYBLS_PY_DOCSTRING \
"MyBLS_PY module docstring...\n\
...\n\
"
#define MYBLS_DOCSTRING \
"MyBLS_PY.MyBLS function docstring...\n\
...\n\
"

//wrapper function prototype
static PyObject * mybls_py(PyObject *self, PyObject *args, PyObject *keywds);
