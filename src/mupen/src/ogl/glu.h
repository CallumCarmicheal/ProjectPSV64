
#ifndef OPENGL_VITA_GLUPROJECT_H
	#define OPENGL_VITA_GLUPROJECT_H 1	

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>

	#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a) = (b); (b) = _tmp; }
	#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
	#define MAT(m, r, c) (m)[(c) * 4 + (r)]

	static void __gluMakeIdentityf(GLfloat m[16]);

	void GLAPIENTRY gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);

	#define __glPi 3.14159265358979323846

	void GLAPIENTRY gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

	static void normalize(float v[3]);

	static void cross(float v1[3], float v2[3], float result[3]);

	void GLAPIENTRY gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
		GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
		GLdouble upz);

	static void __gluMultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4],
		GLdouble out[4]);

	/*
	** Invert 4x4 matrix.
	** Contributed by David Moore (See Mesa bug #6748)
	*/
	static int __gluInvertMatrixd(const GLdouble m[16], GLdouble invOut[16]);

	static void __gluMultMatricesd(const GLdouble a[16], const GLdouble b[16],
		GLdouble r[16]);

	GLint GLAPIENTRY gluProject(GLdouble objx, GLdouble objy, GLdouble objz,
		const GLdouble modelMatrix[16],
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLdouble *winx, GLdouble *winy, GLdouble *winz);

	GLint GLAPIENTRY gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
		const GLdouble modelMatrix[16],
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLdouble *objx, GLdouble *objy, GLdouble *objz);

	GLint GLAPIENTRY gluUnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
		const GLdouble modelMatrix[16],
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLclampd nearVal, GLclampd farVal,
		GLdouble *objx, GLdouble *objy, GLdouble *objz,
		GLdouble *objw);

	void GLAPIENTRY gluPickMatrix(GLdouble x, GLdouble y, GLdouble deltax, GLdouble deltay,
		GLint viewport[4]);

#endif /* !OPENGL_VITA_GLUPROJECT_H */