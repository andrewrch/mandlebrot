#include "mesh.h"

Mesh::Mesh() : vertices(0), indices(0) {

}

Mesh::Mesh(const std::vector<Vertex>& v,
           const std::vector<unsigned int>& i) : vertices(v), indices(i) {
}

const std::vector<Vertex>& Mesh::getVertices() {
  return vertices;
}

const std::vector<unsigned int>& Mesh::getIndices() {
  return indices;
}
