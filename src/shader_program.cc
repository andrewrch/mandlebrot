#include "shader_program.h"

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders) {
  id = glCreateProgram();
  for (auto& shader : shaders) {
    shader.compile();
    glAttachShader(id, shader.id);
  }

  GLint status;
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint len;
    glGetProgramiv (id, GL_INFO_LOG_LENGTH, &len);
    GLchar info_log[len];
    glGetProgramInfoLog (id, len, NULL, info_log);
    fprintf(stderr, "Shader link failure\n%s", info_log);
  }

  for (auto& shader : shaders) {
    glDetachShader(id, shader.id);
  }
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(id);
}

void ShaderProgram::use() {
  glUseProgram(id);
}

template<>
void ShaderProgram::add_uniform(const std::string& name,
                                        double uniform) {
  GLuint location = glGetUniformLocation(id, name.c_str());
  glUniform1d(location, uniform);
}

template<>
void ShaderProgram::add_uniform(const std::string& name,
                                     int uniform) {
  GLuint location = glGetUniformLocation(id, name.c_str());
  glUniform1i(location, uniform);
}

template<>
void ShaderProgram::add_uniform(const std::string& name,
                                           const glm::vec3& uniform) {
  GLuint location = glGetUniformLocation(id, name.c_str());
  glUniform3fv(location, 1, glm::value_ptr(uniform));
}

template<>
void ShaderProgram::add_uniform(const std::string& name,
                                           const glm::vec4& uniform) {
  GLuint location = glGetUniformLocation(id, name.c_str());
  glUniform4fv(location, 1, glm::value_ptr(uniform));
}

template<>
void ShaderProgram::add_uniform(const std::string& name,
                                           const glm::mat4& uniform) {
  GLuint location = glGetUniformLocation(id, name.c_str());
  glUniformMatrix4fv(location, 1, false, glm::value_ptr(uniform));
}
