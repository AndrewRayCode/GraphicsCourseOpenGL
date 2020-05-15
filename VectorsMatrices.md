# Vectors

- Quantity with a magnitude and direction. Often drawn as an arrow
- How far something is and in what direction
- Magnitude = LENGTH, replace in our head

In this class, Z is going into / out of screen. I think that's shader output by
default?

```javascript
// You might see it written x,y,z like this, related to matrices
v = [4,
     6,
     2]
```

# Addition
[1,2,3] + [2,3,4] = [1+2, 2+3, 3+4]

a + b = putting arrows and b end to end, and the result is one starting from
start of a to end of b

a - b is place a and b at same starting point, and resulting vector is the
one from the tip of b going to the tip of a. Result arrow starts at the tip
of the second operand

"scalar" means "single value". Multiplying by scalar = twice as far.
Multiplying two vectors is not easily visualized and not often used

# Dot product
- Also called "scalar product" because it returns a scalar value
- Can be used for reflections, collision detection
- [a,b,c] · [d,e,f] = (a * d) + (b * e) + (c * f). This is adding the multiples of each x,y,z
- Second way to calculate dot product, a·b = |a| * |b| * cos(angle between two vectors)

- |a| is magintude of a, the length of a. Vectors form right angle triangles.
- Uses variation of pythagorean theorm
- In 3D, it's |v| = sqrt(x^2 + y^2 + z+2)
- The longest side of our right triangle is the vector itself

- So if we know v1 · v2 = 30, and we know v1·v2 = |v1| * |v2| * cos(theta)
- and we calculate the two magnitudes....
- (v1 · v2) / (|v1| * |v2|) = cos(theta)
now we can rearrange this equation to get the theta value, the angle between
the two vectors
- theta = cos-1((v1 · v2) / (|v1| * |v2|))

If b is a unit vector (length 1), a · b = how far along b a is, if a is projected
onto b. If |_ a is straight up and b is straight right, dot product is zero.
- We can check if something is at a right angle to a value

# Unit Vectors
Sometimes we only want to know direction, unit vector has length 1
unit vector represented by u
u = v/|v|. This is a vector divided by its magnitude
[1,2,3] / sqrt(1^2 + 2^2 + 3^2)
u has same direction as v, but now magnitude is 1

# Cross product
- Only really works in 3d
- Creates a vector at right angles to two other vectors
(Ax    (Bx    (Ay · Bz - Az · By            
 Ay  *  By  =  Az · Bz - Ax · Bz            
 Az)    Bz)    Ax · By - Ay · Bx)

If you have two vectors with origins together, there are always 2 vectors coming
out at right angles to both of them. Imagine 2 vectors on a piece of flat paper,
the cross product is always the one coming up out of the paper. A x B goes up, B
x A goes down

We'll use it for finding the normals of surfaces, which way the surface is
pointing.

# Matrices

- Group of values in an ixj grid
- i = rows, j = columns. So rows by columns. So matrix[i][j] = row i, column j

Can be added and subtracted. Same as with vectors, values are added to
counterparts in other matrix. Sort of like overlyaing matrices. Dimensions have
to match. We don't add matrices together much

Multiplication is stranger. Scalar multiplication = multiply every cell by value

Multiplying matrices is more complex, and *important* because it's the most
common thing we'll do

A x B = columns on left hand side MUST equal rows on right hand side.

[1,2,3  x [7, 8,
 4,5,6]   [9, 10,
          [11, 12]

The result of this is a 2x2 matrix, because it's where the two matrices are
overlapping

The top left element of this 2x2 is dot product of first row and first column,
 (1,2,3) · (7,9,11). The top right element is first row, SECOND column of right
 side matrix.

[58, 64,
 ..., ...]

The reason we represent vectors in the top to bottom 
```javascript
v = [4,
     6,
     2]
```
We're treating this as a matrix with one column! So now we can multiply by
matrices. So multiplying a matrix by a vector will have the size as the overlap
of the two matrices, which will also be same size as vector!

Multiplying is transforming. Translation, rotation, scaling.

# Identity matrix
[1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0
 0, 0, 0, 1]

All diagonals down the middle. So if you multiply a vector by this matrix, you
get the original vector back. Doing the math, 1000 · 2345 = 2, because it's
(2x1) + and the rest are all zeroes. And this keeps going for 0100 · 2345 = 3

Used as a starting point, then you multiply by other matrices to combine other
transforms.

# Translation matrix
[1, 0, 0, X,
 0, 1, 0, Y,
 0, 0, 1, Z
 0, 0, 0, 1]

translation * vector = [x + X, y + Y, z + Z, 1]
If your vector is [2,3,4,1] and matrix X is 3, then it's 1003 · 2341! So that
extra one at the end of the vector is doin the fucken thing. That's why it's a 1

woah

# Scaling matrix
[sX, 0,  0,  0,
 0,  sY, 0,  0,
 0,  0,  sZ, 0
 0,  0,  0,  1]

Multiplying this by a vector scales it. Makes sense if identity matrix doesn't
change it. Vector becomes
[sX * x, sY * y, sZ * z, 1]

# Rotation matrix
- Rotates a vector, around the origin axis.
- If you want to rotate around an arbitrary point in space, you'd do reverse
  translation first, then rotate, them move back

There are three types of matrices for handling rotation!

x
[1,  0,      0,       0,
 0,  cos(θ), -sin(θ), 0,
 0,  sin(θ),  cos(θ), 0
 0,  0,       0,      1]

# Matrix Transforms
GLM (opeNGL Mathematics) will do most of the transforms for us

# Combining matrices
Combining matrices is as simple as multiplying them.

Order matters! Transforms happen in reverse order. translation * scale, scale
happens first, then translation. Otherwise you'd scale the translation matrix,
if order was reversed.

# Uniforms

```glsl
uniform mat4 model;
```

- need to find location so we can bind value to it
  int location = glGetUniformLocation(shaderId, "uniformVarName")
- Now we can bind to that location, glUniform1f(location. 3.5f)
  if it were a float here
- Make sure your shader program is in use when you call glUniform
- glUniform4f means vec4 of floaing values
- glUniformMatrix4fv, mat4 of floating point values. Ending in v means
  value is specified by a pointers

Uniforms are global to shaders, changed in application external to shaders.

# Uniforms vs Attributes
Attributes are unique per point (vertex)
Uniforms are the same ("uniform") between every shader run


# Main matrices in 3D
View
Model - Model coordinates, assuming every coordinate is starting at 0,0, need
  to convert to where they are in the world, as opposed to where they were built. Model matrix is conversion from model cooridnates to world coordinates
Projection

# Projection matrix
From the triangle video - a triangle that goes from -1,-1, up to top middle at 0,1, if you rotate it 90 degrees on its side on a 4:3 ratio screen, the base is exactly as tall as the screen, because we're not multiplying by projection matrix. the viewport is still -1 - 1

# Interpolation

In vertex shaders, we only work with individual attribute values, but what about values between those?

A "weighted average" is passed to the fragment. It's how it knows to color all the points in between. Important for lightning and textures, for example.

The fragment picked up the interpolated values. It's effectively an estimate, but it's a very accurate estimate.

Interpolation happens during the *rasterization stage.*

# Indexed Draws

- So far we've just define 3 points of our triangle.
- When working with more triangles, there is some annoying repetition.
- A cube made up of trangles = 12 x 3 verts per face, or 36!
- Instead why not define the 8 verts of the cube?
- Also allows for less memory on graphics card

Create an GL_ELEMENT_ARRAY_BUFFER in the VAO. He calles them an "index buffer
object" or "IBO." Can reference vertices by number (index). Sometimes "element"
is called "index", they're the same.

- Can still be a pain, solution is 3d modelling software.

# Projections

"How we see things." 
- Projection matrix is used to convert from "view space" to "clip space."
- Can be used to scene a "3d" look, or an orthographic projection

# Transforming between spaces

"Local space": raw position of each vertex drawn relative to the origin. Local
to the model itself.

(remember model matrix = transforms, translation / rotations / scaling)
"local space * "model matrix" = "world space"
Aka converting it to its location in world space

"World space": position in the world itself (if camera is assumed at origin)

"world space" * "view matrix" = "view space"

"View Space": Position of the vertex in the world relative to the camera position
and orientation.

"view space" * "projection matrix" = "clip space"

"Clip space": Position of the vertex in the world *relative to the camera
position,* as viewd in the area not to be "clipped" from the final output.

"Screen space": Eventually everything is converted into screen space, we don't
define a matrix for this, part of pipeline. Final image is placed in coordinates
of screen itself. 0 at top left, 1 at bottom right.

# Clip Space
To create clip space, we need to define an area (frustum), NOT FRUSTRUM, of what
is not to be clipped with a projection matrix.

Two commonly used projections: ortohgraphic, perspective

# Orthographic Projection
Near Plane and Far plane. Near plane is basically where the camera is. In
orthographic, your lines between near and far planes are parallel. So as things
move towards you, they follow the lines, and there is no illusion of depth.

# Perspective Projection
Near plane is smaller than the far plane. "near clipping plane" and "far
clipping plane". The frustum for perspective projections is a "truncated
pyramid," it's a pyramid (with 4 sides) with the top pointy bit chopped off. I
think the "near plane" (your screen, basically?) is the camera.

Gives the illusion of depth. So if you project lines down this pyramid and an
object is moving closer to the view, it will intersect more and more of the
lines, making it appear larger, giving it the illusion of depth and getting
closer.

glm::perspective outputs a projection matrix given fov, aspect, near, far.
- fov is crocodile mouth up and down angle
- aspect ratio is aspect of viewport
- near = distance of near plane for cutoff
- far = distance of far plane for cutoff

gl_Position = projection * view * model * vec4(pos, 1.0);

Order is important here! Has to be projection, view, model, local
Aka projection has to be at start. Going in reverse order, it's local space to
world space to view space to clip space. We end up in clip space.
