#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"

namespace DI{

extern DebugData* eg_debugData;

void TextureHandler::Set(Texture &texture, std::string path){
    // Create texture obj
    texture.path = path;
    texture.slot = 0;

    // For now texture handler
    glGenTextures(1,&texture.id);
    glBindTexture(GL_TEXTURE_2D,texture.id);
    // Define texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,texture.wrap_s);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,texture.wrap_t);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture.min_filt);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture.mag_filt);
    // Flip texture
    stbi_set_flip_vertically_on_load(true);  
    // Loading image
    texture.data = stbi_load(texture.path.c_str(),&texture.width,&texture.height,&texture.chanels,0);
    if (texture.data){
        if (texture.chanels == 4){
            texture.mode = GL_RGBA;
        }
        else if (texture.chanels == 3){
            texture.mode = GL_RGB;
        }
        // Bind image to texture
        glTexImage2D(GL_TEXTURE_2D,0,texture.mode,texture.width,texture.height,0,texture.mode,GL_UNSIGNED_BYTE,texture.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture.data);
        eg_debugData->counterDITextures_inMem++;
    }
    else{
        DI_LOG_ERROR("STB CALLBACK: fail to load image. Path:{0}",texture.path);
    }
}
void TextureHandler::SetFilters(Texture &texture,unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filt, unsigned int mag_filt){
    texture.wrap_s   = wrap_s;
    texture.wrap_t   = wrap_t;
    texture.min_filt = min_filt;
    texture.mag_filt = mag_filt;
}
void TextureHandler::Use(Texture &texture,const Shader& shad){
    // Bind texture to polygon
    eg_debugData->counterDITextures_inUse++;
    ShaderHandler::SetUniform(shad, texture.sampler.c_str(), (int)texture.slot);
    glActiveTexture(GL_TEXTURE0 + texture.slot);
    glBindTexture(GL_TEXTURE_2D,texture.id);
}

}
