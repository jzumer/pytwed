/*
Filename: twed_wrap.c
Python wrapper for twed
source code for the Time Warp Edit Distance in ANSI C.
Author: Pierre-Francois Marteau, Rewritten by JZ based on the python version
Licence: MIT
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>

#define INDEX(x, y) ((y)*arr1_lgt + (x))

double dist(double* arr1, double* arr2, int lgt, int degree) {
    double ret = 0;

    for(unsigned int i = 0; i < lgt; i++) {
        ret += pow(fabs(arr1[i] - arr2[i]), degree);
    }

    return pow(ret, 1.0 / degree);
}

double DTWEDL1d(int n_feats, double* arr1, int arr1_lgt, double* arr1_spec, double* arr2, int arr2_lgt, double* arr2_spec, double nu, double lambda, int degree) {
    // Version based on the python version

    // INIT
    double* D = (double*)calloc((arr1_lgt)*(arr2_lgt), sizeof(double));
    for(unsigned int i = 1; i < arr1_lgt; i++) {
        D[INDEX(i, 0)] = INFINITY;
    }
    for(unsigned int i = 1; i < arr2_lgt; i++) {
        D[INDEX(0, i)] = INFINITY;
    }

    // Go!
    for(unsigned int j = 1; j < arr2_lgt; j++) {
        for(unsigned int i = 1; i < arr1_lgt; i++) {
            float del_a = D[INDEX(i-1, j)]
                            + dist(arr1+((i-1)*n_feats), arr1+(i*n_feats), n_feats, degree)
                            + nu * (arr1_spec[i] - arr1_spec[i-1])
                            + lambda;
            float del_b = D[INDEX(i, j-1)]
                            + dist(arr2+((j-1)*n_feats), arr2+(j*n_feats), n_feats, degree)
                            + nu * (arr2_spec[j] - arr2_spec[j-1])
                            + lambda;
            float match = D[INDEX(i-1, j-1)]
                            + dist(arr1+(i*n_feats), arr2+(j*n_feats), n_feats, degree)
                            + dist(arr1+((i-1)*n_feats), arr2+((j-1)*n_feats), n_feats, degree)
                            + nu * (fabs(arr1_spec[i] - arr2_spec[j]) + fabs(arr1_spec[i-1] - arr2_spec[j-1]));

            D[INDEX(i, j)] = fmin(match, fmin(del_a, del_b));
        }
    }

    double ret = D[INDEX(arr1_lgt-1, arr2_lgt-1)];
    free(D);
    return ret;
}

static PyObject* twed_ (PyObject* dummy, PyObject* args, PyObject* kw) {
    PyObject* input1 = NULL;
    PyObject* input2 = NULL;
    PyObject* input3 = NULL;
    PyObject* input4 = NULL;

    double nu = 0.001;
    double lmbda = 1.0;
    int degree = 2;

    static char* keywords[] = { "arr1", "arr2", "arr1_spec", "arr2_spec", "nu", "lmbda", "degree", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kw, "OOOO|ddi", keywords, &input1, &input2, &input3, &input4, &nu, &lmbda, &degree)) {
        Py_RETURN_NONE;
    }

    PyArrayObject* arr1 = NULL;
    arr1 = (PyArrayObject*)PyArray_FromAny(input1, PyArray_DescrFromType(NPY_DOUBLE), 1, 2, NPY_ARRAY_CARRAY_RO, NULL);
    if(arr1 == NULL) goto fail;
    PyArrayObject* arr2 = NULL;
    arr2 = (PyArrayObject*)PyArray_FromAny(input2, PyArray_DescrFromType(NPY_DOUBLE), 1, 2, NPY_ARRAY_CARRAY_RO, NULL);
    if(arr2 == NULL) goto fail;
    PyArrayObject* ts_specs1 = NULL;
    ts_specs1 = (PyArrayObject*)PyArray_FromAny(input3, PyArray_DescrFromType(NPY_DOUBLE), 1, 1, NPY_ARRAY_CARRAY_RO, NULL);
    if(ts_specs1 == NULL) goto fail;
    PyArrayObject* ts_specs2 = NULL;
    ts_specs2 = (PyArrayObject*)PyArray_FromAny(input4, PyArray_DescrFromType(NPY_DOUBLE), 1, 1, NPY_ARRAY_CARRAY_RO, NULL);
    if(ts_specs2 == NULL) goto fail;

    int ndims = PyArray_NDIM(arr1);
    uint64_t* arr1_dims = PyArray_DIMS(arr1);
    uint64_t* arr2_dims = PyArray_DIMS(arr2);
	
    int n_feats = 1;
    if(ndims > 1) { 
		n_feats = arr1_dims[1];
	} else {
        uint64_t dims1[] = { arr1_dims[0], 1 };
        PyArray_Dims newshape1 = { dims1, 2 };

        uint64_t dims2[] = { arr2_dims[0], 1 };
        PyArray_Dims newshape2 = { dims2, 2 };

        arr1 = (PyArrayObject*)PyArray_Newshape(arr1, &newshape1, NPY_CORDER);
        arr2 = (PyArrayObject*)PyArray_Newshape(arr2, &newshape2, NPY_CORDER);
    }

    double* arr1_data = (double*)PyArray_DATA(arr1);
    double* arr2_data = (double*)PyArray_DATA(arr2);
    double* arr1_specs_data = (double*)PyArray_DATA(ts_specs1);
    double* arr2_specs_data = (double*)PyArray_DATA(ts_specs2);

    double ret = DTWEDL1d(n_feats, arr1_data, (int)arr1_dims[0], arr1_specs_data, arr2_data, (int)arr2_dims[0], arr2_specs_data, nu, lmbda, degree);

    Py_DECREF(arr1);
    Py_DECREF(arr2);
    Py_DECREF(ts_specs1);
    Py_DECREF(ts_specs2);

    return PyFloat_FromDouble(ret);

fail:
    Py_XDECREF(arr1);
    Py_XDECREF(arr2);
    Py_XDECREF(ts_specs1);
    Py_XDECREF(ts_specs2);

    Py_RETURN_NONE;
}

static PyMethodDef twedmethods[] = {
    { "twed", (PyCFunctionWithKeywords)twed_,
      METH_VARARGS|METH_KEYWORDS,
      "Computes the Time Warp Edit Distance (Marteau, 2009).\nInputs:\n\tFirst input (N x D)\n\tSecond input (M x D)\n\tTimepoint indices for the first input (N x 1)\n\tTimepoint indices for the second input (M x 1)\n\tKeywords nu (double), lmbda (double), degree (integer)\n\nOutput:\n\tThe distance between the two inputs."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};
   
PyMODINIT_FUNC PyInit_twed(void)
{
    PyObject *module;
    static struct PyModuleDef twed = {
    PyModuleDef_HEAD_INIT,
    "twed", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    twedmethods,
	NULL,
    NULL,
    NULL,
    NULL
	};
    module = PyModule_Create(&twed);
    if (!module) return NULL;

    /* Load `numpy` functionality. */
    import_array();

    return module;
}
