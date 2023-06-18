#include "mesh.hpp"
#include "GL/glew.h"
#include "DIDebuger/debuger.hpp"


namespace DI {

extern DebugData* eg_debugData;

void MeshHandler::Set(Mesh &mesh){
  glGenVertexArrays(1,&mesh.buffer.vao);
  glBindVertexArray(mesh.buffer.vao);

  glGenBuffers(1,&mesh.buffer.vbo);
  glBindBuffer(GL_ARRAY_BUFFER,mesh.buffer.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.verticies.size, mesh.verticies.data, GL_STATIC_DRAW);

  glGenBuffers(1,&mesh.buffer.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffer.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.elements.count, mesh.elements.data, GL_STATIC_DRAW);
  mesh.model_matrix = glm::mat4(1.0f);

  eg_debugData->counterDIMeshes_inMem++;
  eg_debugData->counterDIElements_inMem += mesh.elements.count;
  eg_debugData->counterDIVerticies_inMem += mesh.verticies.count;
}
void MeshHandler::Scale(Mesh &mesh, const glm::vec3 offset){
    mesh.model_matrix = glm::scale(mesh.model_matrix,offset);
}
void MeshHandler::Translate(Mesh &mesh, const glm::vec3 offset){
    mesh.model_matrix = glm::translate(mesh.model_matrix,offset);
}
void MeshHandler::Rotate(Mesh &mesh, const float angle, const glm::vec3 offset){
    mesh.model_matrix = glm::rotate(mesh.model_matrix,glm::radians(angle),offset);
}
}
