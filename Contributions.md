Username : pauldin91
Changes were made to function drawglobeVBO to add the first point in the
begining and the end of each slice. The texture was problemating otherwise.

Changes in camera. I wrote my own couple of glfw functions to rotate the 
sphere with the movement of the mouse using quaternions.

Added Texture.hpp, Texture.cpp, container.png in header/stb/ so that texture 
can be applied using images.

Wrote a function textureCoordinates in main.cpp file so that a vector 
with the texture coordinates is returned.

Small changes in vertex and fragment shader so that uniform sampler2D can 
be set and the object to take the color of the texture applied. 