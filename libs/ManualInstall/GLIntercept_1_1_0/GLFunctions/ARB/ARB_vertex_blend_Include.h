#define GLI_INCLUDE_ARB_VERTEX_BLEND

enum Main {

  GL_MAX_VERTEX_UNITS_ARB               =   0x86A4,
  GL_ACTIVE_VERTEX_UNITS_ARB            =   0x86A5,
  GL_WEIGHT_SUM_UNITY_ARB               =   0x86A6,
  GL_VERTEX_BLEND_ARB                   =   0x86A7,
  GL_CURRENT_WEIGHT_ARB                 =   0x86A8,
  GL_WEIGHT_ARRAY_TYPE_ARB              =   0x86A9,
  GL_WEIGHT_ARRAY_STRIDE_ARB            =   0x86AA,
  GL_WEIGHT_ARRAY_SIZE_ARB              =   0x86AB,
  GL_WEIGHT_ARRAY_POINTER_ARB           =   0x86AC,
  GL_WEIGHT_ARRAY_ARB                   =   0x86AD,
  //GL_MODELVIEW0_ARB                     =   0x1700,
  GL_MODELVIEW1_ARB                     =   0x850A,
  GL_MODELVIEW2_ARB                     =   0x8722,
  GL_MODELVIEW3_ARB                     =   0x8723,
  GL_MODELVIEW4_ARB                     =   0x8724,
  GL_MODELVIEW5_ARB                     =   0x8725,
  GL_MODELVIEW6_ARB                     =   0x8726,
  GL_MODELVIEW7_ARB                     =   0x8727,
  GL_MODELVIEW8_ARB                     =   0x8728,
  GL_MODELVIEW9_ARB                     =   0x8729,
  GL_MODELVIEW10_ARB                    =   0x872A,
  GL_MODELVIEW11_ARB                    =   0x872B,
  GL_MODELVIEW12_ARB                    =   0x872C,
  GL_MODELVIEW13_ARB                    =   0x872D,
  GL_MODELVIEW14_ARB                    =   0x872E,
  GL_MODELVIEW15_ARB                    =   0x872F,
  GL_MODELVIEW16_ARB                    =   0x8730,
  GL_MODELVIEW17_ARB                    =   0x8731,
  GL_MODELVIEW18_ARB                    =   0x8732,
  GL_MODELVIEW19_ARB                    =   0x8733,
  GL_MODELVIEW20_ARB                    =   0x8734,
  GL_MODELVIEW21_ARB                    =   0x8735,
  GL_MODELVIEW22_ARB                    =   0x8736,
  GL_MODELVIEW23_ARB                    =   0x8737,
  GL_MODELVIEW24_ARB                    =   0x8738,
  GL_MODELVIEW25_ARB                    =   0x8739,
  GL_MODELVIEW26_ARB                    =   0x873A,
  GL_MODELVIEW27_ARB                    =   0x873B,
  GL_MODELVIEW28_ARB                    =   0x873C,
  GL_MODELVIEW29_ARB                    =   0x873D,
  GL_MODELVIEW30_ARB                    =   0x873E,
  GL_MODELVIEW31_ARB                    =   0x873F,

};


void glWeightbvARB  (GLint size, const GLbyte *weights);
void glWeightsvARB  (GLint size, const GLshort *weights);
void glWeightivARB  (GLint size, const GLint *weights);
void glWeightfvARB  (GLint size, const GLfloat *weights);
void glWeightdvARB  (GLint size, const GLdouble *weights);
void glWeightubvARB (GLint size, const GLubyte *weights);
void glWeightusvARB (GLint size, const GLushort *weights);
void glWeightuivARB (GLint size, const GLuint *weights);
void glWeightPointerARB (GLint size, GLenum[Main] type, GLsizei stride, const GLvoid *pointer);
void glVertexBlendARB (GLint count);
