#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DI{

// Types for OpenGL
enum UniformType : unsigned int{
   FL,
   IN,
   SAMPLER2D,
   UIN,
   DOB,
   BOOL,
   FVEC2,
   FVEC3,
   FVEC4,
   IVEC2,
   IVEC3,
   IVEC4,
   DVEC2,
   DVEC3,
   DVEC4,
   UIVEC2,
   UIVEC3,
   UIVEC4,
   MAT2,
   MAT3,
   MAT4
};
// Shader data contain id  of program and their uniforms
struct Shader{
   bool isDebugCount;
   unsigned int id;
   std::string layout; // hold a path of vertex shader, wich will be parsed in future for defining layout
   std::unordered_map<std::string,UniformType> uniforms;
};
// Control data for Shader struct
namespace ShaderHandler{
   // Set Shader data (Create shader program)
   void Set(Shader &shader, const std::string& vertex_path, const std::string& fragment_path);
   // Use Shader 
   void Use(Shader &shader);
   // Get sources of specified file
   std::string Parse(const std::string& name);
   // Get string type 
   std::string GetType(const UniformType type);
   // Set uniform
   void SetUniform(const Shader &shader, const char* name, const float& arg);
   void SetUniform(const Shader &shader, const char* name, const double& arg);
   void SetUniform(const Shader &shader, const char* name, const int& arg);
   void SetUniform(const Shader &shader, const char* name, const unsigned int& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::vec2& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::vec3& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::vec4& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::ivec2& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::ivec3& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::ivec4& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::uvec2& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::uvec3& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::uvec4& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::dvec2& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::dvec3& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::dvec4& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::mat2& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::mat3& arg);
   void SetUniform(const Shader &shader, const char* name, const glm::mat4& arg);
   // Compile shader by type
   unsigned int Compile(unsigned int type, const std::string& source);
   // Set uniforms data in Shader struct
   void processUniforms(Shader &shader, const std::string& source);
   // process blocks uniform (struct)
   void subProcessUniformBlock(Shader& shader, std::string src, const std::string u_name, std::string type_compare);
   // process uniforms as (float double int ...)
   void subProcessUniformSimple(Shader& shader, std::string u_name, std::string type_compare);
   // process uniforms as (vec3 mat4 dvec2 ...)
   void subProcessUniformComplex(Shader& shader, std::string u_name, std::string type_compare);
   // emplace new uniform in struct Shader::uniforms
   void setNewUniform(Shader& shader, const int u_count, std::string u_name, std::string type_compare);
}

}
