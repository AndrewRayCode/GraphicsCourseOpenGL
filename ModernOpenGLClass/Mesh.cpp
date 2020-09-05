#include <iostream>
#include <GL/glew.h>
#include "Mesh.hpp"

Mesh::Mesh() {
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

// by default in opengl, middle of screen is 0,0, and y is up/down,
// x axis is left/right, and z is depth
//GLfloat vertices[] = {
//    // x,  y,     z     u,    v
//    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//    0.0f, 0.0f, 1.0f,   0.5f, 0.0f, // go into third dimension, new point added
//    1.0, -1.0f, 0.0f,   1.0f, 0.0f,
//    0.0f, 1.0f, 0.0f,   0.5f, 1.0f
//};
//
//// The triangles defining the faces of the pyramid
//unsigned int indices[] = {
//    0, 3, 1,
//    1, 3, 2,
//    2, 3, 0,
//    0, 1, 2
//};
//
//GLfloat colors[] = {
//  1.0f, 0.0f, 0.0f,
//  0.0f, 1.0f, 0.0f,
//  0.0f, 0.0f, 1.0f,
//  1.0f, 1.0f, 1.0f
//};
//
//Mesh *obj = new Mesh();
//obj->createMesh(vertices, indices, 20, 12, colors);

void Mesh::createMesh(
    GLfloat *vertices, unsigned int *vertexData, unsigned int vertexDataLength, unsigned int numberOfVertices /*, GLfloat *colorData */
) {
    indexCount = numberOfVertices;

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexData[0]) * numberOfVertices, vertexData, GL_STATIC_DRAW);
    
    // Create buffers (how many buffers, and ID we want to use). We've created a buffer
    // object inside the VAO
    glGenBuffers(1, &VBO);
    
    // Which buffer we want to bind to. A VBO has multiple targets it can bind to
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Now we want to connect the vertices to the VBO
    // which target to bind to, the size of the data we're passing in (length of buffer)
    // then the data, then GL_STATIC_DRAW or GL_DYNAMIC_DRAW, static meaning we wont change
    // the values in the array. Dynamic lets you change vertex positions while the program is
    // running but it's more complex
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertexDataLength, vertices, GL_STATIC_DRAW);
    
    // Our data is a flat array. How many elements belong to each vertex?
    int columns = 8;

    // - location of attribute we create shader for. We want to point to attribute 0 in shader
    // - size of each value getting passed in
    // - type of these values
    // - if you want to normalize the values (we aren't, we want them to go as is)
    // - with VBOs you can splice data together, you can splice colors and vertices together
    //   in the array we pass in. we can tell it to stride across the data, skip 3, get the next
    //   value, etc. We don't want to stride, it's tightly packed data, so 0 skip
    // - Offset, where data starts, in our case there's no reason to use offset. If you only want
    //   to draw part of the object you can start at a different place
    // Update to this: We increased stride to five, because in the vertex array, we're adding
    // two values to each vertex, the u and v
    int vetexPositionLocation = 0;
    glVertexAttribPointer(vetexPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * columns, 0);

    // In the shader, we want to enable the usage of the location we mentioned above
    glEnableVertexAttribArray(vetexPositionLocation);
    
    // ------ UV from class
    // The last (void*) converts it to a pointer to our value. Sets up for slot 1, for layout= in shader,
    // where you can set the value of the attribute you want to point to.
    // UVs start 3 in from each vertex start
    int uvAttributeLocation = 1;
    glVertexAttribPointer(uvAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * columns, (void*)(sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(uvAttributeLocation);
    
    // ------ Normals from class
    // Normals start 5 in from each vertex start
    int normalAttributeLocation = 3;
    glVertexAttribPointer(normalAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * columns, (void*)(sizeof(vertices[0]) * 5));
    glEnableVertexAttribArray(normalAttributeLocation);

    // bind vbo to nothing (unbinding). Some people like to indent the bind from above to here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
      
    /*
    // ------ Andy adding vertex colors!
    int vertexColorLocation = 2;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData[0]) * 12.0f, colorData, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexColorLocation);
     */

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // He says need to unbind the IBO *afteR* the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // and unbind vao
    glBindVertexArray(0);
    
    /*
     layout(location = 0) in vec3 pos;
     layout(location = 1) in vec2 uv;
     layout(location = 2) in vec3 color;
     layout(location = 3) in vec3 normal;
     */
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
