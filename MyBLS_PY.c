/*********************************************************************************
*********************************************************************************/

#include "MyBLS_PY.h"

/*********************************************************************************
Initialise the module and define docstrings etc
*********************************************************************************/

//define methods in the module
PyMethodDef MyBLS_PYMethods[] = {
    {"MyBLS", (PyCFunction) mybls_py, METH_VARARGS|METH_KEYWORDS,MYBLS_DOCSTRING},
    {NULL, NULL, 0, NULL}
};

//initialise the module
PyMODINIT_FUNC 
initMyBLS_PY(void)
{
    (void) Py_InitModule3("MyBLS_PY", MyBLS_PYMethods,MYBLS_PY_DOCSTRING);
    import_array(); //must be present for numpy stuff
}

/********************************************************************************/
//main function called from python - converts args to c varaiables then calls chain func
static PyObject * mybls_py (PyObject *self, PyObject *args, PyObject *keywds)
{
	/********************************************************************************/
	//input keywords
    static char *kwlist[] = {"time_object","flux_object","weight_object","pmin","pmax","kmin","kmax","sampling",NULL};
    
	//define input numpy arrays
	PyObject * time_object;
	PyArrayObject * time;
	PyObject * flux_object;
	PyArrayObject * flux;
	PyObject * weight_object;
	PyArrayObject * weight;
	
	//define input doubles
	double pmin,pmax,kmin,kmax,sampling;
	
	//no measurements
	long nmeas;
	
	//define outputs from C function
	long np = NPMAX;
	double period[NPMAX], statistic[NPMAX], dc[NPMAX], duration[NPMAX], 
    	epoch[NPMAX], depth[NPMAX];
	
	//define arrays to return to python
	PyArrayObject * period_object, *statistic_object, *dc_object, *duration_object, *epoch_object, *depth_object;

	//read in python arguments
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "OOOddddd", kwlist, &time_object,&flux_object,&weight_object,&pmin,&pmax,&kmin,&kmax,&sampling))
	    {
	    printf("Problem loading args...\n");
		return NULL;
		}

	//convert numpy object to contiguous array of doubles
	time = (PyArrayObject*) PyArray_ContiguousFromObject(time_object, PyArray_DOUBLE, 1, 10);
	flux = (PyArrayObject*) PyArray_ContiguousFromObject(flux_object, PyArray_DOUBLE, 1, 10);
	weight = (PyArrayObject*) PyArray_ContiguousFromObject(weight_object, PyArray_DOUBLE, 1, 10);
	if(time == NULL) {printf("time array error!\n"); return 0;}
	if(flux == NULL) {printf("flux array error!\n"); return 0;}
	if(weight == NULL) {printf("weight array error!\n"); return 0;}
	
    //get no of data points in time array
	nmeas = *(time->dimensions);

	//call mybls c function
	mybls( ((double*)time->data), ((double*)flux->data), ((double*)weight->data), nmeas, pmin,pmax,kmin,kmax,sampling,
	  period, statistic, dc, duration, epoch, depth, &np);

	//destroy reference to input arrays
	Py_DECREF(time);
	Py_DECREF(flux);
	Py_DECREF(weight);
		
	//initiate return arrays as doubles of size np
	period_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	statistic_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	dc_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	duration_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	epoch_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	depth_object = (PyArrayObject*) PyArray_SimpleNew(1,&np,PyArray_DOUBLE);
	
	//make numpy objects point to the same memory blocks
	period_object->data = (char*)period;
	statistic_object->data = (char*)statistic;
	dc_object->data = (char*)dc;
	duration_object->data = (char*)duration;
	epoch_object->data = (char*)epoch;
	depth_object->data = (char*)depth;
	
	//return arrays
  	return Py_BuildValue("OOOOOO",period_object,statistic_object,dc_object,duration_object,epoch_object,depth_object);
}

/********************************************************************************/
