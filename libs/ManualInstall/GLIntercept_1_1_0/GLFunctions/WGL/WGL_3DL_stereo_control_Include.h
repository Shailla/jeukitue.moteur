#define GLI_INCLUDE_WGL_3DL_STEREO_CONTROL


enum Main {

  WGL_STEREO_EMITTER_ENABLE_3DL      =  0x2055,
  WGL_STEREO_EMITTER_DISABLE_3DL     =  0x2056,
  WGL_STEREO_POLARITY_NORMAL_3DL     =  0x2057,
  WGL_STEREO_POLARITY_INVERT_3DL     =  0x2058,

};

GLboolean wglSetStereoEmitterState3DL(void * hDC, GLuint uState);


