#pragma once 

#include <string>


namespace DI{
   
   struct Texture;
   struct Shader;
   // Contain layout data
   struct Layout{
      unsigned int  index;
      unsigned int  count;
      unsigned int  type;
      unsigned char normalized;
      unsigned int  stribe;
      unsigned int  entry;
   };
   // Set up all attributes for buffer(it depends from shader(vertex))
   namespace LayoutHandler{
      // Set up layout for current buffers (mesh)
      void Set(const std::string& vertShader_path);
      // get layout size( accept source - not path)
      int get_layout_size(const std::string& src);
   }
   
} // namespace DI
