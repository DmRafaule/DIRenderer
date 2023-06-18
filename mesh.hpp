#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DI {
   // Contain vertices data
   struct Vertice{
      void         *data;
      unsigned int  count;
      unsigned int  size;// How long data in one vertice
   };
   // Contain element data(Indices)
   struct Element{
      void         *data;
      unsigned int  count;
   };
   // Contain buffers (till now this ones)
   struct Buffer{
      unsigned int ebo;
      unsigned int vao;
      unsigned int vbo;
   };
   // Containg data about meshe
   struct Mesh{
      Buffer       buffer;
      Vertice      verticies;
      Element      elements; 
      glm::mat4    model_matrix;
   };
  
   // Control mesh data
   namespace MeshHandler{
      // Filed up mesh data
      void Set(Mesh &mesh);
      // Scale mesh
      void Scale(Mesh &mesh, const glm::vec3 offset);
      // Translate mesh
      void Translate(Mesh &mesh, const glm::vec3 offset);
      // Rotate mesh
      void Rotate(Mesh &mesh,const float angle, const glm::vec3 offset);
   };

}
