#define GLI_INCLUDE_GL_PGI_MISC_HINTS

enum Main {

  GL_PREFER_DOUBLEBUFFER_HINT_PGI         = 0x1A1F8,
  GL_CONSERVE_MEMORY_HINT_PGI             = 0x1A1FD,
  GL_RECLAIM_MEMORY_HINT_PGI              = 0x1A1FE,
  GL_NATIVE_GRAPHICS_HANDLE_PGI           = 0x1A202,
  GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI       = 0x1A203,
  GL_NATIVE_GRAPHICS_END_HINT_PGI         = 0x1A204,
  GL_ALWAYS_FAST_HINT_PGI                 = 0x1A20C,
  GL_ALWAYS_SOFT_HINT_PGI                 = 0x1A20D,
  GL_ALLOW_DRAW_OBJ_HINT_PGI              = 0x1A20E,
  GL_ALLOW_DRAW_WIN_HINT_PGI              = 0x1A20F,
  GL_ALLOW_DRAW_FRG_HINT_PGI              = 0x1A210,
  GL_ALLOW_DRAW_MEM_HINT_PGI              = 0x1A211,
  GL_STRICT_DEPTHFUNC_HINT_PGI            = 0x1A216,
  GL_STRICT_LIGHTING_HINT_PGI             = 0x1A217,
  GL_STRICT_SCISSOR_HINT_PGI              = 0x1A218,
  GL_FULL_STIPPLE_HINT_PGI                = 0x1A219,
  GL_CLIP_NEAR_HINT_PGI                   = 0x1A220,
  GL_CLIP_FAR_HINT_PGI                    = 0x1A221,
  GL_WIDE_LINE_HINT_PGI                   = 0x1A222,
  GL_BACK_NORMALS_HINT_PGI                = 0x1A223,

};

void glHintPGI(GLenum[Main] target, GLint mode);
