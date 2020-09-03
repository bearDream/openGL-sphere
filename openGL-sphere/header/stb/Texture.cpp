#include "Texture.hpp"


Texture::Texture(const std::string & filepath):
   m_texturepath(filepath),m_localbuffer(nullptr),
   m_width(0),m_height(0),m_bpp(0)
{
    
    stbi_set_flip_vertically_on_load(1);
    m_localbuffer=stbi_load(filepath.c_str(),&m_width,&m_height,&m_bpp,4);
    
    glGenTextures(1,&m_texture_ID);
    glBindTexture(GL_TEXTURE_2D,m_texture_ID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D,     0,GL_RGBA,
                   m_width,m_height,0, GL_RGBA,
                   GL_UNSIGNED_BYTE,m_localbuffer);
                   glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    if(m_localbuffer){
        stbi_image_free(m_localbuffer);
    }

}


void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D,m_texture_ID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D,0);
}
Texture::~Texture(){
    glDeleteTextures(1,&m_texture_ID);
}
