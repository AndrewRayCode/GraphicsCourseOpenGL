- Textures add extra detal to objects
- Textures can hold generic data, but is more advanced
- Textures can be 2D, but there's also 2D and 3D textures
- Points on textures are "texels," not "pixels". A texel isn't given
  a position based on width and height of image, it's from 0 - 1. (u,v)
  of (0.5, 0.5) = 50% across
- Interpolation across fragments is how we calculate texels

# Generating

Much like creating a VBO / VAO

glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
different like _1D and _3D.

Once you bind to texture you can use it but also write to it.
glTexImage2D(
  - target - what you're bound to
  - "mipmap" level
  - format of stored data (rgb, or rgba, and lots of others)
  - width
  - height of texture
  - always be zero - legacy thing, used to define borders
  - format of data being loaded, different than what's being stored on the
    graphics card
  - data type of values (ints, floats, bytes)
  - data itself
)

# Mipmaps
There are resolution limits for textures
As we get closer to a texture, it becomes more pixelated
As we move away, it will try to draw 2 or more pixels on the same pixels

Solution: Create multiple versions of image at different resolutions
  and switch between them based on distance

Zero above means we're defining the level for mipmap level zero. And if we
do this it will use the texture across the whole range. Can generate mimaps
manually and set them for every range.

# More things: Filters
What if you try to render off-center of texels?
two possibilites:
- use texel with most overlap (pixelated effect)
- linear: weighted average of surrounding texels (blends boundaries)

Linear is more common, a little more computation but not enough to matter

To set filters: `glTexParameteri()`
We pass in integers (enum, but enums are usually integers)
glTexParameteri(
  texture id,
  gl_texture_min_filter - filter that's applied when texture is made smaller
  gl_texture_mag_filter - filter when the texture is made bigger,
  linear filtering: is the filtering of above
)

# Wrap

What happens if the texel is at 1.1? what do we du?
You can repeat, repeat mirror, extend pixels, or apply a colored border
glTexParamater(i) defines its
GL_TEXTURE_WRAP_S is how we handle on "s-axis" (x-axis)
GL_TEXTURE_WRAP_T is how we handle on "t-axis" (y-axis)

# Loading
It would get tedius to write libraries to do the work for us, there are OSS
ones.

# Texture Samplers

Textures in Shaders are accessed via "samplers"
Samplers aren't the texture itself, they consist of an ID which points to
a texture unit, and the texture unit has the texture applied to it(?)

Samplers access textures attached to their texture unit

You attach a texture to a "texture unit" - texture unit is for multiple
textures. Samplers attach to a texture unit. You can attach multiple
samplers to a texture unit.

Textures attach to texture units, samplers read from textures attached
to those texture units.