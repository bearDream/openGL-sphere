#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define PI 3.1415926

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/shader/shader.hpp"
#include "header/camera/camera.hpp"

#include <vector>
#include <iostream>
using namespace std;

GLint statcky = 60;    // 横向向切成多少片
GLint stlicex = 60; // 纵向切多少片

std::vector<float> drawglobeVBO(){
    vector<float> c;
    GLfloat R = 0.8f;    // 半径
    GLfloat angleHy = (GLfloat)(2*PI)/statcky;    // 横向每份的角度        算出弧度值
    GLfloat angleZx = (GLfloat)(2*PI)/stlicex;    // 纵向每份的角度        算出弧度值
    GLfloat NumAngleHy = 0;    // 当前横向角度
    GLfloat NumAngleZx = 0;    // 当前纵向角度
    
    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;
    for (float j = 0; j <statcky; j++ )
    {
        for (float i = 0; i <stlicex; i ++)
        {
            NumAngleHy = angleHy*i;
            NumAngleZx = angleZx*j; // 起点都是轴指向的方向。根据右手定则决定转向，只要转向相同，那么两个就合适
            GLfloat x = R*cos(NumAngleHy)*cos(NumAngleZx);    // 记得转化精度
            GLfloat y = R*sin(NumAngleHy);
            GLfloat z = R*cos(NumAngleHy)*sin(NumAngleZx);
            c.push_back(x);
            c.push_back(y);
            c.push_back(z);
        }
    }
    return c;
}
std::vector<int> drawglobeEBO(){
    vector<int> ebo;
    vector<float> vbo = drawglobeVBO();
    int only = vbo.size();
    int num = (int)((only/(3*statcky))*2);
    for (int x = 0; x<stlicex/2;)
    {
        for (int y = 0;y<statcky;y++)
        {
            ebo.push_back(y+x*stlicex);
            ebo.push_back(y+x*stlicex+1);
            ebo.push_back(y+x*stlicex+stlicex);
            ebo.push_back(y+x*stlicex+stlicex+1);
            ebo.push_back(y+x*stlicex+stlicex);
            ebo.push_back(y+x*stlicex+1);
        }
        x = x+1;
    }
    return ebo;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed init Glew." << endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    Shader lampShader("shaders/shader.vs", "shaders/shader.fs");
    vector<float> mmc = drawglobeVBO();
    vector<int> mfc = drawglobeEBO();
    
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mmc.size()*sizeof(float),&mmc[0], GL_STATIC_DRAW);
    // &vertor[0] 是指这个vector的地址。&vector不是指vector地址。
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mfc.size()*sizeof(float),&mfc[0], GL_STATIC_DRAW);
    // &vertor[0] 是指这个vector的地址。&vector不是指vector地址。
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        lampShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        
        // world transformation
        glm::mat4 model;
        model = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        lampShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, mfc.size(), GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}
