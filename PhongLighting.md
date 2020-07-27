# Phong Lighting!

3 parts:
- Ambient lighting
- Diffuse light: determined by direction of light source - creates faded effect
  further from the light
- Specular lighting - light reflected prefectly from source to viewer's eye

Combining these three sources = phong lighting. Not technically the same as
phone shading. "Phong reflection model" is another name for the "phong lighting
model".

"Global illumination" simulates light bouncing off of other objects. Not
necessary here.

Ambient light = light color * ambient strength

# Diffuse lighting

Simulates drop-off of lighting as angle becomes more shallow.
Have to take into account the angle of the face

We want to know the angle between the normal (coming directly out of the face)
and the angle to the light source. For a fragment, this determines the diffuse
factor. A smaller angle = brighter, a larger angle = dimmer.

If you normalize both lighting and normal vectors...
  |v1| = |v2| = 1
  So: v1 · v2 = cos(θ)
  cos(0 - 90 degrees) = 0 to 1, which is the same as what we want
  We can use the output of v1 · v2 to calcualte the dropoff
  If the value is negative, we discard

Diffuse lighting requires normals on the object. Could apply them per face,
but because we're using indexed draws, each vertex is only being defined once,
so figuring out which face we're on is confusing (i don't think he explained
this well).

# Phone shading
Phong shading is an interpolation method.
Each vertex has an average of all the normals of all the surfaces it's a part of

This is why a cube with smooth shading looks weird I bet - it averages per
fragment how far along the face you are, and the edges are the average of the
faces the vertex touches, meaning diagonally away!

There's an issue - if you scale your object with non uniform scales, the normals
will be skewed (he says the math is complicated). A solution is to create a
normal matrix from a model matrix. This is from taking model matrix, inverting
it, then tranposing it, then converting it to a 3x3 matrix, which is the normal
matrix. It will scale the normals properly. He links to http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/

# Specular lighting

Relies on the position of the viewer! (the others don't, wow!)

We need to find an angle between the viewer and the light *reflection*.

So bounce light source off object (opposite angle of light source entry) and
calculate difference between viewer position.

"view vector" is difference between fragment position (in 3d space) and camera
position. Just a line from camera to fragment.

"Reflection vector" for reflection has built in glsl function: reflect(incident, normal)
  "Incident" = vector to reflect. Angle of incidence, incoming angle of light source
  normal: normal to reflect around

Just like with diffuse, we can get dot product between light reflection and
viewer reflection vector

And don't take this dot product as is, we raise to a power. The higher the
power, the higher the shininess.

# Type of lights
Directional; Light without position or source. All light is coming in as parallel
rays from a seemingly infinite distance. Best example is the sun.

Point Light: light with a position that shines in all directions, like a light
bulb.

Spot light: similar to point light, but cut down to emit a certain range
with a certain angle. Best example is a flash light. Or, you know, a spot
light.

Area light: More advanced type, where light is coming from an area. Like in
a building where a light is part of a massive square panel.

*Directional* is simplest form of light. It only needs a direction.