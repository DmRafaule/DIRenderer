#include "layout.hpp"
#include "GL/glew.h"
#include "glfw3.h"
#include "shader.hpp"
#include "DIDebuger/log.hpp"

namespace DI {

    int LayoutHandler::get_layout_size(const std::string& src){
      int result = 0;
      int start = src.find("layout",0);
      int end = 0;

      while (start != -1){
          start = src.find("layout",end);
          if (start == -1)
              break;
          end   = src.find(";",start);
          result++;
       }
       return result;
    }
    void LayoutHandler::Set(const std::string& vertShader){
      std::string src = ShaderHandler::Parse(vertShader);
      std::array<std::string,5> types_simple({"bool","float","int","uint","double"});
      std::array<std::string,6> types_compex({"vec","ivec","uvec","bvec","dvec","mat"});
      int size = get_layout_size(src);
      DI_LOG_INFO("All attribute sends {0}",size); 
      int index = 0;
      int stribe = 0;
      Layout *layout = new Layout[size];

      int start = src.find("layout",0);
      int end = 0;


      while (start != -1){
         start = src.find("layout",end);
         if (start == -1)
            break;

         end   = src.find(";",start);
        int size = end - start;
        std::string line = src.substr(start,size);
        // Find index
        int dig_s = line.find_first_of("0123456789",0);
        if (dig_s != -1){
            int dig_e = line.find_first_not_of("0123456789",dig_s);
            int dig_size = dig_e - dig_s;
            layout[index].index = stoi(line.substr(dig_s,dig_size));
        }
        // Find count for vec and mat
        int type_s = 0;
        auto it1 = std::find_if(std::begin(types_compex),std::end(types_compex),[&](const std::string& str){
            type_s = line.find(str,0);
            return type_s != std::string::npos;
        });
        if (it1 != std::end(types_compex)){
            std::string type = line.substr(type_s,10);
            int dig_s = type.find_first_of("0123456789",0);
            if (dig_s != -1){
                int dig_e = type.find_first_not_of("0123456789",dig_s);
                int dig_size = dig_e - dig_s;
                layout[index].count = stoi(type.substr(dig_s,dig_size));
            }
        }
        else{
            // Find count for int float bool etc ...
            auto it = std::find_if(std::begin(types_simple),std::end(types_simple),[&](const std::string& str){
                return line.find(str,0) != std::string::npos;
            });
            if (it != std::end(types_simple)){
                layout[index].count = 1;
    
            }
        }

        // Find all layouts
        layout[index].type  = GL_FLOAT; 
        layout[index].normalized = GL_FALSE;
        layout[index].entry = sizeof(float) * stribe;
        stribe += layout[index].count;
      
        
        index++;
      }
      for (int i = 0; i < size; i++)
        layout[i].stribe = sizeof(float) * stribe;
      
      for (int i = 0; i < size; i++){
         glVertexAttribPointer(layout[i].index,layout[i].count,layout[i].type,layout[i].normalized,layout[i].stribe,(void*)(layout[i].entry));
         glEnableVertexAttribArray(layout[i].index);

         DI_LOG_INFO("Set vertex attribute index={0} count={1} type=GL_FLOAT norm=GL_FALSE stribe={2} entry={3}",layout[i].index,layout[i].count,layout[i].stribe,layout[i].entry);
      }
      
      glBindVertexArray(0);

      delete[] layout;
    }
   
}
