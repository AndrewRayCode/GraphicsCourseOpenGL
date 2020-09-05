//
//  Mesh.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 5/14/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp
#include <GL/glew.h>

class Mesh {
public:
    Mesh();
    
    void createMesh(
        GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices/*, GLfloat *attributes */
    );
    void renderMesh();
    void clearMesh();

    ~Mesh();
    
private:
    GLuint VAO, VBO, VBO1, IBO;
    GLsizei indexCount;
};

#endif
