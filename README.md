# University of Wisconsin CS 559 Project 3
### By Matt Kline and Michael Sutherland

## Completed Program Features:

- All basic/essential features as specified on project website:
    - Several moving objects at all times (cars/drones).
    - Multiple different types of behaviors: Drones randomly choose their paths,
      satellite dishes rotate, etc.
    - Multiple different types of buildings/scenery: Buildings were created by
      hand then imported as OBJ files (more on that later).
    - Multiple new textures: Scenery textures were created by hand. Skybox, sun,
      and the "Matrix" overlay for the drones were found online.
    - Technical challenges: loads of them. See below.
    - Articulated objects: The crane and antenna in the corner of the colony are
      articulated.
    - Sufficient frame rate: Frame rate should be sufficient on most discrete
	  graphics cards made in the past several years.
    - Advanced texture mapping: Textures are used extensively for advanced
	  effects. See bwlow.

- Custom GUI using CEGUI instead of the given FLTK library. CEGUI allows the UI
  to be highly customizable, and the UI can be collapsed via the
  "Show Controls" checkbox to allow for a full-window experience free of GUI
  elements when they aren't needed.

##Dependencies:

- This program uses freeglut (<http://freeglut.sourceforge.net/>)
  and CEGUI (<http://www.cegui.org.uk/wiki/index.php/Main_Page>)
  for window rendering (instead of FLTK)

- The Visual Studio solution to compile the project makes the following
  assumptions:
    - CEGUI has been built (or downloaded) in a directory pointed to by the
	  environment variable CEGUI_HOME. The solution searches for CEGUI's headers
	  in CEGUI_HOME\cegui\include\ and library files in CEGUI_HOME\lib
	- Freeglut has been built in a directory pointed to by the environment
	  variable FREEGLUT_HOME. The solution searches for Freeglut headers in
	  FREEGLUT_HOME\include\ and library files in FREEGLUT_HOME\lib\x86

##Keyboard Controls


####Camera controls:
w, s, a, and d move the camera along the XZ plane.

q and e moves the camera up and down along the Y axis.

The arrow keys control pitch and yaw of the camera.

####View controls:
Use the number keys to display different pieces of scene data
used by the deferred lighting system.

1 shows surfaces that aren't using dynamic lighting.

2 shows view space normals.

3 shows normalized depth (fully lit is the near clip plane, dark is the far
  clip plane).

4 Shows colors used for dynamic lighting.

5 Shows the final composited scene.

## Additional Notes
- Comments are formatted in Doxygen (<http://www.stack.nl/~dimitri/doxygen/>)
  style.

- Transform and vector classes as well as MKMath.hpp were taken from an ongoing
  collection of code I've developed. See <https://github.com/slavik262/MKb>
  Their design is based on similar classes in the Irrlicht graphics engine,
  which I have worked with in the past. Irrlicht can be found at
  <http://irrlicht.sourceforge.net/>

- The SceneRenderer concept is inspired by Irrlicht and OGRE
  (http://www.ogre3d.org/). Instead of pushing and popping matrices using the
  OpenGL matrix stack system, absolute matrices are calculated using a
  Scene Node's matrix and that of its parents. This allows scenes of any
  complexity instead of limiting scenes by the matrix stack's size.

## License

(The zlib License)

Copyright (c) 2012 Matt Kline and Michael Sutherland

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
