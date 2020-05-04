# What is GLEW?
- It stands for OpenGL Extension Wrangler
- It acts as an interface for OpenGL versions 1.1, which come with operating
  systems.
- GLEW lets us interface with these higher versions
- With GLEW we can check if extensions exist on platforms

- Lets us interact with openGL itself without having to manually code
  interactions with the graphics card

There are alternatives, GL3W, glLoadGen, glad, glsdk, glbinding, libepoxy, Glee
GLEW is "the standard" one

# Using GLEW
- need to include GL/glew.h
- _After_ initialize OpenGL context, you need to glewExperimental = GL_TRUE,
  which gives us access to the new features
- Then need to init glew, glewInit(); It should return GLEW_OK
- Can check if extensions exist with if GLEW_EXT_ext_name exists - do your thing
- Note "wglew.h" is windows specific

# How do you create the context for glew to work?
# GLFW
- We need a library for handling windows, this is *GLFW*
- This library and "sdl" handle window creation and control and gl context
- Maybe stands for "opengl framework"
- glfw allows for handling input from keyboard, mouse, gamepad
- SDL is more heavyweight version of glfw, like threading and filesystems and
  audio

Alternatives: sfml (sdl but more features), glut (opengl utility toolkit) -
unmaintained. avoid it
