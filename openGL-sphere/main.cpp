#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define PI 3.1415926
#include "header/stb/Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include "header/shader/shader.hpp"
#include "header/camera/camera.hpp"

#include <vector>
#include <iostream>
using namespace std;
using namespace glm;
bool pushed=false;
const double width= 1024.0;
const double height=920.0;
double xpos=1024/2.0;
double ypos=920/2.0;
glm::mat4 model;
const vec3 up(0.0,1.0,0.0);
const vec3 eye(4.0,0.0,0.0);
const vec3 viewPos(0.0,0.0,0.0);
glm::mat4 view=lookAt(eye,viewPos,up);
GLint statcky = 60;    // 横向向切成多少片
GLint stlicex = 60; // 纵向切多少片
static void movement(GLFWwindow * window,double xx,double yy);
void mouse(GLFWwindow * window,int button,int action,int mods);
double transform(double t,double a,double b,double c,double d);
std::vector<float> drawglobeVBO(){
    vector<float> c;
    GLfloat R = 0.8f;    // 半径
    GLfloat angleHy = (GLfloat)(2*PI)/statcky;    // 横向每份的角度        算出弧度值
    GLfloat angleZx = (GLfloat)(2*PI)/stlicex;    // 纵向每份的角度        算出弧度值
    // GLfloat NumAngleHy = 0;    // 当前横向角度
    // GLfloat NumAngleZx = 0;    // 当前纵向角度
    float rad_i,rad_j;
    for (int j = 0; j <=statcky; j++ )
    {
        rad_j=M_PI*((float)j/statcky);
        for (int i = 0; i <=stlicex; i++)
        {
            rad_i=2*M_PI*i/stlicex;
            // NumAngleHy = angleHy*i;
            // NumAngleZx = angleZx*j; // 起点都是轴指向的方向。根据右手定则决定转向，只要转向相同，那么两个就合适
            GLfloat x = R*cos(rad_i)*sin(rad_j);    // 记得转化精度
            GLfloat y = R*cos(rad_j);
            GLfloat z = R*sin(rad_i)*sin(rad_j);
            c.push_back(x);
            c.push_back(y);
            c.push_back(z);
        }
    }
    return c;
}
std::vector<float> textureCoordinates(){
    int i,j;
    std::vector<float> texCoords;
    float radi,radj;
         for(j=0;j<=statcky;++j){
             radj=((float)j/(statcky));
            for(i=0;i<=stlicex ;++i){
               radi=((float)i/(stlicex));
               texCoords.push_back(radi);
               texCoords.push_back(radj);
            }
         }
    return texCoords;

}

std::vector<int> drawglobeEBO(){
    vector<int> ebo;
    vector<float> vbo = drawglobeVBO();

    for (int i = 0; i<stlicex;++i)
    {
        for (int j = 0;j<statcky;j++)
        {
            ebo.push_back(j*    (stlicex+1)+i);
            ebo.push_back((j+1)*(stlicex+1)+i);
            ebo.push_back(j    *(stlicex+1)+i+1);
            ebo.push_back(j    *(stlicex+1)+i+1);
            ebo.push_back((j+1)*(stlicex+1)+i);
            ebo.push_back((j+1)*(stlicex+1)+i+1);
        }
        
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
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window,movement);
    glfwSetMouseButtonCallback(window,mouse);

    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed init Glew." << endl;
        return -1;
    }
    glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glEnable(GL_DEPTH_TEST);
    Texture text("header/stb/container.png");
    text.bind();
    Shader lampShader("shaders/shader.vs", "shaders/shader.fs");
    vector<float> mmc = drawglobeVBO();
    vector<int> mfc = drawglobeEBO();
    vector<float> txr=textureCoordinates();
    vector<float> merge;
    for(int i=0;i<mmc.size();++i){
        merge.push_back(mmc[3*i+0]);
        merge.push_back(mmc[3*i+1]);
        merge.push_back(mmc[3*i+2]);
        merge.push_back(txr[2*i+0]);
        merge.push_back(txr[2*i+1]);

    }
    
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    
    
     unsigned int VBO,cubeVAO,EBO;
     glGenVertexArrays(1,&cubeVAO);
     glGenBuffers(1,&VBO);
     glBindBuffer(GL_ARRAY_BUFFER,VBO);
     glBufferData(GL_ARRAY_BUFFER,merge.size()*sizeof(float),&merge[0],GL_STATIC_DRAW);
     glBindVertexArray(cubeVAO);
     glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(const void *)0);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(const void *)(3*sizeof(float)));
     glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mfc.size()*sizeof(float),&mfc[0], GL_STATIC_DRAW);
    // &vertor[0] 是指这个vector的地址。&vector不是指vector地址。
    
    
       glm::mat4 proj=perspective(radians(60.0),4.0/3.0,0.1,20.0);
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        lampShader.use();
        text.bind();
        lampShader.setInt1("u_Texture",0);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lampShader.setMat4("projection", proj);
        lampShader.setMat4("view", view);
        
        // world transformation
        
        lampShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, (int)mfc.size(), GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}


static void movement(GLFWwindow * window,double xx,double yy){
        
        float theta=transform(xx-xpos,0,width,-M_PI,M_PI);
        float phi=-transform(yy-ypos,0,height,-M_PI/2.0,M_PI/2.0);
        glm::vec3 nrm=vec3(cos(theta)*sin(phi),cos(phi),sin(theta)*sin(phi));
        glm::quat q(0.0,-nrm);
        model = mat4_cast(q);
 
}
void mouse(GLFWwindow * window,int button,int action,int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS){
        
        pushed=true;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_RELEASE){
        pushed=false;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_RELEASE){
        

    }
}
double transform(double t,double a,double b,double c,double d){
  return c+(d-c)*(t-a)/(b-a);
}
