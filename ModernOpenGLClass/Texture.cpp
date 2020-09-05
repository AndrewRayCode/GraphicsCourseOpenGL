//
//  Texture.cpp
//  ModernOpenGLClass
//
//  Created by Andrew Ray on 6/28/20.
//  Copyright © 2020 Andrew Ray. All rights reserved.
//
#include "Texture.hpp"

Texture::Texture() {
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
}

Texture::Texture(const char* location) {
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = location;
}

bool Texture::loadTextureAlpha() {
    // He claims these are bytes in an unsigned char array. true?
    unsigned char *data = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if(!data) {
        printf("Failed to find %s: %s\n", fileLocation, stbi_failure_reason());
        return false;
    }
    
    // Generating a texture (on gpu memory?)
    glGenTextures(1, &textureId);
    // Different types of textures you can find, in battlefield if you
    // fly through a cloud there are textures on the inside of a cloud. 2d is flat sruface
    // we're applying to our triangles. And remember the first argument to this and below
    // is whatever is currently bound to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // How to wrap a texutre if it wraps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Tries to blend two pixels together. GL_NEAREST is more pixelated. This blends
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // byte = char = 8 bits long
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // automatical create mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Don't need raw data anymore
    stbi_image_free(data);
    
    return true;
}

bool Texture::loadTexture() {
    // He claims these are bytes in an unsigned chara array. true?
    unsigned char *data = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if(!data) {
        printf("Failed to find %s: %s\n", fileLocation, stbi_failure_reason());
        return false;
    }
    
    // Generating a texture (on gpu memory?)
    glGenTextures(1, &textureId);
    // Different types of textures you can find, in battlefield if you
    // fly through a cloud there are textures on the inside of a cloud. 2d is flat sruface
    // we're applying to our triangles. And remember the first argument to this and below
    // is whatever is currently bound to GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // How to wrap a texutre if it wraps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Tries to blend two pixels together. GL_NEAREST is more pixelated. This blends
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // byte = char = 8 bits long
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // automatical create mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Don't need raw data anymore
    stbi_image_free(data);
    
    return true;
}

void Texture::useTexture() {
    // The first argument is the "texture unit". When the shader runs, it will be a "sampler", and the sampler
    // has data for our texture, and it accesses it _through_ the texture unit. 0 is the default so you can
    // leave it out on some graphics cards
    glActiveTexture(GL_TEXTURE0);
    
    // Binds this texture to this texture unit
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::clearTexture() {
    glDeleteTextures(1, &textureId);
}

Texture::~Texture() {
    clearTexture();
}
