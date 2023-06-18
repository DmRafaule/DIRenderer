#pragma once

#include <string>
#include "GL/glew.h"

#include "stb_image.h"


struct Shader;

namespace DI{
// Texture data (sampler not supposed to be here)
struct Texture{
   std::string sampler;
   std::string path;
   unsigned int id;
   
   unsigned int mode;
   unsigned char* data;
   unsigned int slot;

   unsigned int wrap_s   = GL_CLAMP_TO_BORDER; 
   unsigned int wrap_t   = GL_CLAMP_TO_BORDER; 
   unsigned int min_filt = GL_NEAREST; 
   unsigned int mag_filt = GL_NEAREST;
   int width;
   int height;
   int chanels;

};
// Control texture data
namespace TextureHandler{
  void Set(Texture &texture, const std::string path);
  void SetFilters(Texture &texture, unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filt, unsigned int mag_filt);
  void Use(Texture &texture,const Shader& shad);
};

}
