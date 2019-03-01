//
//  shader.hpp
//  openGL-T9-3D
//
//  Created by Lax Zhang on 2019/2/27.
//  Copyright © 2019 Lax Zhang. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader{
public:
    unsigned int ID;
    
    // 构造函数
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(void);
    // 激活程序
    void use();
    
    void setFloat4(const std::string &name, float value1, float value2, float value3, float value4) const;
    void setBool1(const std::string &name, bool value) const;
    void setInt1(const std::string &name, int value) const;
    void setFloat1(const std::string &name, float value) const;
    void setMat2(const std::string &name, const glm::mat4 &mat) const;
    void setMat3(const std::string &name, const glm::mat4 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setVec3(const std::string &name, const float x, const float y, const float z) const;
};

#endif /* shader_hpp */
