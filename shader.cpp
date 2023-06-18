#include <fstream>
#include <iostream>
#include <unordered_set>
#include "shader.hpp"
#include "GL/glew.h"
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"

namespace DI{

extern DebugData* eg_debugData;

void ShaderHandler::Set(Shader& shader, const std::string& vertex_name, const std::string& fragment_name){
   // Get source of shader files
   shader.isDebugCount = false;
   shader.layout = vertex_name;
   std::string vertex_src = ShaderHandler::Parse(vertex_name);
   std::string fragment_src = ShaderHandler::Parse(fragment_name);
   // Set uniforms
   processUniforms(shader,vertex_src);
   processUniforms(shader,fragment_src);

   shader.id = glCreateProgram();
   uint vs = Compile(GL_VERTEX_SHADER,vertex_src);
   uint fs = Compile(GL_FRAGMENT_SHADER,fragment_src);

   glAttachShader(shader.id,vs);
   glAttachShader(shader.id,fs);
   glLinkProgram(shader.id);
   glValidateProgram(shader.id);
   eg_debugData->counterDIShaders_inMem++;
   glDeleteShader(vs);
   glDeleteShader(fs);
}

void ShaderHandler::Use(Shader& shader){
   if (!shader.isDebugCount){
      eg_debugData->counterDIShaders_inUse++;
      shader.isDebugCount = true;
   }
   glUseProgram(shader.id);
}
std::string ShaderHandler::Parse(const std::string& name){
   std::fstream file;
   std::string fileContent{""};
   file.open(name);
   if (!file.is_open()){
      fileContent = "not_loaded";
      DI_LOG_ERROR("ShaderHandler say: Failed opening file {0}",name);
   }
   else{
       std::string buffer;
       while(std::getline(file,buffer)){
           fileContent += buffer + "\n";
       }
       file.close();
   }

   return fileContent;
}
unsigned int ShaderHandler::Compile(unsigned int type, const std::string& source){
   unsigned int id = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id,1,&src,nullptr); 
   glCompileShader(id);

   int result;
   glGetShaderiv(id,GL_COMPILE_STATUS,&result);
   if (result == GL_FALSE){
       int length;
       glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
       char* message =  (char*)alloca(length * sizeof(char));
       glGetShaderInfoLog(id,length,&length,message);
       std::cout << "Faile compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader.\n";
       std::cout << message << std::endl;
       glDeleteShader(id);
       return 1;
   }

   return id;
}

void ShaderHandler::setNewUniform(Shader& shader, const int u_count, std::string u_name, std::string type_compare){
   // Define type for hash table
   UniformType u_type;
   switch(u_count){
      case 1:{
         if (type_compare == "float")
            u_type = UniformType::FL;
         else if (type_compare == "int")
            u_type = UniformType::IN;
         else if (type_compare == "sampler2D")
            u_type = UniformType::SAMPLER2D;
         else if (type_compare == "uint")
            u_type = UniformType::UIN;
         else if (type_compare == "double")
            u_type = UniformType::DOB;
         else if (type_compare == "bool")
            u_type = UniformType::BOOL;
         break;
      }
      case 2:{
         if (type_compare == "vec")
            u_type = UniformType::FVEC2;
         else if (type_compare == "ivec")
            u_type = UniformType::IVEC2;
         else if (type_compare == "uvec")
            u_type = UniformType::UIVEC2;
         else if (type_compare == "dvec")
            u_type = UniformType::DVEC2;
         else if (type_compare == "mat")
            u_type = UniformType::MAT2;
         break;
      }
      case 3:{
         if (type_compare == "vec")
            u_type = UniformType::FVEC3;
         else if (type_compare == "ivec")
            u_type = UniformType::IVEC3;
         else if (type_compare == "uvec")
            u_type = UniformType::UIVEC3;
         else if (type_compare == "dvec")
            u_type = UniformType::DVEC3;
         else if (type_compare == "mat")
            u_type = UniformType::MAT3;
         break;
      }
      case 4:{
         if (type_compare == "vec")
            u_type = UniformType::FVEC4;
         else if (type_compare == "ivec")
            u_type = UniformType::IVEC4;
         else if (type_compare == "uvec")
            u_type = UniformType::UIVEC4;
         else if (type_compare == "dvec")
            u_type = UniformType::DVEC4;
         else if (type_compare == "mat")
            u_type = UniformType::MAT4;
         break;
      }
   }
   // Set new pair
   shader.uniforms.emplace(u_name,u_type);
}
void ShaderHandler::subProcessUniformBlock(Shader& shader, std::string src, std::string u_name, std::string type_compare){
   int u_count = 0;
   int struct_end = 0;
   std::string line;
   std::unordered_set<std::string> types_complex({"vec","ivec","uvec","bvec","dvec","mat"});
   int struct_start = src.find("struct");

   // Check if type of struct match with type of uniform
   int struct_type_s = src.find_first_of(" ",struct_start);
   struct_type_s = src.find_first_not_of(" ",struct_type_s);
   int struct_type_e = src.find_first_of(" {\n",struct_type_s);
   while (type_compare.compare(src.substr(struct_type_s,struct_type_e - struct_type_s)) not_eq 0){
      struct_start = src.find("struct",struct_type_e);   
      struct_type_s = src.find_first_of(" ",struct_start);
      struct_type_s = src.find_first_not_of(" ",struct_type_s);
      struct_type_e = src.find_first_of(" {\n",struct_type_s);
   }

   if (struct_start not_eq -1){
      struct_start = src.find("{",struct_start);
      struct_end   = src.find("};",struct_start);
      int size = struct_end - struct_start;
      line = src.substr(struct_start,size+1);// Shrink string to struct
     
      int mem_e =  0;
      int mem_s =  0;
         // Find all struct members
      while (mem_e not_eq -1){
         mem_s = line.find_first_not_of(" \n",mem_e+1);
         mem_e =  line.find(";",mem_s);
         if (mem_e == -1)
            break;
         // Shrink srs to one line
         std::string sub_line = line.substr(mem_s,mem_e - mem_s);
         // Find (Define) type and counter
         type_compare = line.substr(mem_s,mem_e - mem_s);
         int dig_s1 = type_compare.find_first_of("0123456789",0);
         if (dig_s1 not_eq -1){
            auto isExist1 = types_complex.find(type_compare.substr(0,dig_s1));
            // IF it is build in type
            if (isExist1 not_eq types_complex.end()){// Find(Define) Count and type for complex one(RESULT)
               u_count = stoi(type_compare.substr(dig_s1,1));
               type_compare = type_compare.substr(0,dig_s1); 
            }
            else{// Find(Define) Count and type for custom one (RESULT)
               u_count = 1;
               type_compare = type_compare.substr(0,type_compare.find(" "));
            }
         }
         else{// Find Count and type for simple one type (RESULT)
            u_count = 1;
            type_compare = type_compare.substr(0,type_compare.find(" "));
         }
         // Find full name for  uniform
         u_name += '.';// Add . to name
         u_name += sub_line.substr(sub_line.find(" "),mem_e - sub_line.find(" "));// Here if uniforms not after declaration of struct you have problem, fix it
         u_name.erase(std::remove(u_name.begin(),u_name.end(),' '),u_name.end());
         setNewUniform(shader,u_count,u_name,type_compare);//////
         // Clear name for next one
         u_name = u_name.substr(0,u_name.find_first_of('.'));
      }
   }
}
void ShaderHandler::subProcessUniformSimple(Shader& shader, std::string u_name, std::string type_compare){
   setNewUniform(shader,1,u_name,type_compare);
}
void ShaderHandler::subProcessUniformComplex(Shader& shader, std::string u_name, std::string type_compare){
   int dig_s = type_compare.find_first_of("0123456789",0);
   // Find count for type(complex)(RESULT)
   int u_count = stoi(type_compare.substr(dig_s,1));
   // Find type(without number)(RESULT)
   type_compare = type_compare.substr(0,dig_s); 
   setNewUniform(shader,u_count,u_name,type_compare);
}
void ShaderHandler::processUniforms(Shader& shader, const std::string& src){
   std::unordered_set<std::string> types_complex({"vec","ivec","uvec","bvec","dvec","mat"});
   std::unordered_set<std::string> types_simple({"float","double","uint","int","bool","sampler2D"});
   int start = 0;
   int end = 0;


   while (start not_eq -1){
      start = src.find("uniform",end);
      if (start == -1)
         break;

      end   = src.find(";",start);
      std::string line = src.substr(start,end - start);

      // Define temp buffers for store data about uniform
      std::string type_compare;
      std::string u_name;

      // Shrink line to first appearing ' '
      type_compare = line.substr(line.find_first_of(" ",0),line.find_first_of(";",0));
      // Find where first letter appeared
      int type_s1 = type_compare.find_first_not_of(" ",0);
      // Shrink result string to first appearing character(what means first letter for type)
      type_compare = type_compare.substr(type_s1,line.find_first_of(";",0));
      // Find where the last letter for first word(type for uniform)
      int type_e1 = type_compare.find_first_of(" ",0);
      // Shrink line to first appeating ' '(that mean uniform name with probels but anyway)
      u_name = type_compare.substr(type_e1,line.find_first_of(";",0));
      // Find where first letter appeared
      int name_s = u_name.find_first_not_of(" ",0);
      // Shrink name to uniform name(RESULT)
      u_name = u_name.substr(name_s,u_name.size() - name_s);
      u_name.erase(std::remove(u_name.begin(),u_name.end(),' '),u_name.end());
      // Shring type to type name(but with number) 
      type_compare = type_compare.substr(0,type_e1);

      int dig_s = type_compare.find_first_of("0123456789",0);
      if (dig_s not_eq -1 && type_compare.compare("sampler2D") not_eq 0){   
         auto isExist = types_complex.find(type_compare.substr(0,dig_s));
         // IF it is build in type
         if (isExist not_eq types_complex.end()){
            subProcessUniformComplex(shader,u_name,type_compare);
         }
         // IF it's not build in type(struct)
         else{
            subProcessUniformBlock(shader,src,u_name,type_compare);
         }    
      }
      else{ 
         auto isExist = types_simple.find(type_compare);
         // IF it is build in type
         if (isExist not_eq types_complex.end()){
            subProcessUniformSimple(shader,u_name,type_compare);
         }
         else{
            subProcessUniformBlock(shader,src,u_name,type_compare);
         }
      }
   }
}

void ShaderHandler::SetUniform(const Shader &shader, const char* name, const float& arg){
   glUniform1f(glGetUniformLocation(shader.id,name),(float)arg);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const double& arg){
   glUniform1d(glGetUniformLocation(shader.id,name),arg);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const int& arg){
   glUniform1i(glGetUniformLocation(shader.id,name),arg);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const unsigned int& arg){
   glUniform1ui(glGetUniformLocation(shader.id,name),arg);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::vec2& arg){
   glUniform2f(glGetUniformLocation(shader.id,name),arg.x,arg.y);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::vec3& arg){
   glUniform3f(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::vec4& arg){
   glUniform4f(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z,arg.w);
}

void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::ivec2& arg){
   glUniform2i(glGetUniformLocation(shader.id,name),arg.x,arg.y);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::ivec3& arg){
   glUniform3i(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::ivec4& arg){
   glUniform4i(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z,arg.w);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::uvec2& arg){
   glUniform2ui(glGetUniformLocation(shader.id,name),arg.x,arg.y);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::uvec3& arg){
   glUniform3ui(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::uvec4& arg){
   glUniform4ui(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z,arg.w);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::dvec2& arg){
   glUniform2d(glGetUniformLocation(shader.id,name),arg.x,arg.y);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::dvec3& arg){
   glUniform3d(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::dvec4& arg){
   glUniform4d(glGetUniformLocation(shader.id,name),arg.x,arg.y,arg.z,arg.w);
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::mat2& arg){
   glUniformMatrix2fv(glGetUniformLocation(shader.id,name),1,GL_FALSE,glm::value_ptr(arg));
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::mat3& arg){
   glUniformMatrix3fv(glGetUniformLocation(shader.id,name),1,GL_FALSE,glm::value_ptr(arg));
}
void ShaderHandler::SetUniform(const Shader &shader, const char* name, const glm::mat4& arg){
   glUniformMatrix4fv(glGetUniformLocation(shader.id,name),1,GL_FALSE,glm::value_ptr(arg));
}

std::string ShaderHandler::GetType(const UniformType type){
   std::string src;
   switch(type){
      case UniformType::FL:{
         src = "float" ;
         break;
      }
      case UniformType::IN:{
         src = "int" ;
         break;
      }
      case UniformType::UIN:{
         src = "uint" ;
         break;
      }
      case UniformType::DOB:{
         src = "double" ;
         break;
      }
      case UniformType::BOOL:{
         src = "bool" ;
         break;
      }
      case UniformType::FVEC2:{
         src = "vec2" ;
         break;
      }
      case UniformType::FVEC3:{
         src = "vec3" ;
         break;
      }
      case UniformType::FVEC4:{
         src = "vec4" ;
         break;
      }
      case UniformType::IVEC2:{
         src = "ivec2" ;
         break;
      }
      case UniformType::IVEC3:{
         src = "ivec3" ;
         break;
      }
      case UniformType::IVEC4:{
         src = "ivec4" ;
         break;
      }
      case UniformType::DVEC2:{
         src = "dvec2" ;
         break;
      }
      case UniformType::DVEC3:{
         src = "dvec3" ;
         break;
      }
      case UniformType::DVEC4:{
         src = "dvec4" ;
         break;
      }
      case UniformType::UIVEC2:{
         src = "uvec2" ;
         break;
      }
      case UniformType::UIVEC3:{
         src = "uvec3" ;
         break;
      }
      case UniformType::UIVEC4:{
         src = "uvec4" ;
         break;
      }
      case UniformType::MAT2:{
         src = "mat2" ;
         break;
      }
      case UniformType::MAT3:{
         src = "mat3" ;
         break;
      }
      case UniformType::MAT4:{
         src = "mat4" ;
         break;
      }
   }
   return src;
}

}
