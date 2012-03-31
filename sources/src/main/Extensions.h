
#ifndef __JKT__EXTENSIONS_H
#define __JKT__EXTENSIONS_H

#include "glext.h"

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;

#endif
