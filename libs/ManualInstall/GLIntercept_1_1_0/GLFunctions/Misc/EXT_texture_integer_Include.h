#define GLI_INCLUDE_EXT_TEXTURE_INTEGER


enum Main {
  
  //GL_RGBA32UI_EXT                  = 0x8D70,
  //GL_RGB32UI_EXT                   = 0x8D71,
  GL_ALPHA32UI_EXT                 = 0x8D72,
  GL_INTENSITY32UI_EXT             = 0x8D73,
  GL_LUMINANCE32UI_EXT             = 0x8D74,
  GL_LUMINANCE_ALPHA32UI_EXT       = 0x8D75,
  //GL_RGBA16UI_EXT                  = 0x8D76,
  //GL_RGB16UI_EXT                   = 0x8D77,
  GL_ALPHA16UI_EXT                 = 0x8D78,
  GL_INTENSITY16UI_EXT             = 0x8D79,
  GL_LUMINANCE16UI_EXT             = 0x8D7A,
  GL_LUMINANCE_ALPHA16UI_EXT       = 0x8D7B,
  //GL_RGBA8UI_EXT                   = 0x8D7C,
  //GL_RGB8UI_EXT                    = 0x8D7D,
  GL_ALPHA8UI_EXT                  = 0x8D7E,
  GL_INTENSITY8UI_EXT              = 0x8D7F,
  GL_LUMINANCE8UI_EXT              = 0x8D80,
  GL_LUMINANCE_ALPHA8UI_EXT        = 0x8D81,
  //GL_RGBA32I_EXT                   = 0x8D82,
  //GL_RGB32I_EXT                    = 0x8D83,
  GL_ALPHA32I_EXT                  = 0x8D84,
  GL_INTENSITY32I_EXT              = 0x8D85,
  GL_LUMINANCE32I_EXT              = 0x8D86,
  GL_LUMINANCE_ALPHA32I_EXT        = 0x8D87,
  //GL_RGBA16I_EXT                   = 0x8D88,
  //GL_RGB16I_EXT                    = 0x8D89,
  GL_ALPHA16I_EXT                  = 0x8D8A,
  GL_INTENSITY16I_EXT              = 0x8D8B,
  GL_LUMINANCE16I_EXT              = 0x8D8C,
  GL_LUMINANCE_ALPHA16I_EXT        = 0x8D8D,
  //GL_RGBA8I_EXT                    = 0x8D8E,
  //GL_RGB8I_EXT                     = 0x8D8F,
  GL_ALPHA8I_EXT                   = 0x8D90,
  GL_INTENSITY8I_EXT               = 0x8D91,
  GL_LUMINANCE8I_EXT               = 0x8D92,
  GL_LUMINANCE_ALPHA8I_EXT         = 0x8D93,
  //GL_RED_INTEGER_EXT               = 0x8D94,
  //GL_GREEN_INTEGER_EXT             = 0x8D95,
  //GL_BLUE_INTEGER_EXT              = 0x8D96,
  //GL_ALPHA_INTEGER_EXT             = 0x8D97,
  //GL_RGB_INTEGER_EXT               = 0x8D98,
  //GL_RGBA_INTEGER_EXT              = 0x8D99,
  //GL_BGR_INTEGER_EXT               = 0x8D9A,
  //GL_BGRA_INTEGER_EXT              = 0x8D9B,
  GL_LUMINANCE_INTEGER_EXT         = 0x8D9C,
  GL_LUMINANCE_ALPHA_INTEGER_EXT   = 0x8D9D,
  GL_RGBA_INTEGER_MODE_EXT         = 0x8D9E,

};

void glClearColorIiEXT ( GLint r, GLint g, GLint b, GLint a );
void glClearColorIuiEXT ( GLuint r, GLuint g, GLuint b, GLuint a );
void glTexParameterIivEXT( GLenum[Main] target, GLenum[Main] pname, const GLint *params );
void glTexParameterIuivEXT( GLenum[Main] target, GLenum[Main] pname, const GLuint *params );
void glGetTexParameterIivEXT ( GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetTexParameterIuivEXT ( GLenum[Main] target, GLenum[Main] pname, GLuint *params);