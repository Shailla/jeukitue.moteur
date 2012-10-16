#define GLI_INCLUDE_NV_REGISTER_COMBINERS

enum Main {
  
  GL_REGISTER_COMBINERS_NV                        =   0x8522,
  GL_VARIABLE_A_NV                                =   0x8523,
  GL_VARIABLE_B_NV                                =   0x8524,
  GL_VARIABLE_C_NV                                =   0x8525,
  GL_VARIABLE_D_NV                                =   0x8526,
  GL_VARIABLE_E_NV                                =   0x8527,
  GL_VARIABLE_F_NV                                =   0x8528,
  GL_VARIABLE_G_NV                                =   0x8529,
  GL_CONSTANT_COLOR0_NV                           =   0x852A,
  GL_CONSTANT_COLOR1_NV                           =   0x852B,
  GL_PRIMARY_COLOR_NV                             =   0x852C,
  GL_SECONDARY_COLOR_NV                           =   0x852D,
  GL_SPARE0_NV                                    =   0x852E,
  GL_SPARE1_NV                                    =   0x852F,
  GL_DISCARD_NV                                   =   0x8530,
  GL_E_TIMES_F_NV                                 =   0x8531,
  GL_SPARE0_PLUS_SECONDARY_COLOR_NV               =   0x8532,
  GL_UNSIGNED_IDENTITY_NV                         =   0x8536,
  GL_UNSIGNED_INVERT_NV                           =   0x8537,
  GL_EXPAND_NORMAL_NV                             =   0x8538,
  GL_EXPAND_NEGATE_NV                             =   0x8539,
  GL_HALF_BIAS_NORMAL_NV                          =   0x853A,
  GL_HALF_BIAS_NEGATE_NV                          =   0x853B,
  GL_SIGNED_IDENTITY_NV                           =   0x853C,
  GL_SIGNED_NEGATE_NV                             =   0x853D,
  GL_SCALE_BY_TWO_NV                              =   0x853E,
  GL_SCALE_BY_FOUR_NV                             =   0x853F,
  GL_SCALE_BY_ONE_HALF_NV                         =   0x8540,
  GL_BIAS_BY_NEGATIVE_ONE_HALF_NV                 =   0x8541,
  GL_COMBINER_INPUT_NV                            =   0x8542,
  GL_COMBINER_MAPPING_NV                          =   0x8543,
  GL_COMBINER_COMPONENT_USAGE_NV                  =   0x8544,
  GL_COMBINER_AB_DOT_PRODUCT_NV                   =   0x8545,
  GL_COMBINER_CD_DOT_PRODUCT_NV                   =   0x8546,
  GL_COMBINER_MUX_SUM_NV                          =   0x8547,
  GL_COMBINER_SCALE_NV                            =   0x8548,
  GL_COMBINER_BIAS_NV                             =   0x8549,
  GL_COMBINER_AB_OUTPUT_NV                        =   0x854A,
  GL_COMBINER_CD_OUTPUT_NV                        =   0x854B,
  GL_COMBINER_SUM_OUTPUT_NV                       =   0x854C,
  GL_MAX_GENERAL_COMBINERS_NV                     =   0x854D,
  GL_NUM_GENERAL_COMBINERS_NV                     =   0x854E,
  GL_COLOR_SUM_CLAMP_NV                           =   0x854F,
  GL_COMBINER0_NV                                 =   0x8550,
  GL_COMBINER1_NV                                 =   0x8551,
  GL_COMBINER2_NV                                 =   0x8552,
  GL_COMBINER3_NV                                 =   0x8553,
  GL_COMBINER4_NV                                 =   0x8554,
  GL_COMBINER5_NV                                 =   0x8555,
  GL_COMBINER6_NV                                 =   0x8556,
  GL_COMBINER7_NV                                 =   0x8557,

  //NV_register_combiners2
  GL_PER_STAGE_CONSTANTS_NV                       =   0x8535,

};



void glCombinerParameterfvNV(GLenum[Main] pname, const GLfloat *params);

void glCombinerParameterivNV(GLenum[Main] pname, const GLint *params);

void glCombinerParameterfNV(GLenum[Main] pname, GLfloat param);

void glCombinerParameteriNV(GLenum[Main] pname, GLint param);

void glCombinerInputNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] variable, GLenum[Main] input, GLenum[Main] mapping, GLenum[Main] componentUsage);

void glCombinerOutputNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] abOutput, GLenum[Main] cdOutput, GLenum[Main] sumOutput, GLenum[Main] scale, GLenum[Main] bias, GLboolean abDotProduct,GLboolean cdDotProduct, GLboolean muxSum);

void glFinalCombinerInputNV(GLenum[Main] variable, GLenum[Main] input, GLenum[Main] mapping, GLenum[Main] componentUsage);

void glGetCombinerInputParameterfvNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] variable, GLenum[Main] pname, GLfloat *params);

void glGetCombinerInputParameterivNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] variable, GLenum[Main] pname, GLint *params);

void glGetCombinerOutputParameterfvNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] pname, GLfloat *params);

void glGetCombinerOutputParameterivNV(GLenum[Main] stage, GLenum[Main] portion, GLenum[Main] pname, GLint *params);

void glGetFinalCombinerInputParameterfvNV(GLenum[Main] variable, GLenum[Main] pname, GLfloat *params);

void glGetFinalCombinerInputParameterivNV(GLenum[Main] variable, GLenum[Main] pname, GLint *params);

//NV_register_combiners2
void glCombinerStageParameterfvNV(GLenum[Main] stage, GLenum[Main] pname, const GLfloat *params);

void glGetCombinerStageParameterfvNV(GLenum[Main] stage, GLenum[Main] pname, GLfloat *params);


