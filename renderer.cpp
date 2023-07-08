#include "renderer.hpp"
#include "GL/glew.h"
#include "mesh.hpp"
//#include "model.hpp"
#include "material.hpp"
#include "DIWindow/win.hpp"
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"


namespace DI{

extern DebugData* eg_debugData;

RendererSingleton* RendererSingleton::m_render = 0;
void RendererSingleton::Init(){
  DI_LOG_TRACE("RendererSingleton say: Init RendererSingleton");
  m_render = new RendererSingleton();
}  
void RendererSingleton::Kill(){
  DI_LOG_TRACE("RendererSingleton say: Kill RendererSingleton");
  delete m_render;
}  

void RendererSingleton::DrawElements(Mesh &mesh, unsigned int gl_render_type){
	glBindVertexArray(mesh.buffer.vao);
  glDrawElements(gl_render_type,mesh.elements.count,GL_UNSIGNED_INT,0);
  eg_debugData->counterDIVerticies_inUse += mesh.verticies.count;
  eg_debugData->counterDIElements_inUse += mesh.elements.count;
  eg_debugData->counterDICalls++;
}
void RendererSingleton::DrawArrays(Mesh &mesh, unsigned int gl_render_type){
	glBindVertexArray(mesh.buffer.vao);
  glDrawArrays(gl_render_type,0,mesh.elements.count);
  eg_debugData->counterDIVerticies_inUse += mesh.verticies.count;
  eg_debugData->counterDICalls++;
}

};
