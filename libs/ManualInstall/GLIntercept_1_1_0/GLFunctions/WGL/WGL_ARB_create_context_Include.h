#define GLI_INCLUDE_WGL_ARB_CREATE_CONTEXT


/* WGL enum values are not displayed currently
  WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091,
  WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092,
  WGL_CONTEXT_LAYER_PLANE_ARB   = 0x2093,
  WGL_CONTEXT_FLAGS_ARB         = 0x2094,
  WGL_CONTEXT_PROFILE_MASK_ARB  = 0x9126,

  // Mask bits
  WGL_CONTEXT_DEBUG_BIT_ARB              = 0x0001,
  WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 0x0002,

  // Mask bits
  WGL_CONTEXT_CORE_PROFILE_BIT_ARB          = 0x00000001,
  WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x00000002,

  // Windows error codes
  ERROR_INVALID_VERSION_ARB = 0x2095,
  ERROR_INVALID_PROFILE_ARB = 0x2096,
*/


void * wglCreateContextAttribsARB(void * hDC, void * hShareContext, const GLint *attribList);
