#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#define GLM_FORCE_RADIANS
#include "glm/ext.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader_program.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

std::string vert_source = " \
#version 330\n\
layout (location = 0) in vec3 position;\n\
layout (location = 1) in vec2 tex_coord;\n\
void main() {\n\
  gl_Position = vec4(position, 1.0);\n\
}\n\
";

po::options_description initOptions() {
  po::options_description desc("Command line options");
  desc.add_options()
    ("help,h", "produce help message")
    ("frag,f", po::value<std::string>()->required(),
     "Fragment shader")
  ;
  return desc;
}

bool processArgs(int argc, char* argv[],
                 po::variables_map& args) {
  auto desc = initOptions();
  try {
  po::store(po::parse_command_line(argc, argv, desc), args);
    if (args.count("help")) {
      std::cout << desc << std::endl;
      return false;
    }
  po::notify(args);
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return false;
  }
  return true;
}

void errorCallback(int error, const char* description)
{
  std::cerr << description << " " << error << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode,
                 int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  // Leave this here to borrow later
  if (key == GLFW_KEY_N && action == GLFW_PRESS && (mods & GLFW_MOD_SHIFT)) {
  }
}

void shutdown(int code)
{
  glfwTerminate();
  std::cout << "Shutting down..." << std::endl;
  exit(code);
}

int main(int argc, char* argv[]) {
  // Process command line args
  po::variables_map args;
  if (!processArgs(argc, argv, args)) {
    exit(1);
  }
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) {
    shutdown(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "", NULL, NULL);
  if (!window) {
    shutdown(EXIT_FAILURE);
  }
  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    shutdown(EXIT_FAILURE);
  }
  glGetError();
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  Shader frag(args["frag"].as<std::string>(), ShaderType::FRAG);
  Shader vert(ShaderType::VERT);
  vert.add_source(vert_source);

  ShaderProgram program({frag, vert});

  glfwSetTime(0);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  return EXIT_SUCCESS;
}
