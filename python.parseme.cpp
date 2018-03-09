#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../glm/glm.hpp"
#include <sstream>
#include "../glm/ext.hpp"
#include "../glm/gtc/noise.hpp"

#include "python.hpp"

namespace glmpython {

/*
https://docs.python.org/3/c-api/buffer.html

PyBuffer struct has member arrays shape and strides which should be allocated and filled in the getbuffer function.
This memory should later be released in releasebuffer function.

Since shape and strides aren't arrays but numbers for glm objects, it would be wasteful to do all these allocations and deletes 
each time a buffer is requested.

Also since the documentation guarantees that shape and strides are read only, we can just set them to point to a static array.
Hence the horrible constants_array.

By Jure.
*/
static Py_ssize_t constants_array[15] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14
};

/* * * Header * * */
/*$ MATRIX $*/

/* ${p}mat${n} */

static PyObject *glm_${p}mat${n}_nb_add(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_subtract(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_multiply(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_true_divide(PyObject *, PyObject *);

static PyObject *glm_${p}mat${n}_nb_negative(PyObject *);
static PyObject *glm_${p}mat${n}_nb_positive(PyObject *);

static PyObject *glm_${p}mat${n}_nb_inplace_add(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_inplace_subtract(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_inplace_multiply(PyObject *, PyObject *);
static PyObject *glm_${p}mat${n}_nb_inplace_true_divide(PyObject *, PyObject *);

static Py_ssize_t glm_${p}mat${n}_sq_length(PyObject *);
static PyObject *glm_${p}mat${n}_sq_item(PyObject *, glm::length_t);
static int glm_${p}mat${n}_sq_ass_item(PyObject *, glm::length_t, PyObject *);

static PyObject *glm_${p}mat${n}_tp_repr(PyObject *);
static int glm_${p}mat${n}_tp_init(PyObject *, PyObject *, PyObject *);

static int glm_${p}mat${n}_bf_getbuffer(PyObject *, Py_buffer *, int);

static PyObject *glm_${p}mat${n}_tp_richcompare(PyObject *, PyObject *, int);
static Py_hash_t glm_${p}mat${n}_tp_hash(PyObject *);
/*$ $*/
/*$ VECTOR $*/

/* ${p}vec${n} */


/*$ VECTOR_MATH $*/
$?{not only or type == only or type in only
static PyObject *glm_${p}vec${n}_nb_${f}(PyObject *, PyObject *);
static PyObject *glm_${p}vec${n}_nb_inplace_${f}(PyObject *, PyObject *);
$?}
/*$ $*/
$?{type == 'float'
static PyObject *glm_${p}vec${n}_nb_remainder(PyObject *, PyObject *);
static PyObject *glm_${p}vec${n}_nb_divmod(PyObject *, PyObject *);
static PyObject *glm_${p}vec${n}_nb_power(PyObject *, PyObject *);
$?}
static PyObject *glm_${p}vec${n}_nb_negative(PyObject *);
static PyObject *glm_${p}vec${n}_nb_positive(PyObject *);
static PyObject *glm_${p}vec${n}_nb_absolute(PyObject *);
$?{type == 'int'

static PyObject *glm_${p}vec${n}_nb_invert(PyObject *);
$?}

$?{type == 'float'
static PyObject *glm_${p}vec${n}_nb_floor_divide(PyObject *, PyObject *);
static PyObject *glm_${p}vec${n}_nb_inplace_floor_divide(PyObject *, PyObject *);
$?}

static Py_ssize_t glm_${p}vec${n}_sq_length(PyObject *);
static PyObject *glm_${p}vec${n}_sq_item(PyObject *, glm::length_t);
static int glm_${p}vec${n}_sq_ass_item(PyObject *, glm::length_t, PyObject *);

static PyObject *glm_${p}vec${n}_tp_repr(PyObject *);
static PyObject *glm_${p}vec${n}_tp_getattro(PyObject *, PyObject *);
static int glm_${p}vec${n}_tp_setattro(PyObject *, PyObject *, PyObject *);
static int glm_${p}vec${n}_tp_init(PyObject *, PyObject *, PyObject *);

static int glm_${p}vec${n}_bf_getbuffer(PyObject *, Py_buffer *, int);

static PyObject *glm_${p}vec${n}_tp_richcompare(PyObject *, PyObject *, int);
static Py_hash_t glm_${p}vec${n}_tp_hash(PyObject *);
/*$ $*/

/* * * Types * * */

/*$ MATRIX $*/
static
PyNumberMethods glm_${p}mat${n}_NumberMethods = {
	(binaryfunc)glm_${p}mat${n}_nb_add,
	(binaryfunc)glm_${p}mat${n}_nb_subtract,
	(binaryfunc)glm_${p}mat${n}_nb_multiply,
	NULL,
	NULL,
	NULL,
	(unaryfunc)glm_${p}mat${n}_nb_negative,
	(unaryfunc)glm_${p}mat${n}_nb_positive,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	(binaryfunc)glm_${p}mat${n}_nb_inplace_add,
	(binaryfunc)glm_${p}mat${n}_nb_inplace_subtract,
	(binaryfunc)glm_${p}mat${n}_nb_inplace_multiply,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	NULL,
	(binaryfunc)glm_${p}mat${n}_nb_true_divide,
	NULL,
	(binaryfunc)glm_${p}mat${n}_nb_inplace_true_divide,

	NULL,
};

static
PySequenceMethods glm_${p}mat${n}_SequenceMethods = {
	(lenfunc)glm_${p}mat${n}_sq_length,
	NULL,
	NULL,
	(ssizeargfunc)glm_${p}mat${n}_sq_item,
	NULL,
	(ssizeobjargproc)glm_${p}mat${n}_sq_ass_item,
	NULL,
	NULL,

	NULL,
	NULL,
};

static
PyBufferProcs glm_${p}mat${n}_BufferMethods = {
	(getbufferproc)glm_${p}mat${n}_bf_getbuffer,
    NULL
};

// ${p}mat${n} Methods

/*$ MATRIX_FUNCTION $*/
$?{availableTo == 'all' or n in availableTo
static PyObject *glm_${p}mat${n}_function_${func}(PyObject *, PyObject *);
$?}
/*$ $*/

/*$ MATRIX_FUNCTION $*/
$?{availableTo == 'all' or n in availableTo
PyDoc_STRVAR(glm_${p}mat${n}_function_${func}__doc__, "${func_doc}");
$?}
/*$ $*/

static
PyMethodDef glm_${p}mat${n}Methods[] = {
/*$ MATRIX_FUNCTION $*/
$?{availableTo == 'all' or n in availableTo
	{"${func}", (PyCFunction) glm_${p}mat${n}_function_${func}, ${'METH_NOARGS' if not args else 'METH_VARARGS'}, glm_${p}mat${n}_function_${func}__doc__},
$?}
/*$ $*/
	{NULL, NULL},
};

PyDoc_STRVAR(glm_${p}mat${n}Type__doc__, "A ${n if len(n) > 1 else n + 'x' + n} matrix.");

// ${p}mat${n} Object

static
PyTypeObject glm_${p}mat${n}Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"glm.${p}mat${n}",										/* tp_name */
	sizeof(glm_${p}mat${n}),								/* tp_basicsize */
	0,														/* tp_itemsize */
	0,														/* tp_dealloc */
	0,														/* tp_print */
	0,														/* tp_getattr */
	0,														/* tp_setattr */
	0,														/* tp_reserved */
	(reprfunc)glm_${p}mat${n}_tp_repr,						/* tp_repr */
	&glm_${p}mat${n}_NumberMethods,							/* tp_as_number */
	&glm_${p}mat${n}_SequenceMethods,						/* tp_as_sequence */
	0,														/* tp_as_mapping */
	(hashfunc)glm_${p}mat${n}_tp_hash,						/* tp_hash  */
	0,														/* tp_call */
	0,														/* tp_str */
	PyObject_GenericGetAttr,								/* tp_getattro */
	PyObject_GenericSetAttr,								/* tp_setattro */
	&glm_${p}mat${n}_BufferMethods,							/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	  Py_TPFLAGS_BASETYPE,									/* tp_flags */
	glm_${p}mat${n}Type__doc__,								/* tp_doc */
	0,														/* tp_traverse */
	0,														/* tp_clear */
	glm_${p}mat${n}_tp_richcompare,							/* tp_richcompare */
	0,														/* tp_weaklistoffset */
	0,														/* tp_iter */
	0,														/* tp_iternext */
	glm_${p}mat${n}Methods,									/* tp_methods */
	0,														/* tp_members */
	0,														/* tp_getset */
	0,														/* tp_base */
	0,														/* tp_dict */
	0,														/* tp_descr_get */
	0,														/* tp_descr_set */
	0,														/* tp_dictoffset */
	(initproc)glm_${p}mat${n}_tp_init,						/* tp_init */
	0,														/* tp_alloc */
	PyType_GenericNew,										/* tp_new */
};
/*$ $*/

/*$ VECTOR $*/
static
PyNumberMethods glm_${p}vec${n}_NumberMethods = {
	(binaryfunc)glm_${p}vec${n}_nb_add,
	(binaryfunc)glm_${p}vec${n}_nb_subtract,
	(binaryfunc)glm_${p}vec${n}_nb_multiply,
$?{type == 'int'
	NULL,
	NULL,
	NULL,
$??{type == 'float'
	(binaryfunc)glm_${p}vec${n}_nb_remainder,
	(binaryfunc)glm_${p}vec${n}_nb_divmod,
	(ternaryfunc)glm_${p}vec${n}_nb_power,
$?}
	(unaryfunc)glm_${p}vec${n}_nb_negative,
	(unaryfunc)glm_${p}vec${n}_nb_positive,
	(unaryfunc)glm_${p}vec${n}_nb_absolute,
	NULL,
$?{type == 'int'
	(unaryfunc)glm_${p}vec${n}_nb_invert,
	(binaryfunc)glm_${p}vec${n}_nb_lshift,
	(binaryfunc)glm_${p}vec${n}_nb_rshift,
	(binaryfunc)glm_${p}vec${n}_nb_and,
	(binaryfunc)glm_${p}vec${n}_nb_xor,
	(binaryfunc)glm_${p}vec${n}_nb_or,
$??{type == 'float'
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
$?}
	NULL,
	NULL,
	NULL,

	(binaryfunc)glm_${p}vec${n}_nb_inplace_add,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_subtract,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_multiply,
	NULL,
	NULL,
$?{type == 'int'
	(binaryfunc)glm_${p}vec${n}_nb_inplace_lshift,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_rshift,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_and,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_xor,
	(binaryfunc)glm_${p}vec${n}_nb_inplace_or,
$??{type == 'float'
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
$?}
$?{type == 'int'
	(binaryfunc)glm_${p}vec${n}_nb_true_divide,
$??{type == 'float'
	(binaryfunc)glm_${p}vec${n}_nb_floor_divide,
$?}
	(binaryfunc)glm_${p}vec${n}_nb_true_divide,
$?{type == 'int'
	(binaryfunc)glm_${p}vec${n}_nb_inplace_true_divide,
$??{type == 'float'
	(binaryfunc)glm_${p}vec${n}_nb_inplace_floor_divide,
$?}
	(binaryfunc)glm_${p}vec${n}_nb_inplace_true_divide,

	NULL,
};

static
PySequenceMethods glm_${p}vec${n}_SequenceMethods = {
	(lenfunc)glm_${p}vec${n}_sq_length,
	NULL,
	NULL,
	(ssizeargfunc)glm_${p}vec${n}_sq_item,
	NULL,
	(ssizeobjargproc)glm_${p}vec${n}_sq_ass_item,
	NULL,
	NULL,

	NULL,
	NULL,
};

static
PyBufferProcs glm_${p}vec${n}_BufferMethods = {
	(getbufferproc)glm_${p}vec${n}_bf_getbuffer,
	NULL
};

// ${p}vec${n} Methods

/*$ VECTOR_FUNCTION $*/

$?{args
    static PyObject* glm_${p}vec${n}_function_${func}(PyObject *self, PyObject *other) {
$??{
    static PyObject* glm_${p}vec${n}_function_${func}(PyObject *self) {
$?}

$?{args
/*$ {len(args)} $*/
	${'PyObject *' if isinstance(args[I], str) else args[I].__name__} argument${I};
/*$ $*/
	
	if(!PyArg_ParseTuple(other, "${''.join('f' if t == float else 'i' if t == int else 'O' for t in args)}:${func}"
/*$ {len(args)} $*/
	, &argument${I}
/*$ $*/
	))
		return NULL;
	
/*$ {len(args)} $*/
// $?{isinstance(args[I], str)
	// if(1 != PyObject_IsInstance(argument${I}, (PyObject *)&glm_${args[I]}Type)) {
		// std::stringstream ss;
		// ss << "Argument ${I + 1} must be of type '${'glm.' + args[I] if isinstance(args[I], str) else args[I].__name__}' not '" << Py_TYPE(argument${I})->tp_name << "'.";
		// std::string s = ss.str();
		// PyErr_SetString(PyExc_TypeError, s.c_str());
		// return NULL;
	// }
// $?}
/*$ $*/
$?}

$?{p == 'i'
    PyErr_SetString(PyExc_TypeError, "${func}() only supported for floating point vectors.");
    return NULL;
$??{
    $?{func == 'length'
        return PyFloat_FromDouble(static_cast<double>(glm::${func}(glm_${p}vec${n}Data(self))));
    $??{func == 'normalize'
        const glm::${p}vec${n} computed = glm::${func}(glm_${p}vec${n}Data(self));
        return glm_${p}vec${n}New(computed);
    $??{func == 'dot'
        return PyFloat_FromDouble(static_cast<double>(glm::${func}(glm_${p}vec${n}Data(self), glm_${p}vec${n}Data(argument0))));
    $??{func == 'cross' and n == 3
        PyObject *result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}vec${n} *)result)->vec = glm::${func}(glm_${p}vec${n}Data(self), glm_${p}vec${n}Data(argument0));
        return result;
    $??{
        PyErr_SetString(PyExc_TypeError, "${func}() not supported for these types.");
        return NULL;
    $?}
$?}
}
/*$ $*/

/*$ VECTOR_FUNCTION $*/
PyDoc_STRVAR(glm_${p}vec${n}_function_${func}__doc__, "${func_doc}");
/*$ $*/

static
PyMethodDef glm_${p}vec${n}Methods[] = {
/*$ VECTOR_FUNCTION $*/
	{"${func}", (PyCFunction) glm_${p}vec${n}_function_${func}, ${'METH_NOARGS' if not args else 'METH_VARARGS'}, glm_${p}vec${n}_function_${func}__doc__},
/*$ $*/
	{NULL, NULL},
};

PyDoc_STRVAR(glm_${p}vec${n}Type__doc__, "A ${n}D${' integer' if type == 'int' else ''} vector.");

// ${p}vec${n} Object

static
PyTypeObject glm_${p}vec${n}Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"glm.${p}vec${n}",										/* tp_name */
	sizeof(glm_${p}vec${n}),								/* tp_basicsize */
	0,														/* tp_itemsize */
	0,														/* tp_dealloc */
	0,														/* tp_print */
	0,														/* tp_getattr */
	0,														/* tp_setattr */
	0,														/* tp_reserved */
	(reprfunc)glm_${p}vec${n}_tp_repr,						/* tp_repr */
	&glm_${p}vec${n}_NumberMethods,							/* tp_as_number */
	&glm_${p}vec${n}_SequenceMethods,						/* tp_as_sequence */
	0,														/* tp_as_mapping */
	(hashfunc)glm_${p}vec${n}_tp_hash,						/* tp_hash  */
	0,														/* tp_call */
	0,														/* tp_str */
	(getattrofunc)glm_${p}vec${n}_tp_getattro,				/* tp_getattro */
	(setattrofunc)glm_${p}vec${n}_tp_setattro,				/* tp_setattro */
	&glm_${p}vec${n}_BufferMethods,							/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	  Py_TPFLAGS_BASETYPE,									/* tp_flags */
	glm_${p}vec${n}Type__doc__,								/* tp_doc */
	0,														/* tp_traverse */
	0,														/* tp_clear */
	glm_${p}vec${n}_tp_richcompare,							/* tp_richcompare */
	0,														/* tp_weaklistoffset */
	0,														/* tp_iter */
	0,														/* tp_iternext */
	glm_${p}vec${n}Methods,									/* tp_methods */
	0,														/* tp_members */
	0,														/* tp_getset */
	0,														/* tp_base */
	0,														/* tp_dict */
	0,														/* tp_descr_get */
	0,														/* tp_descr_set */
	0,														/* tp_dictoffset */
	(initproc)glm_${p}vec${n}_tp_init,						/* tp_init */
	0,														/* tp_alloc */
	PyType_GenericNew,										/* tp_new */
};
/*$ $*/
/*$ BASETYPEDEF $*/

/* * * ${type} Iterator * * */

static
PyObject * glm_${type}Iterator_tp_iternext(PyObject *self) {
	PyObject *result;
	glm_${type}Iterator *iter = (glm_${type}Iterator *)self;
	
	if(iter->obj == NULL) {
		PyErr_SetString(PyExc_TypeError, "${type.capitalize()} is invalid.");
		return NULL;
	}
	
	Py_ssize_t len = PyObject_Size(iter->obj);
	
	if(len < 0) {
		std::string s = "'";
		s += Py_TYPE(iter->obj)->tp_name;
		s += "' is not an iterable ${type.lower()}.";
		PyErr_SetString(PyExc_TypeError, s.c_str());
		return NULL;
	}
	
	if(iter->offset > len - 1)
		return NULL;
	
	result = PySequence_GetItem(iter->obj, iter->offset);
	
	if(result == NULL)
		return NULL;
	
	iter->offset += 1;
	
	return result;
}

static
PyObject * glm_${type}Iterator_tp_iter(PyObject *self) {
	PyObject *result;
	PyObject *args = Py_BuildValue("(O)", ((glm_${type}Iterator *)self)->obj);
	
	if(args == NULL)
		return NULL;
	
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), args);
	Py_DECREF(args);
	return result;
}

static
int glm_${type}Iterator_tp_init(PyObject *self, PyObject *args, PyObject *kwargs) {
	glm_${type}Iterator *real;
	PyObject *obj;
	
	real = (glm_${type}Iterator *)self;
	real->offset = 0;
	
	if(!PyArg_ParseTuple(args, "O:${type}Iterator", &obj)) {
		real->obj = NULL;
		return -1;
	}
	
	if(obj == NULL) {
		real->obj = NULL;
	}
	else {
		Py_INCREF(obj);
		real->obj = obj;
	}
	
	return 0;
}

static int
glm_${type}Iterator_tp_traverse(glm_${type}Iterator *self, visitproc visit, void *arg)
{
    Py_VISIT(self->obj);
    return 0;
}

static int
glm_${type}Iterator_tp_clear(glm_${type}Iterator *self)
{
    Py_CLEAR(self->obj);
    return 0;
}

static void
glm_${type}Iterator_tp_dealloc(glm_${type}Iterator* self)
{
    glm_${type}Iterator_tp_clear(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyDoc_STRVAR(glm_${type}Iterator__doc__, "A ${type} iterator.");

static
PyTypeObject glm_${type}IteratorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"glm.${type}Iterator",								/* tp_name */
	sizeof(glm_${type}Iterator),						/* tp_basicsize */
	0,													/* tp_itemsize */
	(destructor)glm_${type}Iterator_tp_dealloc,			/* tp_dealloc */
	0,													/* tp_print */
	0,													/* tp_getattr */
	0,													/* tp_setattr */
	0,													/* tp_reserved */
	0,													/* tp_repr */
	0,													/* tp_as_number */
	0,													/* tp_as_sequence */
	0,													/* tp_as_mapping */
	0,													/* tp_hash  */
	0,													/* tp_call */
	0,													/* tp_str */
	PyObject_GenericGetAttr,							/* tp_getattro */
	PyObject_GenericSetAttr,							/* tp_setattro */
	0,													/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,			/* tp_flags */
	glm_${type}Iterator__doc__,							/* tp_doc */
	(traverseproc)glm_${type}Iterator_tp_traverse,		/* tp_traverse */
	(inquiry)glm_${type}Iterator_tp_clear,				/* tp_clear */
	0,													/* tp_richcompare */
	0,													/* tp_weaklistoffset */
	(getiterfunc)glm_${type}Iterator_tp_iter,			/* tp_iter */
	(iternextfunc)glm_${type}Iterator_tp_iternext,		/* tp_iternext */
	0,													/* tp_methods */
	0,													/* tp_members */
	0,													/* tp_getset */
	0,													/* tp_base */
	0,													/* tp_dict */
	0,													/* tp_descr_get */
	0,													/* tp_descr_set */
	0,													/* tp_dictoffset */
	(initproc)glm_${type}Iterator_tp_init,				/* tp_init */
	0,													/* tp_alloc */
	PyType_GenericNew,									/* tp_new */
};
/*$ $*/

/*$ MATRIX $*/
/* * * ${p}mat${n} * * */

/* ${p}mat${n}: Numbers */

static
PyObject *glm_${p}mat${n}_nb_add(PyObject *self, PyObject *other) {
	PyObject *result;
	
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat + (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat + (float)PyFloat_AsDouble(other);
$?}
	}
$?{cols == rows
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = (int)PyLong_AsLong(self) + ((glm_${p}mat${n} *)other)->mat;
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = (float)PyFloat_AsDouble(self) + ((glm_${p}mat${n} *)other)->mat;
$?}
	}
$?}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat + ((glm_${p}mat${n} *)other)->mat;
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_subtract(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat - (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat - (float)PyFloat_AsDouble(other);
$?}
	}
$?{cols == rows
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = (int)PyLong_AsLong(self) - ((glm_${p}mat${n} *)other)->mat;
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = (float)PyFloat_AsDouble(self) - ((glm_${p}mat${n} *)other)->mat;
$?}
	}
$?}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat - ((glm_${p}mat${n} *)other)->mat;
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_multiply(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat * (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat * (float)PyFloat_AsDouble(other);
$?}
	}
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = (int)PyLong_AsLong(self) * ((glm_${p}mat${n} *)other)->mat;
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = (float)PyFloat_AsDouble(self) * ((glm_${p}mat${n} *)other)->mat;
$?}
	}
    else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${rows}Type)) {
        result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
		((glm_${p}vec${cols} *)result)->vec = ((glm_${p}mat${n} *)self)->mat * ((glm_${p}vec${rows} *)other)->vec;
    }
$?{cols == rows
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat * ((glm_${p}mat${n} *)other)->mat;
	}
$?}
	else {
$?{cols == rows
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
$??{
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
$?}
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_negative(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	((glm_${p}mat${n} *)result)->mat = -((glm_${p}mat${n} *)self)->mat;
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_positive(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_inplace_add(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
		((glm_${p}mat${n} *)self)->mat += (int)PyLong_AsLong(other);
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type))
		((glm_${p}mat${n} *)self)->mat += ((glm_${p}mat${n} *)other)->mat;
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	Py_INCREF(self);
	return self;
}

static
PyObject *glm_${p}mat${n}_nb_inplace_subtract(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
$?{type == 'int'
		((glm_${p}mat${n} *)self)->mat -= (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)self)->mat -= (float)PyFloat_AsDouble(other);
$?}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type))
		((glm_${p}mat${n} *)self)->mat -= ((glm_${p}mat${n} *)other)->mat;
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	Py_INCREF(self);
	return self;
}

static
PyObject *glm_${p}mat${n}_nb_inplace_multiply(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
$?{type == 'int'
		((glm_${p}mat${n} *)self)->mat *= (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)self)->mat *= (float)PyFloat_AsDouble(other);
$?}
$?{cols == rows
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type))
		((glm_${p}mat${n} *)self)->mat *= ((glm_${p}mat${n} *)other)->mat;
$?}
	else {
$?{cols == rows
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
$??{
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
$?}
		return NULL;
	}
	
	Py_INCREF(self);
	return self;
}

static
PyObject *glm_${p}mat${n}_nb_true_divide(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat / (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat / (float)PyFloat_AsDouble(other);
$?}
	}
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}mat${n} *)result)->mat = (int)PyLong_AsLong(self) / ((glm_${p}mat${n} *)other)->mat;
$??{type == 'float'
		((glm_${p}mat${n} *)result)->mat = (float)PyFloat_AsDouble(self) / ((glm_${p}mat${n} *)other)->mat;
$?}
	}
$?{cols == rows
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}mat${n} *)result)->mat = ((glm_${p}mat${n} *)self)->mat / ((glm_${p}mat${n} *)other)->mat;
	}
$?}
	else {
$?{cols == rows
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
$??{
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
$?}
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}mat${n}_nb_inplace_true_divide(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
$?{type == 'int'
		((glm_${p}mat${n} *)self)->mat /= (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}mat${n} *)self)->mat /= (float)PyFloat_AsDouble(other);
$?}
$?{cols == rows
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type))
		((glm_${p}mat${n} *)self)->mat /= ((glm_${p}mat${n} *)other)->mat;
$?}
	else {
$?{cols == rows
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
$??{
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
$?}
		return NULL;
	}
	
	Py_INCREF(self);
	return self;
}

/* ${p}mat${n}: Sequence */

static
Py_ssize_t glm_${p}mat${n}_sq_length(PyObject *self) {
	return ${cols * rows};
}

static
PyObject *glm_${p}mat${n}_sq_item(PyObject *self, glm::length_t item) {
	PyObject *result;
	if(item >= ${cols * rows}) {
		PyErr_SetString(PyExc_IndexError, "Out of range.");
		return NULL;
	}
	
$?{type == 'int'
	result = PyLong_FromLong((long)((glm_${p}mat${n} *)self)->mat[item/${rows}][item%${rows}]);
$??{type == 'float'
	result = PyFloat_FromDouble((double)((glm_${p}mat${n} *)self)->mat[item/${rows}][item%${rows}]);
$?}
	return result;
}

static
int glm_${p}mat${n}_sq_ass_item(PyObject *self, glm::length_t item, PyObject *value) {
	if(item >= ${cols * rows}) {
		PyErr_SetString(PyExc_IndexError, "Out of range.");
		return -1;
	}
	
	if(!PyNumber_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
		return -1;
	}
	
$?{type == 'int'
	((glm_${p}mat${n} *)self)->mat[item/${rows}][item%${rows}] = (int)PyLong_AsLong(value);
$??{type == 'float'
	((glm_${p}mat${n} *)self)->mat[item/${rows}][item%${rows}] = (float)PyFloat_AsDouble(value);
$?}
	
	return 0;
}

/* Definition */

static
PyObject * glm_${p}mat${n}_tp_repr(PyObject *self) {
	glm::${p}mat${n} *v = &((glm_${p}mat${n} *)self)->mat;
	std::stringstream s;
$?{type == 'float'
	s.precision(std::numeric_limits<float>::digits10);
$?}
	s << Py_TYPE(self)->tp_name << "("
/*$ {cols * rows} $*/
	${'<< ", "' if I > 0 else ''} << (*v)[${int(I/rows)}][${int(I%rows)}]
/*$ $*/
	<< ")";
	PyObject *result = PyUnicode_FromString(s.str().c_str());
	return result;
}

static
int glm_${p}mat${n}_tp_init(PyObject *self, PyObject *args, PyObject *kwargs) {
	if(1 != PyObject_IsInstance(self, (PyObject *)&glm_${p}mat${n}Type)) {
		PyErr_SetString(PyExc_TypeError, "Can only init ${p}mat${n}.");
		return -1;
	}
	
	glm_${p}mat${n} *real = (glm_${p}mat${n} *)self;
	
	Py_ssize_t argsize = PyTuple_GET_SIZE(args);
	
	if(argsize == 0) {
		real->mat = glm::${p}mat${n}();
		return 0;
	}
	
	if(argsize == 1) {
		PyObject *tmp;
		tmp = PyTuple_GET_ITEM(args, 0);
		
		if(PyNumber_Check(tmp)) {
$?{type == 'int'
			real->mat = glm::${p}mat${n}((int)PyLong_AsLong(tmp));
$??{type == 'float'
			real->mat = glm::${p}mat${n}((float)PyFloat_AsDouble(tmp));
$?}
			return 0;
		}
		
		if(PyBytes_CheckExact(tmp)) {
			memcpy(&real->mat, PyBytes_AsString(tmp), sizeof(${type}) * ${cols * rows});
			
			return 0;
		}
	}
    
$?{rows == cols
/*$ {(2, 3, 4)} $*/
$?{I >= int(n)
    if(PyObject_IsInstance(args, (PyObject *)&glm_${p}mat${I}Type) == 1) {
        real->mat = glm::${p}mat${n}(((glm_${p}mat${I} *)args)->mat);
        return 0;
    }
$?}
/*$ $*/
$?}

	real->mat = glm::${p}mat${n}();
	
	PyObject *tmp;
	glm::length_t i;
	glm::length_t at = 0;

	for(i = 0; i < argsize; i++) {
		tmp = PyTuple_GET_ITEM(args, i);

		if(PyBytes_CheckExact(tmp)) {
			glm::length_t len = static_cast<glm::length_t>(PyBytes_Size(tmp)) / sizeof(${type});
			
			if(at + len > ${cols * rows - 1})
				len = ${cols * rows} - at;
			
			memcpy(glm::value_ptr(real->mat) + at, PyBytes_AsString(tmp), len * sizeof(${type}));
			
			at += len;
		}
		else if(PyIter_Check(tmp) || Py_TYPE(tmp)->tp_iter) {
			PyObject *tmpi = PyObject_GetIter(tmp);
			PyObject *item;
	
			while(1) {
				item = PyIter_Next(tmpi);
				if(item == NULL)
					break;
		
$?{type == 'int'
				real->mat[at/${rows}][at%${rows}] = (int)PyLong_AsLong(item);
$??{type == 'float'
				real->mat[at/${rows}][at%${rows}] = (float)PyFloat_AsDouble(item);
$?}
		
				Py_DECREF(item);
				at += 1;
			}
	
			Py_DECREF(tmpi);
		}
		else if(PyNumber_Check(tmp)) {
$?{type == 'int'
			real->mat[at/${rows}][at%${rows}] = (int)PyLong_AsLong(tmp);
$??{type == 'float'
			real->mat[at/${rows}][at%${rows}] = (float)PyFloat_AsDouble(tmp);
$?}

			at += 1;
		}
		else {
			PyErr_Format(PyExc_TypeError, "Initialization only accepts numbers, sequences, and bytes as arguments, not '%s' at argument %i.", Py_TYPE(tmp)->tp_name, (int)at);
			return -1;
		}
		
		if(at > ${cols * rows}) {
			PyErr_SetString(PyExc_TypeError, "Too much data.");
			return -1;
		}
	}
	
	return 0;
}

int glm_${p}mat${n}_bf_getbuffer(PyObject *self, Py_buffer *view, int flags) {
    if (view == NULL) {
        PyErr_SetString(PyExc_ValueError, "NULL view in glm_${p}mat${n}_bf_getbuffer");
        return -1;
    }

    void *buf = (void*)glm::value_ptr(((glm_${p}mat${n} *)self)->mat);
    const int ret = PyBuffer_FillInfo(view, self, buf, sizeof(${type}) * ${rows * cols}, 0, flags);
    
    if(ret < 0) {
        PyErr_SetString(PyExc_BufferError, "Buffer error in glm_${p}mat${n}_bf_getbuffer");
        view->obj = NULL;
        return -1;
    }
    
    const Py_ssize_t shape = ${rows * cols};
    const Py_ssize_t strides = sizeof(${type});

    view->obj = self;
    view->ndim = 1;
    view->len = shape * sizeof(${type});
    view->shape = constants_array+shape;
    view->strides = constants_array+strides;
    view->itemsize = sizeof(${type});
$?{type == 'float'
    view->format = "f";
$??{type == 'int'
    view->format = "i";
$?}
    return 0;
}

PyObject *glm_${p}mat${n}_tp_richcompare(PyObject *self, PyObject *other, int op) {
	switch(op) {
/*$ {(('EQ', '=='), ('NE', '!='))} $*/
		case Py_${I[0]}:
			if(1 != PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
				if((PyIter_Check(other) || Py_TYPE(other)->tp_iter) && PyObject_Size(self) == PyObject_Size(other)) {
					PyObject *iter = PyObject_GetIter(self);
					PyObject *itero = PyObject_GetIter(other);
					PyObject *iteritem;
					PyObject *iteritemo;
					
					if(iter != NULL && itero != NULL) {
						while(true) {
							iteritem = PyIter_Next(iter);
							iteritemo = PyIter_Next(itero);
						
							if(iteritem == NULL && iteritemo == NULL) {
								Py_INCREF(Py_True);
								return Py_True;
							}
						
							if(iteritem == NULL || iteritemo == NULL || 1 != PyObject_RichCompareBool(iteritem, iteritemo, Py_${I[0]}))
								break;
							
							Py_DECREF(iteritem);
							Py_DECREF(iteritemo);
						}
					}
					
					Py_XDECREF(iter);
					Py_XDECREF(itero);
				}
				
				Py_INCREF(Py_False);
				return Py_False;
			}
			
			if(1 != PyObject_IsInstance(other, (PyObject *)&glm_${p}mat${n}Type)) {
				Py_INCREF(Py_False);
				return Py_False;
			}
			
			if(((glm_${p}mat${n} *)self)->mat ${I[1]} ((glm_${p}mat${n} *)other)->mat) {
				Py_INCREF(Py_True);
				return Py_True;
			}
			
			Py_INCREF(Py_False);
			return Py_False;
/*$ $*/
		default:
			PyErr_SetString(PyExc_TypeError, "Comparison not valid.");
			return NULL;
	}
}

Py_hash_t glm_${p}mat${n}_tp_hash(PyObject *self) {
	Py_hash_t x;

$?{type == 'int'
	x = (Py_hash_t) ((glm_${p}mat${n} *)self)->mat[0][0];
/*$ {range(1, cols * rows)} $*/
	x ^= (Py_hash_t) ((glm_${p}mat${n} *)self)->mat[${int(I/rows)}][${int(I%rows)}] << ${int(I / (cols * rows) * 31)};
/*$ $*/
$??{
	x = _Py_HashDouble((double)((glm_${p}mat${n} *)self)->mat[0][0]);
/*$ {range(1, cols * rows)} $*/
	x ^= _Py_HashDouble((double)((glm_${p}mat${n} *)self)->mat[${int(I/rows)}][${int(I%rows)}]) << ${int(I / (cols * rows) * 31)};
/*$ $*/
$?}
	
	if(x == -1)
		x = -2;
	
	return x;
}

/*$ $*/
/*$ VECTOR $*/
/* * * ${p}vec${n} * * */

/* ${p}vec${n}: Numbers */

/*$ VECTOR_MATH $*/
$?{not only or type == only or type in only
static
PyObject *glm_${p}vec${n}_nb_${f}(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = ((glm_${p}vec${n} *)self)->vec ${s} (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = ((glm_${p}vec${n} *)self)->vec ${s} (float)PyFloat_AsDouble(other);
$?}
	}
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = (int)PyLong_AsLong(self) ${s} ((glm_${p}vec${n} *)other)->vec;
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = (float)PyFloat_AsDouble(self) ${s} ((glm_${p}vec${n} *)other)->vec;
$?}
	}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}vec${n} *)result)->vec = ((glm_${p}vec${n} *)self)->vec ${s} ((glm_${p}vec${n} *)other)->vec;
	}
$?{not only
	else if(PyIter_Check(other) || Py_TYPE(other)->tp_iter) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		
		PyObject *convert;
		PyObject *args = Py_BuildValue("(O)", other);
		convert = PyObject_CallObject((PyObject *)Py_TYPE(self), args);
		Py_DECREF(args);
		
		((glm_${p}vec${n} *)result)->vec = ((glm_${p}vec${n} *)self)->vec ${s} ((glm_${p}vec${n} *)convert)->vec;
		
		Py_DECREF(convert);
	}
$?}
	else {
		PyErr_SetString(PyExc_TypeError, "${'Must be a number or of the same type.' if only else 'Must be a number, the same type, or an iterable.'}");
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}vec${n}_nb_inplace_${f}(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
$?{type == 'int'
		((glm_${p}vec${n} *)self)->vec ${s}= (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}vec${n} *)self)->vec ${s}= (float)PyFloat_AsDouble(other);
$?}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type))
		((glm_${p}vec${n} *)self)->vec ${s}= ((glm_${p}vec${n} *)other)->vec;
$?{not only
	else if(PyIter_Check(other) || Py_TYPE(other)->tp_iter) {
		PyObject *convert;
		PyObject *args = Py_BuildValue("(O)", other);
		convert = PyObject_CallObject((PyObject *)Py_TYPE(self), args);
		Py_DECREF(args);
		
		((glm_${p}vec${n} *)self)->vec ${s}= ((glm_${p}vec${n} *)convert)->vec;
		
		Py_DECREF(convert);
	}
$?}
	else {
		PyErr_SetString(PyExc_TypeError, "${'Must be a number or of the same type.' if only else 'Must be a number, the same type, or an iterable.'}");
		return NULL;
	}
	
	Py_INCREF(self);
	return self;
}

$?}
/*$ $*/

$?{type == 'float'
static
PyObject *glm_${p}vec${n}_nb_remainder(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = glm::mod(((glm_${p}vec${n} *)self)->vec, (int)PyLong_AsLong(other));
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = glm::mod(((glm_${p}vec${n} *)self)->vec, (float)PyFloat_AsDouble(other));
$?}
	}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}vec${n} *)result)->vec = glm::mod(((glm_${p}vec${n} *)self)->vec, ((glm_${p}vec${n} *)other)->vec);
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	return result;
}

static
PyObject *glm_${p}vec${n}_nb_divmod(PyObject *self, PyObject *other) {
	if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		PyObject *whole = PyNumber_FloorDivide(self, other);
	
		if(whole == NULL)
			return NULL;
		
		PyObject *args = Py_BuildValue("(O)", self);
		PyObject *tmp = Py_TYPE(other)->tp_new(Py_TYPE(other), args, NULL);
		Py_DECREF(args);
		PyObject *remain = PyNumber_Remainder(tmp, other);
		Py_DECREF(tmp);
		
		if(remain == NULL) {
			Py_DECREF(whole);
			return NULL;
		}
		
		PyObject *result = Py_BuildValue("(OO)", whole, remain);
		
		Py_DECREF(whole);
		Py_DECREF(remain);
		
		return result;
	}
	else if(!PyNumber_Check(other) && 1 != PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		PyErr_SetString(PyExc_TypeError, "Other must be a number or of the same type.");
		return NULL;
	}
	
	PyObject *whole = PyNumber_FloorDivide(self, other);
	
	if(whole == NULL)
		return NULL;
	
	PyObject *remain = PyNumber_Remainder(self, other);
	
	if(remain == NULL) {
		Py_DECREF(whole);
		return NULL;
	}
	
	PyObject *result = Py_BuildValue("(OO)", whole, remain);
	
	Py_DECREF(whole);
	Py_DECREF(remain);
	
	return result;
}

static
PyObject *glm_${p}vec${n}_nb_power(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);;
		
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = glm::pow(((glm_${p}vec${n} *)self)->vec, glm::${p}vec${n}((int)PyLong_AsLong(other)));
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = glm::pow(((glm_${p}vec${n} *)self)->vec, glm::${p}vec${n}((float)PyFloat_AsDouble(other)));
$?}
	}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}vec${n} *)result)->vec = glm::pow(((glm_${p}vec${n} *)self)->vec, ((glm_${p}vec${n} *)other)->vec);
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	return result;
}
$?}

static
PyObject *glm_${p}vec${n}_nb_negative(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	((glm_${p}vec${n} *)result)->vec = -((glm_${p}vec${n} *)self)->vec;
	return result;
}

static
PyObject *glm_${p}vec${n}_nb_positive(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	return result;
}

static
PyObject *glm_${p}vec${n}_nb_absolute(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	((glm_${p}vec${n} *)result)->vec = glm::abs(((glm_${p}vec${n} *)self)->vec);
	return result;
}

$?{type == 'int'
static
PyObject *glm_${p}vec${n}_nb_invert(PyObject *self) {
	PyObject *result;
	result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
	((glm_${p}vec${n} *)result)->vec = ~((glm_${p}vec${n} *)self)->vec;
	return result;
}
$?}

$?{type == 'float'
static
PyObject *glm_${p}vec${n}_nb_floor_divide(PyObject *self, PyObject *other) {
	PyObject *result;
	if(PyNumber_Check(other)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = glm::floor(((glm_${p}vec${n} *)self)->vec / (int)PyLong_AsLong(other));
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = glm::floor(((glm_${p}vec${n} *)self)->vec / (float)PyFloat_AsDouble(other));
$?}
	}
	else if(PyNumber_Check(self) && 1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(other), NULL);
$?{type == 'int'
		((glm_${p}vec${n} *)result)->vec = glm::floor((int)PyLong_AsLong(self) / ((glm_${p}vec${n} *)other)->vec);
$??{type == 'float'
		((glm_${p}vec${n} *)result)->vec = glm::floor((float)PyFloat_AsDouble(self) / ((glm_${p}vec${n} *)other)->vec);
$?}
	}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
		result = PyObject_CallObject((PyObject *)Py_TYPE(self), NULL);
		((glm_${p}vec${n} *)result)->vec = glm::floor(((glm_${p}vec${n} *)self)->vec / ((glm_${p}vec${n} *)other)->vec);
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	return result;
}
$?}

$?{type == 'float'
static
PyObject *glm_${p}vec${n}_nb_inplace_floor_divide(PyObject *self, PyObject *other) {
	if(PyNumber_Check(other))
$?{type == 'int'
		((glm_${p}vec${n} *)self)->vec /= (int)PyLong_AsLong(other);
$??{type == 'float'
		((glm_${p}vec${n} *)self)->vec /= (float)PyFloat_AsDouble(other);
$?}
	else if(1 == PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type))
		((glm_${p}vec${n} *)self)->vec /= ((glm_${p}vec${n} *)other)->vec;
	else {
		PyErr_SetString(PyExc_TypeError, "Must be a number or of the same type.");
		return NULL;
	}
	
	((glm_${p}vec${n} *)self)->vec = glm::floor(((glm_${p}vec${n} *)self)->vec);
	
	Py_INCREF(self);
	return self;
}
$?}

/* ${p}vec${n}: Sequence */

static
Py_ssize_t glm_${p}vec${n}_sq_length(PyObject *self) {
	return ${n};
}

static
PyObject *glm_${p}vec${n}_sq_item(PyObject *self, glm::length_t item) {
	PyObject *result;
	if(item >= ${n}) {
		PyErr_SetString(PyExc_IndexError, "Out of range.");
		return NULL;
	}
	
$?{type == 'int'
	result = PyLong_FromLong((long)((glm_${p}vec${n} *)self)->vec[item]);
$??{type == 'float'
	result = PyFloat_FromDouble((double)((glm_${p}vec${n} *)self)->vec[item]);
$?}
	return result;
}

static
int glm_${p}vec${n}_sq_ass_item(PyObject *self, glm::length_t item, PyObject *value) {
	if(item >= ${n}) {
		PyErr_SetString(PyExc_IndexError, "Out of range.");
		return -1;
	}
	
	if(!PyNumber_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Must be a number.");
		return -1;
	}
	
$?{type == 'int'
	((glm_${p}vec${n} *)self)->vec[item] = (int)PyLong_AsLong(value);
$??{type == 'float'
	((glm_${p}vec${n} *)self)->vec[item] = (float)PyFloat_AsDouble(value);
$?}
	
	return 0;
}

/* Definition */

static
PyObject * glm_${p}vec${n}_tp_repr(PyObject *self) {
	glm::${p}vec${n} *v = &((glm_${p}vec${n} *)self)->vec;
	std::stringstream s;
$?{type == 'float'
	s.precision(std::numeric_limits<float>::digits10);
$?}
	s << Py_TYPE(self)->tp_name << "("
/*$ {n} $*/
	${'<< ", "' if I > 0 else ''} << (*v)[${I}]
/*$ $*/
	<< ")";
	PyObject *result = PyUnicode_FromString(s.str().c_str());
	return result;
}

static
int glm_${p}vec${n}_tp_init(PyObject *self, PyObject *args, PyObject *kwargs) {
	if(1 != PyObject_IsInstance(self, (PyObject *)&glm_${p}vec${n}Type)) {
		PyErr_SetString(PyExc_TypeError, "Can only init ${p}vec${n}.");
		return -1;
	}
	
	glm_${p}vec${n} *real = (glm_${p}vec${n} *)self;
	
	Py_ssize_t argsize = PyTuple_GET_SIZE(args);
	
	if(argsize == 0) {
		real->vec = glm::${p}vec${n}();
		return 0;
	}
	
	if(argsize == 1) {
		PyObject *tmp;
		tmp = PyTuple_GET_ITEM(args, 0);
		
		if(PyNumber_Check(tmp)) {
$?{type == 'int'
			real->vec = glm::${p}vec${n}((int)PyLong_AsLong(tmp));
$??{type == 'float'
			real->vec = glm::${p}vec${n}((float)PyFloat_AsDouble(tmp));
$?}
			return 0;
		}
		
		if(PyBytes_CheckExact(tmp)) {
			memcpy(&real->vec, PyBytes_AsString(tmp), sizeof(${type}) * ${n});
			
			return 0;
		}
	}
	
	real->vec = glm::${p}vec${n}();
	
	PyObject *tmp;
	glm::length_t i;
	glm::length_t at = 0;

	for(i = 0; i < argsize; i++) {
		tmp = PyTuple_GET_ITEM(args, i);
		
		if(PyBytes_CheckExact(tmp)) {
			glm::length_t len = static_cast<glm::length_t>(PyBytes_Size(tmp)) / sizeof(${type});
			
			if(at + len > ${n - 1})
				len = ${n} - at;
			
			memcpy(glm::value_ptr(real->vec) + at, PyBytes_AsString(tmp), len * sizeof(${type}));
			
			at += len;
		}
		else if(PyIter_Check(tmp) || Py_TYPE(tmp)->tp_iter) {
			PyObject *tmpi = PyObject_GetIter(tmp);
			PyObject *item;
	
			while(1) {
				item = PyIter_Next(tmpi);
				if(item == NULL)
					break;
		
$?{type == 'int'
				real->vec[at] = (int)PyLong_AsLong(item);
$??{type == 'float'
				real->vec[at] = (float)PyFloat_AsDouble(item);
$?}
		
				Py_DECREF(item);
				
				at += 1;
			}
	
			Py_DECREF(tmpi);
		}
		else if(PyNumber_Check(tmp)) {
$?{type == 'int'
			real->vec[at] = (int)PyLong_AsLong(tmp);
$??{type == 'float'
			real->vec[at] = (float)PyFloat_AsDouble(tmp);
$?}

			at += 1;
		}
		else {
			PyErr_Format(PyExc_TypeError, "Initialization only accepts numbers, sequences, and bytes as arguments, not '%s' at argument %i.", Py_TYPE(tmp)->tp_name, (int)at);
			return -1;
		}
		
		if(at > ${n}) {
			PyErr_SetString(PyExc_TypeError, "Too much data.");
			return -1;
		}
	}
	
	return 0;
}

static
PyObject * glm_${p}vec${n}_tp_getattro(PyObject *self, PyObject *attr_name) {
	PyObject *tmp;
	tmp = PyObject_GenericGetAttr(self, attr_name);
	
	if(tmp != NULL)
		return tmp;
	else
		PyErr_Clear();
	
	if(!PyUnicode_Check(attr_name)) {
		PyErr_SetString(PyExc_TypeError, "Attributes use strings.");
		return NULL;
	}
	
	Py_ssize_t swizzles = PyObject_Size(attr_name);
	
	if(swizzles < 1) {
		PyErr_SetString(PyExc_TypeError, "Attributes are strings with a commendable length.");
		return NULL;
	}
	
	PyObject * result;
	PyObject * args;
	Py_ssize_t c;
	
	args = PyTuple_New(swizzles);
	
	if(args == NULL) {
		PyErr_SetString(PyExc_ValueError, "Could not create a simple tuple...hmmm...");
		return NULL;
	}
	
	for(c = 0; c < swizzles; c++) {
		tmp = PySequence_ITEM(attr_name, c);
/*$ {((0, 'x', 's', 'r'), (1, 'y', 't', 'g'), (2, 'z', 'p', 'b'), (3, 'w', 'q', 'a'))[:n]} $*/
		${'' if I[0] == 0 else 'else '}if(
			PyUnicode_CompareWithASCIIString(tmp, "${I[1]}") == 0 ||
			PyUnicode_CompareWithASCIIString(tmp, "${I[2]}") == 0 ||
			PyUnicode_CompareWithASCIIString(tmp, "${I[3]}") == 0 )
$?{type == 'int'
			PyTuple_SET_ITEM(args, c, PyLong_FromLong((long)((glm_${p}vec${n} *)self)->vec[${I[0]}]));
$??{type == 'float'
			PyTuple_SET_ITEM(args, c, PyFloat_FromDouble((double)((glm_${p}vec${n} *)self)->vec[${I[0]}]));
$?}
/*$ $*/
		else {
			Py_DECREF(tmp);
			Py_DECREF(args);
			PyErr_SetString(PyExc_AttributeError, "Swizzle out of range.");
			return NULL;
		}
		
		Py_DECREF(tmp);
	}
	
	if(swizzles == 1) {
		result = PyTuple_GET_ITEM(args, 0);
		Py_INCREF(result);
	}
	else if(swizzles == 2)
		result = PyObject_CallObject((PyObject *)&glm_${p}vec2Type, args);
	else if(swizzles == 3)
		result = PyObject_CallObject((PyObject *)&glm_${p}vec3Type, args);
	else if(swizzles == 4)
		result = PyObject_CallObject((PyObject *)&glm_${p}vec4Type, args);
	else
		return args;
	
	Py_DECREF(args);
	
	if(result == NULL)
		PyErr_SetString(PyExc_ValueError, "Could not create vector.");
	
	return result;
}

static
int glm_${p}vec${n}_tp_setattro(PyObject *self, PyObject *attr_name, PyObject *value) {
	if(!PyUnicode_Check(attr_name)) {
		PyErr_SetString(PyExc_TypeError, "Attributes use strings.");
		return -1;
	}
	
	Py_ssize_t swizzles = PyObject_Size(attr_name);
	
	Py_ssize_t c;
	PyObject *tmp;
	
	if(PyNumber_Check(value)) {
		for(c = 0; c < swizzles; c++) {
			tmp = PySequence_ITEM(attr_name, c);
/*$ {((0, 'x', 's', 'r'), (1, 'y', 't', 'g'), (2, 'z', 'p', 'b'), (3, 'w', 'q', 'a'))[:n]} $*/
			${'' if I[0] == 0 else 'else '}if(
				PyUnicode_CompareWithASCIIString(tmp, "${I[1]}") == 0 ||
				PyUnicode_CompareWithASCIIString(tmp, "${I[2]}") == 0 ||
				PyUnicode_CompareWithASCIIString(tmp, "${I[3]}") == 0 )
$?{type == 'int'
				((glm_${p}vec${n} *)self)->vec[${I[0]}] = (int)PyLong_AsLong(value);
$??{type == 'float'
				((glm_${p}vec${n} *)self)->vec[${I[0]}] = (float)PyFloat_AsDouble(value);
$?}
/*$ $*/
			else {
				Py_DECREF(tmp);
				PyErr_SetString(PyExc_AttributeError, "Swizzle out of range.");
				return -1;
			}
		
			Py_DECREF(tmp);
		}
	}
	else if(PyIter_Check(value) || Py_TYPE(value)->tp_iter) {
		PyObject *iter = PyObject_GetIter(value);
		PyObject *iteritem;
		
		if(iter == NULL)
			return -1;
		
		for(c = 0; c < swizzles; c++) {
			tmp = PySequence_ITEM(attr_name, c);
			iteritem = PyIter_Next(iter);
			
			if(iteritem == NULL) {
				Py_DECREF(tmp);
				Py_DECREF(iter);
				PyErr_SetString(PyExc_AttributeError, "Too few values.");
				return -1;
			}
			else if(!PyNumber_Check(iteritem)) {
				Py_DECREF(tmp);
				Py_DECREF(iter);
				Py_DECREF(iteritem);
				PyErr_SetString(PyExc_AttributeError, "Values must all be numbers.");
				return -1;
			}
/*$ {((0, 'x', 's', 'r'), (1, 'y', 't', 'g'), (2, 'z', 'p', 'b'), (3, 'w', 'q', 'a'))[:n]} $*/
			${'else ' if I[0] > 0 else ''}if(
				PyUnicode_CompareWithASCIIString(tmp, "${I[1]}") == 0 ||
				PyUnicode_CompareWithASCIIString(tmp, "${I[2]}") == 0 ||
				PyUnicode_CompareWithASCIIString(tmp, "${I[3]}") == 0 )
$?{type == 'int'
				((glm_${p}vec${n} *)self)->vec[${I[0]}] = (int)PyLong_AsLong(iteritem);
$??{type == 'float'
				((glm_${p}vec${n} *)self)->vec[${I[0]}] = (float)PyFloat_AsDouble(iteritem);
$?}
/*$ $*/
			else {
				Py_DECREF(tmp);
				Py_DECREF(iter);
				Py_DECREF(iteritem);
				PyErr_SetString(PyExc_AttributeError, "Swizzle out of range.");
				return -1;
			}
			
			Py_DECREF(iteritem);
			Py_DECREF(tmp);
		}
		
		Py_DECREF(iter);
	}
	else {
		PyErr_SetString(PyExc_AttributeError, "Value must be a number or iterable.");
		return -1;
	}
	
	return 0;
}

int glm_${p}vec${n}_bf_getbuffer(PyObject *self, Py_buffer *view, int flags) {
    if (view == NULL) {
        PyErr_SetString(PyExc_ValueError, "NULL view in glm_${p}vec${n}_bf_getbuffer");
        return -1;
    }

    void *buf = (void*)glm::value_ptr(((glm_${p}vec${n} *)self)->vec);
    const int ret = PyBuffer_FillInfo(view, self, buf, sizeof(${type}) * ${n}, 0, flags);
    
    if(ret < 0) {
        PyErr_SetString(PyExc_BufferError, "Buffer error in glm_${p}vec${n}_bf_getbuffer");
        view->obj = NULL;
        return -1;
    }
    
    const Py_ssize_t shape = ${n};
    const Py_ssize_t strides = sizeof(${type});

    view->obj = self;
    view->ndim = 1;
    view->len = shape * sizeof(${type});
    view->shape = constants_array+shape;
    view->strides = constants_array+strides;
    view->itemsize = sizeof(${type});
$?{type == 'float'
    view->format = "f";
$??{type == 'int'
    view->format = "i";
$?}
    return 0;
}

PyObject *glm_${p}vec${n}_tp_richcompare(PyObject *self, PyObject *other, int op) {
	switch(op) {
/*$ {(('EQ', '=='), ('NE', '!='))} $*/
		case Py_${I[0]}:
			if(1 != PyObject_IsInstance(other, (PyObject *)&glm_${p}vec${n}Type)) {
				if((PyIter_Check(other) || Py_TYPE(other)->tp_iter) && PyObject_Size(self) == PyObject_Size(other)) {
					PyObject *iter = PyObject_GetIter(self);
					PyObject *itero = PyObject_GetIter(other);
					PyObject *iteritem;
					PyObject *iteritemo;
					
					if(iter != NULL && itero != NULL) {
						while(true) {
							iteritem = PyIter_Next(iter);
							iteritemo = PyIter_Next(itero);
						
							if(iteritem == NULL && iteritemo == NULL) {
								Py_INCREF(Py_True);
								return Py_True;
							}
						
							if(iteritem == NULL || iteritemo == NULL || 1 != PyObject_RichCompareBool(iteritem, iteritemo, Py_${I[0]}))
								break;
							
							Py_DECREF(iteritem);
							Py_DECREF(iteritemo);
						}
					}
					
					Py_XDECREF(iter);
					Py_XDECREF(itero);
				}
				
				Py_INCREF(Py_False);
				return Py_False;
			}
			
			if(((glm_${p}vec${n} *)self)->vec ${I[1]} ((glm_${p}vec${n} *)other)->vec) {
				Py_INCREF(Py_True);
				return Py_True;
			}
			
			Py_INCREF(Py_False);
			return Py_False;
/*$ $*/
		default:
			PyErr_SetString(PyExc_TypeError, "Comparison not valid.");
			return NULL;
	}
}

Py_hash_t glm_${p}vec${n}_tp_hash(PyObject *self) {
	Py_hash_t x;

$?{type == 'int'
	x = (Py_hash_t) ((glm_${p}vec${n} *)self)->vec[0];
/*$ {range(1, n)} $*/
	x ^= (Py_hash_t) ((glm_${p}vec${n} *)self)->vec[${I}] << ${int(I / n * 31)};
/*$ $*/
$??{
	x = _Py_HashDouble((double)((glm_${p}vec${n} *)self)->vec[0]);
/*$ {range(1, n)} $*/
	x ^= _Py_HashDouble((double)((glm_${p}vec${n} *)self)->vec[${I}]) << ${int(I / n * 31)};
/*$ $*/
$?}
	
	if(x == -1)
		x = -2;
	
	return x;
}
/*$ $*/

/* * * New * * */

/*$ MATRIX $*/
PyObject *glm_${p}mat${n}New(glm::${p}mat${n} m) {
	glm_${p}mat${n} *result = PyObject_New(glm_${p}mat${n}, &glm_${p}mat${n}Type);
	result->mat = m;
	return (PyObject *)result;
}
/*$ $*/
/*$ VECTOR $*/
PyObject *glm_${p}vec${n}New(glm::${p}vec${n} v) {
	glm_${p}vec${n} *result = PyObject_New(glm_${p}vec${n}, &glm_${p}vec${n}Type);
	result->vec = v;
	return (PyObject *)result;
}
/*$ $*/

/*$ BASETYPEDEF $*/
/* * * ${type} * * */

static
PyObject *glm_${type}_tp_iter(PyObject *self) {
	PyObject *result;
	PyObject *args;
	args = Py_BuildValue("(O)", self);
	
	if(args == NULL) {
		PyErr_SetString(PyExc_SystemError, "Invalid self.");
		return (PyObject *) NULL;
	}
	
	result = PyObject_CallObject((PyObject *)&glm_${type}IteratorType, args);
	
	Py_DECREF(args);
	return result;
}

/* ${type} Definition */

PyDoc_STRVAR(glm_${type}Type__doc__, "${doc}");

static
PyTypeObject glm_${type}Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"glm.${type}",										/* tp_name */
	sizeof(glm_${type}),								/* tp_basicsize */
	0,													/* tp_itemsize */
	0,													/* tp_dealloc */
	0,													/* tp_print */
	0,													/* tp_getattr */
	0,													/* tp_setattr */
	0,													/* tp_reserved */
	0,													/* tp_repr */
	0,													/* tp_as_number */
	0,													/* tp_as_sequence */
	0,													/* tp_as_mapping */
	0,													/* tp_hash  */
	0,													/* tp_call */
	0,													/* tp_str */
	PyObject_GenericGetAttr,							/* tp_getattro */
	PyObject_GenericSetAttr,							/* tp_setattro */
	0,													/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
	  Py_TPFLAGS_BASETYPE,								/* tp_flags */
	glm_${type}Type__doc__,								/* tp_doc */
	0,													/* tp_traverse */
	0,													/* tp_clear */
	0,													/* tp_richcompare */
	0,													/* tp_weaklistoffset */
	(getiterfunc)glm_${type}_tp_iter,					/* tp_iter */
	0,													/* tp_iternext */
	0,													/* tp_methods */
	0,													/* tp_members */
	0,													/* tp_getset */
	0,													/* tp_base */
	0,													/* tp_dict */
	0,													/* tp_descr_get */
	0,													/* tp_descr_set */
	0,													/* tp_dictoffset */
	0,													/* tp_init */
	0,													/* tp_alloc */
	PyType_GenericNew,									/* tp_new */
};
/*$ $*/

/* * * Matrix Functions * * */

/*$ MATRIX $*/
/*$ MATRIX_FUNCTION $*/
$?{availableTo == 'all' or n in availableTo
static
PyObject *glm_${p}mat${n}_function_${func}(PyObject *self, PyObject *args) {
$?{args
/*$ {len(args)} $*/
	${'PyObject *' if isinstance(args[I], str) else args[I].__name__} argument${I};
/*$ $*/
	
	if(!PyArg_ParseTuple(args, "${''.join('f' if t == float else 'i' if t == int else 'O' for t in args)}:${func}"
/*$ {len(args)} $*/
	, &argument${I}
/*$ $*/
	))
		return NULL;
	
/*$ {len(args)} $*/
$?{isinstance(args[I], str)
	if(1 != PyObject_IsInstance(argument${I}, (PyObject *)&glm_${args[I]}Type)) {
		std::stringstream ss;
		ss << "Argument ${I + 1} must be of type '${'glm.' + args[I] if isinstance(args[I], str) else args[I].__name__}' not '" << Py_TYPE(argument${I})->tp_name << "'.";
		std::string s = ss.str();
		PyErr_SetString(PyExc_TypeError, s.c_str());
		return NULL;
	}
$?}
/*$ $*/
$?}

$?{func == 'mat3'
	glm::${p}mat3 computed;
$??{
    glm::${p}mat${n} computed;
$?}
	PyObject *result;
	computed = glm::${func}(glm_${p}mat${n}Data(self)
/*$ {len(args)} $*/
$?{isinstance(args[I], str)
	, glm_${args[I]}Data(argument${I})
$??{
	, argument${I}
$?}
/*$ $*/
	);
    
$?{func == 'mat3'
	result = glm_${p}mat3New(computed);
$??{
    result = glm_${p}mat${n}New(computed);
$?}
	return result;
}
$?}
/*$ $*/
/*$ $*/

/* * * Functions * * */

/*$ NUMBER_FUNCTION $*/
PyObject *glm_function_${func}(PyObject *module, PyObject *args) {
/*$ {argc + argoc} $*/
$?{type == 'float'
	${type} a${I};
$??{
    PyObject *a${I};
$?}
/*$ $*/

	if(!PyArg_ParseTuple(args, "${p * argc}|${p * argoc}:${func}",
/*$ {argc + argoc} $*/
	&a${I}${', ' if I + 1 < argc + argoc else ''}
/*$ $*/
	))
		return NULL;
        
    $?{returns == 'float'
        return PyFloat_FromDouble(static_cast<double>(glm::${func_glm_name}(
        /*$ {argc + argoc} $*/
            $?{type == 'float'
                a${I}${', ' if I + 1 < argc + argoc else ''}
            $??{
                glm_vec2Data(a${I})${', ' if I + 1 < argc + argoc else ''}
            $?}
                
        /*$ $*/
        )));
    $??{
        PyObject *result = PyObject_CallObject((PyObject *)&glm_${returns}Type, NULL);
        ((glm_${returns} *)result)->${base} = glm::${func_glm_name}(
        /*$ {argc + argoc} $*/
        $?{type == 'float'
            a${I}${', ' if I + 1 < argc + argoc else ''}
        $??{
            glm_${type}Data(a${I})${', ' if I + 1 < argc + argoc else ''}
        $?}
        /*$ $*/
        );
        return result;
    $?}
}

/*$ $*/

PyObject *glm_function_project(PyObject *module, PyObject *args) {
    PyObject *a0;
    PyObject *a1;
    PyObject *a2;
    PyObject *a3;

	if(!PyArg_ParseTuple(args, "OOOO|:project",
	&a0, 
	&a1, 
	&a2,
    &a3
	))
		return NULL;
        
        PyObject *result = PyObject_CallObject((PyObject *)&glm_vec3Type, NULL);
        ((glm_vec3 *)result)->vec = glm::project(
            glm_vec3Data(a0), 
            glm_mat4Data(a1), 
            glm_mat4Data(a2),
            glm_vec4Data(a3)
        );
        return result;
}

PyObject *glm_function_unProject(PyObject *module, PyObject *args) {
    PyObject *a0;
    PyObject *a1;
    PyObject *a2;
    PyObject *a3;

	if(!PyArg_ParseTuple(args, "OOOO|:unProject",
	&a0, 
	&a1, 
	&a2,
    &a3
	))
		return NULL;
        
        PyObject *result = PyObject_CallObject((PyObject *)&glm_vec3Type, NULL);
        ((glm_vec3 *)result)->vec = glm::unProject(
            glm_vec3Data(a0), 
            glm_mat4Data(a1), 
            glm_mat4Data(a2),
            glm_vec4Data(a3)
        );
        return result;
}

PyObject *glm_function_pickMatrix(PyObject *module, PyObject *args) {
    PyObject *a0;
    PyObject *a1;
    PyObject *a2;

	if(!PyArg_ParseTuple(args, "OOO|:unProject",
	&a0, 
	&a1, 
	&a2
	))
		return NULL;
        
        PyObject *result = PyObject_CallObject((PyObject *)&glm_mat4Type, NULL);
        ((glm_mat4 *)result)->mat = glm::pickMatrix(
            glm_vec2Data(a0), 
            glm_vec2Data(a1), 
            glm_vec4Data(a2)
        );
        return result;
}

/* * * GLM Module * * */

/*$ NUMBER_FUNCTION $*/
PyDoc_STRVAR(glm_function_${func}__doc__, "${func_doc}");
/*$ $*/

PyDoc_STRVAR(glm_function_project__doc__, "Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates.");
PyDoc_STRVAR(glm_function_unProject__doc__, "Map the specified window coordinates (win.x, win.y, win.z) into object coordinates.");
PyDoc_STRVAR(glm_function_pickMatrix__doc__, "Define a picking region From GLM_GTC_matrix_transform extension.");

static
PyMethodDef glmmodule_methods[] = {
/*$ NUMBER_FUNCTION $*/
	{"${func}", (PyCFunction) glm_function_${func}, METH_VARARGS, glm_function_${func}__doc__},
/*$ $*/
    {"project", (PyCFunction) glm_function_project, METH_VARARGS, glm_function_project__doc__},
    {"un_project", (PyCFunction) glm_function_unProject, METH_VARARGS, glm_function_unProject__doc__},
    {"pick_matrix", (PyCFunction) glm_function_pickMatrix, METH_VARARGS, glm_function_pickMatrix__doc__},
	{NULL, NULL},
};

PyDoc_STRVAR(glmmodule__doc__, "GLSL Mathematics.");

static
PyModuleDef glmmodule = {
	PyModuleDef_HEAD_INIT,
	"glm",
	glmmodule__doc__,
	-1,
	glmmodule_methods,
};

extern "C"
PyMODINIT_FUNC
PyInit_glm()
{
	/* Ready Bases */
/*$ BASETYPEDEF $*/
	PyType_Ready(&glm_${type}IteratorType);
	PyType_Ready(&glm_${type}Type);
/*$ $*/
	
	/* Ready Matrices */
/*$ MATRIX $*/
	glm_${p}mat${n}Type.tp_base = &glm_MatrixType;
	PyType_Ready(&glm_${p}mat${n}Type);
/*$ $*/
	
	/* Ready Vectors */
/*$ VECTOR $*/
	glm_${p}vec${n}Type.tp_base = &glm_VectorType;
	PyType_Ready(&glm_${p}vec${n}Type);
/*$ $*/
	
	PyObject* m;

	m = PyModule_Create(&glmmodule);
	
	if(m == NULL)
		return NULL;
	
	/* Add types to module */
/*$ BASETYPEDEF $*/
	Py_INCREF(&glm_${type}IteratorType);
	Py_INCREF(&glm_${type}Type);
/*$ $*/
/*$ MATRIX $*/
	Py_INCREF(&glm_${p}mat${n}Type);
/*$ $*/
/*$ VECTOR $*/
	Py_INCREF(&glm_${p}vec${n}Type);
/*$ $*/
/*$ BASETYPEDEF $*/
	PyModule_AddObject(m, "${type}Iterator", (PyObject *) &glm_${type}IteratorType);
	PyModule_AddObject(m, "${type}", (PyObject *) &glm_${type}Type);
/*$ $*/
/*$ MATRIX $*/
	PyModule_AddObject(m, "${p}mat${n}", (PyObject *) &glm_${p}mat${n}Type);
/*$ $*/
/*$ VECTOR $*/
	PyModule_AddObject(m, "${p}vec${n}", (PyObject *) &glm_${p}vec${n}Type);
/*$ $*/
	
	return m;
}

}
