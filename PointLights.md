# Point Lights
Point lights emit in all direction, need to determine direciton vector manually

If you get difference between light position and fragment position, it sould work

There's a formula for attenuation with some constants you need, and
http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation has them

# Spot Lights
similar to point lights
has "cutoff angle", which is angle between direction of light and the edge
of the cone