#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <string>
#include <GL/glew.h>
class Texture{

   unsigned int m_texture_ID;
   std::string m_texturepath;
   unsigned char * m_localbuffer;
   int m_width,m_height,m_bpp;
public:

   Texture(const std::string & path);
   ~Texture();
   void bind(unsigned int slot=0 ) const;
   void unbind()const;


    
};


#endif
