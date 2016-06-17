
#ifndef __JKT__EXTENSIONS_H
#define __JKT__EXTENSIONS_H

#include "GL/glext.h"

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);

extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;

#endif
