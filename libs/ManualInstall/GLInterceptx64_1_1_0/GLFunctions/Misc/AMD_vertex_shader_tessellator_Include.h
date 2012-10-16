#define GLI_INCLUDE_GL_AMD_VERTEX_SHADER_TESSELLATOR

enum Main {

  GL_SAMPLER_BUFFER_AMD                    = 0x9001,
  GL_INT_SAMPLER_BUFFER_AMD                = 0x9002,
  GL_UNSIGNED_INT_SAMPLER_BUFFER_AMD       = 0x9003,
  GL_TESSELLATION_MODE_AMD                 = 0x9004,
  GL_TESSELLATION_FACTOR_AMD               = 0x9005,
  GL_DISCRETE_AMD                          = 0x9006,
  GL_CONTINUOUS_AMD                        = 0x9007,

};

void glTessellationFactorAMD(GLfloat factor);
void glTessellationModeAMD(GLenum[Main] mode);
