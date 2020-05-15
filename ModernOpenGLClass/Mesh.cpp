#include "Mesh.hpp"
#include <GL/glew.h>

Mesh::Mesh() {
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

void Mesh::createMesh(
    GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, GLfloat *attributes
) {

    indexCount = numOfIndices;

    // ammount of arrays we want to create, and where we want to store
    // the ID of the array. This defines space on the graphics card memory!
    // for one vertex array, and gives you that Id back
    glGenVertexArrays(1, &VAO); // I changed this from 1 to 2 - is 2 right for 2 VBOs?
    
    // Then we "bind" it...? Now any opengl functions we use that interact with VBOs
    // will interact with this VAO ID. Got it
    glBindVertexArray(VAO);
    
    // IBO / EBO = index / element buffer
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfVertices, indices, GL_STATIC_DRAW);
    
    // Create buffers (how many buffers, and ID we want to use). We've created a buffer
    // object inside the VAO
//    GLuint mahBufs[2];
    glGenBuffers(1, &VBO);
    
    // Which buffer we want to bind to. A VBO has multiple targets it can bind to
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Now we want to connect the vertices to the VBO
    // which target to bind to, the size of the data we're passing in (length of buffer)
    // then the data, then GL_STATIC_DRAW or GL_DYNAMIC_DRAW, static meaning we wont change
    // the values in the array. Dynamic lets you change vertex positions while the program is
    // running but it's more complex
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);
                 
    // Wont' make as much sense until we get to shaders
    // - location of attribute we create shader for. We want to point to attribute 0 in shader
    // - size of each value getting passed in
    // - type of these values
    // - if you want to normalize the values (we aren't, we want them to go as is)
    // - with VBOs you can splice data together, you can splice colors and vertices together
    //   in the array we pass in. we can tell it to stride across the data, skip 3, get the next
    //   value, etc. We don't want to stride, it's tightly packed data, so 0 skip
    // - Offset, where data starts, in our case there's no reason to use offset. If you only want
    //   to draw part of the object you can start at a different place
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // In the shader, we want to enable the usage of the location we mentioned above
    glEnableVertexAttribArray(0);
    
    // bind vbo to nothing (unbinding). Some people like to indent the bind from above to here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
      
    // Andy adding vertex colors!
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(attributes[0]) * numOfIndices, attributes, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // and unbind vao
    glBindVertexArray(0);
    
    // He says need to unbind the IBO *afteR* the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::renderMesh() {
    // We're working with this VAO now
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Can have triangles, lines (not color in middle), tris are filled in as middle, and
    // then where in the array should we start, and then how many points to draw
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // count of indices, what type they are, pointer to indices
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // Unbind veretx array
    glBindVertexArray(0);
}

void Mesh::clearMesh() {
    if(IBO != 0) {
        // Clear off graphics card memory
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    if(VBO != 0) {
        // Clear off graphics card memory
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if(VBO1 != 0) {
        // Clear off graphics card memory
        glDeleteBuffers(1, &VBO1);
        VBO1 = 0;
    }
    if(VAO != 0) {
        // Clear off graphics card memory
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    indexCount = 0;
}

Mesh::~Mesh() {
    clearMesh();
}
