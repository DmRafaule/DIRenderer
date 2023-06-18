#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace DI{

   struct Texture;
   struct Shader;
   enum  UniformType : unsigned int;

   // Consist from uniforms and samplers(textures)
   struct Material{
      std::vector<std::pair<std::string,Texture>> samplers;
      std::unordered_map<std::string,std::pair<void*,UniformType>> uniforms;
   };
   // Conrol material data
   namespace MaterialHandler{
      // Set sampler data
      void SetSampler(Material& material, const Texture& sampler);
      // Set uniform data
      void SetShader(Material& material, Shader& shader);
      // Get access to material
      void UseMaterial(Material& material, Shader& shader);
   };

} // namespace DI
