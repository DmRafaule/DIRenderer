#pragma once

#include "drawable.hpp"

namespace DI{
	
	struct Mesh;
	struct Shader;
  class Primitive;
  class Engine;

  // Control what to draw
  class RendererSingleton{
    friend class Engine;
    private:
      RendererSingleton(){}
      static void Init();
      static void Kill();
    private:
      static RendererSingleton* m_render;
    public:
      static inline RendererSingleton* GetInstance(){
        if (m_render == 0){
          m_render = new RendererSingleton();
        }
        return m_render; 
      }
      inline void Draw(Drawable& object){
        object.Draw();
      }
		  // Drawing using elements
		  static void DrawElements(Mesh &mesh, unsigned int gl_render_type);
		  // Drawing using arrays
		  static void DrawArrays(Mesh &mesh, unsigned int gl_render_type);

  };

};
