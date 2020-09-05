//
//  Texture.hpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/28/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glew.h>
#include "stb_image.h"

class Texture {
public:
    Texture();
    Texture(const char* fileLocation);
    
    bool loadTexture();
    bool loadTextureAlpha();

    void useTexture();
    void clearTexture();

    ~Texture();
    
private:
    GLuint textureId;
    int width, height, bitDepth;
    
    const char* fileLocation;
};

#endif /* Texture_hpp */

