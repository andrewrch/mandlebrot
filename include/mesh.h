#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 pos;
  glm::vec2 tex_coord;
};

class Mesh {
  public:
    explicit Mesh();
    explicit Mesh(const std::vector<Vertex>& v,
                  const std::vector<unsigned int>& i);
    const std::vector<Vertex>& getVertices();
    const std::vector<unsigned int>& getIndices();
  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
