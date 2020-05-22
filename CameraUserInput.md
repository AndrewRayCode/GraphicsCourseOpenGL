# Camera

"camera", or scene as seen in "view space"

View space is coordinate system with each vertex as seen from the camera

Creating a camera requires
- position of camera
- direction of camera (direction the camera is looking in). Direction vector
  points in opposite direction of intuitive direction. It's because we're moving
  the world around the camera, not the camera around the world. So if the camera
  is at 0,0,1, we have to move the world 0,0,-1 to achieve this effect
- right of camera
- up of camera. Can be derived as a cross product of direction and right vector

Given R = right, U = up, D = direction, P = position, calculating a view marix
is:

[ Rx, Ry, Rz, 0,    [ 1, 0, 0, -Px,
  Ux, Uy, Uz, 0,  *   0, 1, 0, -Py,
  Dx, Dy, Dz, 0,      0, 1, 0, -Pz,
  0,  0,  0,  0 ]     0, 1, 0, 1    ]

GLM does this with glm::lookAt(position, target, up) - up is up of WORLD

Simply apply viewMatrix as projection * view * model * vec4(pos, 1.0)

# Moving the camera

glfwGetKey(window, GLFW_KEY_W) to check if W pressed

# Turning

Pitch: Look up and down. Looking up and down at floor/ceiling with head is
pitching. Like rotating head around X axis
Yaw: Looking left and right. Like rotating head around Y axis.
Roll: Like a plane doing a barrel roll. Turning head to size (around Z axis)

Pitching needs to rotate the view relative to the yaw. Think of looking up
and down and then turning your head and doing the same. Pitch relies on yaw.

Yaw only ever rotates around up axis (Y)

# Calculating Pitch

y (how far up to look) = sin(pitch)
x = cos(pitch)
z = cos(pitch)

# Yaw

Yaw only ever rotates us around Y axis. Only x and z will change.
x = cos(yaw)
z = sin(yaw)

To combine pitch and yaw to get the camera direction vector:

x = cos(pitch) * cos(yaw)
y = sin(pitch)
z = cos(pitch) * sin(yaw)

