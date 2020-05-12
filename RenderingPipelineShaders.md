# Rendering pipeline

Things are passed through GPU, and in order to make things as fast as possible,
it does something called "pipelining". Shaders are parts of the rendering pipeline

Rendering pipeline is series of stages that take place in order to render
image to screen. Four stages are programmable via "shaders".
- Vertex shader
- Fragment shader
- Geometry shader
- Tessalation shader (new, won't be in this course, more rare)
- There's also compute shaders, new as of 2012, also not in course

Shaders are GLSL or HLSL for Direct3D
GLSL is based on C

# Rendering pipeline is 9 stages
Some are programmable, sometimes split up into more stages. Wiki splits it up:
1. Vertex specification

These shaders are programmable, if you don't program, data is passed through
2. Vertex shader (programmable)
3. Tessellation (programmable)
4. Geometry Shader (programmable)

End of all vertex operations
5. Vertex post-procesing

Handling of groups of verticies
6. Primitive Assembly

Converting to fragments
7. Rasterization
8. Fragment shader (programmable)

Operations performed on fragments before rendering to screen
9. Per-sample Operations

# Vertex specification
- A vertex is a point in space. A cube has 8 vertices
- A primitive is a simple shape defined of one or more vertices
- sets up data of vertices, doesn't involve primitives, defining shape. Done
  in application (wtf, this is a stage?)
- Uses two main things
  - Vertex Array Object (VAOs) - defines what data a vertex has. VAO says which
    VBOs an object has. VBOs define the data themselves
  - Vertex Buffer Object (VBO) - defines the vertex data itself "This object has
    position vbo, a color vbo" - this information gets stored in ram of graphics
    card
"Can have multiple VBOs attached to a VAO." 
- Store all this data in ram on graphics card so it can be accessed as quickly
  as possible
- Attribute pointers define where and how shaders can access vertex data.

## To create a VAO:
1. Generate a VAO ID. When you create a vertex array object, when you allocate
  data for the vao, you can't directly access it, you just get an id to query
  the graphics card by ID
2. Bind the VAO with that ID. Bind to graphics card, saying this is what I want
   to work with.
3. Generate VBO ID
4. Bind VBO with that ID (now you're working on vbo). Because you've bound VAO
   first, opengl will bind your vbo to the vao.
5. Attach vertex data to VBO
6. Define attribute pointer formatting - is it groups of 3? made of floats, ints?
7. Enable attribute pointer so shader can access it
8. Unbound vao and vbo, ready for next object to be bound. Ready for next object
   to be bound

Still part of vertex specification: Initiating Draw
1. Activate shader program you want to use, so pipeline knows to use code
   from this shader program.
2. Bind VAO of object you want to draw. When you call draw, it draws whatever is
   currently bound.
3. Call glDrawArrays, which kicks everything off
 - Handles vertices, lets you modify vertices if you want (is this the vertex shader?)
 - Vertex shader is NOT optional
 - HAVE to define gl_Position variable as it used by later stages. Passed down
   the pipeline.
 - Can specify other outputs, like can pass on color (varying)
 - Input to vertex shader is the vertex itself, and data

Very simple vertex shader
```javascript
#version 330 // first thing you do is define version
// layout location is new, don't have to define. Layout defines location in
// the shader. Each variable has own ID, don't have to query its position back
// in application code and keep having to ask every single time, so back in
// app code all we do is bind to 0. "in" means input.
layout(locaiton = 0) in vec3 pos;
void main() {
  // gl position requires a vec4 with 4 values, usually 1 is last element
  gl_Position = vec4(pos, 1.0);
}
```

# Next stage: Tessellation
- Allows you to divide up data into smaller primitives
- Good for level of detail problems.
- Newer, opengl 4.0

# Next shader: Geometry Shader
- Handles primitives like triangle (groups of vertices)
- Takes primitives then emits their vertices to create given primitives. Can
  alter data to move values, like for creating explosions, or even creating new
  one
- Could take in a primitive and output points/lines/triangles

# Next stage: Vertex post-processing
Sometimes this stage can be split up, has two main stages
- Transform feedback: Optional stage. Can save output of vertex and geometry
  stage buffers for later use, if you know something isn't going to change. Can
  be used to skip first half of rendering pipeline, if you know for sure
  something won't change, and it's slowing down your program
- Clipping: primitives that won't be visible are removed. Converted from clip
  space to window space.

# Next stage: Primitive assembly
- Vertices are converted into a series of primitives
- If rendering triangles, 6 verts become 2 triangles
- Face culling happens if triangles are facing "away" frm viewer. Deletes side
  you aren't supposed to see

# Next stage is Rasterization
- Converts primitives into "fragments". NOT a pixel, it's data for each pixel
- Vertex for that particular fragment if it were a vertex
- Fragment data is interpolated based on position to vertex

# Next stage: Fragment Shader
- Goes through each fragment and performs operation on it
- Optional, but rare not to use it. Outputs color of pixel
- Simplest opengl programs usually have a vertex and fragment shader

Very simple fragment shader
```javascript
#version 330 // Same version as other shaders
// fragment only has one output. Can be called whatever you wants. will assume
// this is the main out.
// gl_fragcolor is deprecated?? https://stackoverflow.com/a/51459662/743464
out vec4 color;

// Can "catch" anything here with an "in" value
void main() {
  color = vec4(1.0, 0.0, 0.0, 1.0);
}
```

# Last main stage: Per-Sample Operations
- Series of tests to run to see if the fragment should be drawn at all
- Depth test is most important test. used to determine if something is in front
  of something being drawn. So you don't draw overlapping things
- Can do color blending here, if you want to create transparency effects
- Fragment data is written to currently bound Framebuffer. When you're using
  double buffering + swapbuffers function, the one "behind" is the "framebuffer"
- You can create a third framebuffer if you want and not swap it betwen things.
  Won't be drawn. You could draw entire scene to something, and use for CCTV
  footage if you want, draw data to framebuffer, convert it to texture, pass to
  other frame buffer

# Shaders
- A "shader program" is a group of shaders (vertex, tessellation, geometry,
  fragment...) all associated to one another.

## Creating a shader program
1. Create empty program
2. Create empty shaders (vertex/frag)
3. Attach source code to sahders
4. Compile shaders
5. Attach shaders to program
6. "Link" program - creates executables from shaders, now are executable bits of
  code, and then links them, so when one is finished, it knows where to go to
  next
7. Validate program (optional but advised)

You get a shader program ID back. Calling glUseProgram(ShaderId) - all subsequent
draw calls use this shader program
