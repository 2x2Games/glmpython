#!/usr/bin/env python3

import parseme

glmParse = parseme.Project()

# In the VECTOR section,
# p is a prefix to the name
# n is the number of components in the vector
# type is the common type of the vector
# Make sure to include all three of a type because swizzling relies on them
VECTOR = parseme.Section('VECTOR')
for t in (('', 'float'), ('i', 'int')):
	for n in range(2, 5):
		VECTOR.add(parseme.Round(p = t[0], n = n, type = t[1]))
glmParse.add(VECTOR)

VECTOR_MATH = parseme.Section('VECTOR_MATH')
VECTOR_MATH.add(parseme.Round(s = '+', f = 'add', only = None))
VECTOR_MATH.add(parseme.Round(s = '-', f = 'subtract', only = None))
VECTOR_MATH.add(parseme.Round(s = '*', f = 'multiply', only = None))
VECTOR_MATH.add(parseme.Round(s = '/', f = 'true_divide', only = None))
VECTOR_MATH.add(parseme.Round(s = '<<', f = 'lshift', only = 'int'))
VECTOR_MATH.add(parseme.Round(s = '>>', f = 'rshift', only = 'int'))
VECTOR_MATH.add(parseme.Round(s = '&', f = 'and', only = 'int'))
VECTOR_MATH.add(parseme.Round(s = '^', f = 'xor', only = 'int'))
VECTOR_MATH.add(parseme.Round(s = '|', f = 'or', only = 'int'))

glmParse.add(VECTOR_MATH)

# In the MATRIX section,
# p is a prefix to the name
# cols and rows is the size of the vector
# type is the common type of the vector
# n is is the name, such as 3x4
MATRIX = parseme.Section('MATRIX')
for cols in range(2, 5):
	for rows in range(2, 5):
		MATRIX.add(parseme.Round(p = '', rows = rows, cols = cols,
			n = (str(rows) if rows == cols else str(rows) + 'x' + str(cols)), type = 'float'))
glmParse.add(MATRIX)

# In the MATRIX_FUNCTION section,
# func is the name of the function
# func_doc is the doc string
# args is the type of arguments
# availableTo is which types support it

# Matrix Transform

MATRIX_FUNCTION = parseme.Section('MATRIX_FUNCTION')
MATRIX_FUNCTION.add(parseme.Round(
	func = 'translate',
	func_doc = 'Translates a 4x4 matrix.',
	args = ('vec3',),
	availableTo = ('4',)
))
MATRIX_FUNCTION.add(parseme.Round(
	func = 'rotate',
	func_doc = 'Rotates a 4x4 matrix.',
	args = (float, 'vec3',),
	availableTo = ('4',)
))
MATRIX_FUNCTION.add(parseme.Round(
	func = 'scale',
	func_doc = 'Scales a 4x4 matrix.',
	args = ('vec3',),
	availableTo = ('4',)
))

# Core

MATRIX_FUNCTION.add(parseme.Round(
	func = 'inverse',
	func_doc = 'Matrix\'s inverse.',
	args = (),
	argsT = '',
	availableTo = ('2','3','4',)
))

MATRIX_FUNCTION.add(parseme.Round(
	func = 'transpose',
	func_doc = 'Transposed matrix.',
	args = (),
	argsT = '',
	availableTo = ('2','3','4',)
))

MATRIX_FUNCTION.add(parseme.Round(
	func = 'mat3',
	func_doc = 'Trunc to mat3.',
	args = (),
	argsT = '',
	availableTo = ('4',)
))

glmParse.add(MATRIX_FUNCTION)

# In the VECTOR_FUNCTION section,
# func is the name of the function
# func_doc is the doc string
VECTOR_FUNCTION = parseme.Section('VECTOR_FUNCTION')
VECTOR_FUNCTION.add(parseme.Round(
    func = 'length',
    func_doc = 'Length of the vector.',
    args = (),
    availableTo = ('2','3','4',),
    returns = 'float'
))

VECTOR_FUNCTION.add(parseme.Round(
    func = 'normalize',
    func_doc = 'Returns the normalized vector.',
    args = (),
    availableTo = ('2','3','4',),
    returns = 'vec'
))

VECTOR_FUNCTION.add(parseme.Round(
    func = 'dot',
    func_doc = 'Returns the dot product of this ond other vector.',
    args = ('vec3', ),
    availableTo = ('2','3','4',),
    returns = 'float'
))

VECTOR_FUNCTION.add(parseme.Round(
    func = 'cross',
    func_doc = 'Returns the cross product of this ond other vector.',
    args = ('vec3', ),
    availableTo = ('3',),
    returns = 'vec'
))
glmParse.add(VECTOR_FUNCTION)

# In the NUMBER_FUNCTION section,
# func is the name of the function
# func_doc is the doc string
# argc is the number of arguments
# argoc is the number of optional arguments
# returns is the return type
# type is the argument type
# p is the short name of the in type, used to build value
# base the base type of the return
NUMBER_FUNCTION = parseme.Section('NUMBER_FUNCTION')

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'ortho',
        func_glm_name = 'ortho',
		func_doc = 'Creates an 3d orthographic matrix.',
		argc = 6,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'ortho2d',
        func_glm_name = 'ortho',
		func_doc = 'Creates an 2d orthographic matrix.',
		argc = 4,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'frustum', 
        func_glm_name = 'frustum',
		func_doc = 'Creates a frustum matrix.',
		argc = 6,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'perspective',
        func_glm_name = 'perspective',
		func_doc = 'Creates a perspective matrix.',
		argc = 4,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'perspective_fov',
        func_glm_name = 'perspectiveFov',
		func_doc = 'Creates a perspective matrix with a defined FOV.',
		argc = 5,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'infinite_perspective',
        func_glm_name = 'infinitePerspective',
		func_doc = 'Creates a matrix for a symmetric perspective-view frustum with far plane at infinite.',
		argc = 3,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'tweaked_infinite_perspective',
        func_glm_name = 'tweakedInfinitePerspective',
		func_doc = "Creates a matrix for a symmetric perspective-view frustum with far plane at infinite for graphics hardware that doesn't support depth clamping.",
		argc = 3,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'perlin',
        func_glm_name = 'perlin',
		func_doc = 'Perlin noise.',
		argc = 1,
		argoc = 0,
		returns = 'float',
		type = 'vec2',
		p = 'O',
		base = 'vec'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'perlin_periodic',
        func_glm_name = 'perlin',
		func_doc = 'Perlin noise.',
		argc = 2,
		argoc = 0,
		returns = 'float',
		type = 'vec2',
		p = 'O',
		base = 'vec'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'step',
        func_glm_name = 'step',
		func_doc = 'Step function.',
		argc = 2,
		argoc = 0,
		returns = 'vec3',
		type = 'vec3',
		p = 'O',
		base = 'vec'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'radians',
        func_glm_name = 'radians',
		func_doc = 'Step function.',
		argc = 1,
		argoc = 0,
		returns = 'float',
		type = 'float',
		p = 'f',
		base = 'float'
	)
)


NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'look_at',
        func_glm_name = 'lookAt',
		func_doc = 'Build a look at view matrix.',
		argc = 3,
		argoc = 0,
		returns = 'mat4',
		type = 'vec3',
		p = 'O',
		base = 'mat'
	)
)

NUMBER_FUNCTION.add(
	parseme.Round(
		func = 'yaw_pitch_roll',
        func_glm_name = 'yawPitchRoll',
		func_doc = 'Creates a 3D 4 * 4 homogeneous rotation matrix from euler angles (Y * X * Z).',
		argc = 3,
		argoc = 0,
		returns = 'mat4',
		type = 'float',
		p = 'f',
		base = 'mat'
	)
)

glmParse.add(NUMBER_FUNCTION)

BASETYPEDEF = parseme.Section('BASETYPEDEF')
BASETYPEDEF.add(parseme.Round(type = 'Vector', doc = 'This is a basic vector type that you can isinstance against.  It is also used for global function checking, and in theory you could make your own vector types which define custom calls for global functions.'))
BASETYPEDEF.add(parseme.Round(type = 'Matrix', doc = 'A matrix.'))
glmParse.add(BASETYPEDEF)

if glmParse.parse('pyglm/glm/python.parseme.hpp', 'pyglm/glm/python.parseme.cpp') > 0:
	raise SystemExit

import sys
from distutils.core import setup, Extension
import platform
architecture = platform.architecture()[0]

if sys.platform == "win32":
    compile_args = ["/Ox", "/Ob2", "/Oi", "/Ot", "/Oy", "/GT", "/GL"]
    compile_args += [] if architecture == "64bit" else ["/arch:IA32"]
else:
    compile_args = ["-O3", "-ffast-math", "-s"]
    # This might be needed on posix
    # compile_args += [] if architecture == "64bit" else ["-no-vec"]

glm = Extension('glm', sources = ['pyglm/glm/python.cpp'], extra_compile_args=compile_args, include_dirs = ['../'])

setup(name='glm',
      version='0.9.3',
      description='glm',
      author='JacobF | G-Truc Creation',
      author_email='jacobaferrero@gmail.com | glm@g-truc.net',
      url='http://glm.g-truc.net/',
      ext_modules=[glm]
)
