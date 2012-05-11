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

- Our largest and most impressive technical feature is deferred shading. With
  traditional, forward shading, all lighting calculations are done as objects
  are rendered to the screen. This increases light calculation time as scene
  complexity increases, and there is no guarantee that lit pixels won't be
  occluded by objects closer to the camera. An increasingly popular alternative
  is deferred shading. In deferred shading, each object writes data such as
  depth, normals, and diffuse and specular coefficients to intermediate
  textures. Once all objects have been drawn, the final lit image is constructed
  using shaders as a screen-wide post-process. Deferred shading has the
  following advantages over forward shading:
  
    - Lighting calculations are not spent on occluded pixels

    - Lights can be added to the scene without the need for additional
      transform and rasterization passes, allowing deferred rendering to
      quickly handle many more light sources than forward shading.

    - For point and spot lights, stencil operations can be used to perform
      lighting calculations only on pixels affected by the light, further
      speeding up lighting passes.

  It also has the following disadvantages over forward shading:

    - Because it stores screen-size intermediate textures, the memory overhead
      for deferred shading is massive. Even our demo with a resolution
      of 1024x768 requires several megabytes of texture data.

    - Issues related to the accuracy of the depth buffer in forward shading
      are exacerbated in deferred shading, since 3D position for each pixel is
      often recalculated using the depth buffer in order to avoid having to
      save pixel position as an intermediate.

    - Deferred shading has even more problems with transparent objects than
      forward shading, since lighting is calculated based only on the closest
      pixels to the camera in the scene. Deferred systems usually get around
      this problem by using traditional forward shading techniques for
      transparent objects.
        
  Our deferred shading technique uses MRT (see below) to store the following
  information at each pixel (all textures are 8-bit per channel RGBA textures):

    - One texture stores colors unaffected by dynamic lighting
      (such as emissive light and skybox colors).

    - One texture uses the first three channels to store view space normals.
      The fourth channel stores normalized depth, interpolating between
      the near to far clip planes.

    - One texture stores the color to be used in dynamic lighting in the first
      three channels and a specular coefficient in the fourth. Ambient,
      diffuse, and specular lighting is all supported.

  See View Controls for information on viewing the intermediates in real time.
  Deferred shading is a complex system since it essentially replaces the entire
  lighting pipeline provided by OpenGL. Most of the technical features listed
  below are used by our deferred shading system. Currently only directional
  lights are supported (see Planned Features for details).

- Shader support: Our project features full vertex and fragment shader support.
  Deferred shading depends heavily on shaders, but using shaders also gives us
  an incredible amount of flexibility for material options. We use NVIDIA's Cg
  shader system.

- Render to Target (RTT) - In order to store intermediates needed for deferred
  shading, our engine supports the use of OpenGL Framebuffer Objects (FBOs) to
  render to textures, not just the screen frame buffer.

- Multiple Render Targets (MRT) - A subset of RTT, MRT allows us to render to
  several render targets at once, allowing us to write all of our intermediate
  data in a single transform and rasterization pass. Once again, FBOs are used.

- Post Processing - The final scene is built using two screen-sized textures.
  First, pixels not affected by lighting are copied to one of the textures, and
  ambient lighting is added. Then each light is applied one at a time, adding to
  the previous result. Other post process effects such as bloom, blur, or depth
  of field were planned but not implemented due to time constraints (see Planned
  Features).

- Sky boxes - Each face of the sky box has its own material (see the description
  of the material system below), and is therefore completely customizable with
  shaders and multiple textures.

- OBJ support - 3D models can be loaded from OBJ files exported from programs
  such as 3DS Max or Blender.

- Texture loading - Textures can be loaded from PNG or JPEG images.

- World loading - The layout of the colony and the navigation graph for the
  drones are both loaded from files (based on the OBJ format). This allows
  modification to the world without recompiling.

- The demo is built around an extensible graphics engine which supports all of
  the above features as well as:

    - The use of C++11 smart pointers to simplify memory management and help
      prevent memory leaks

    - A scene graph system which allows objects to be arranged and transformed
      in arbitrary heirarchies.

    - Renderable objects, varying from lights to cameras to meshes, which can
      be attached to scene nodes. This makes switching cameras and adding new
      objects extremely simple.

    - A material system. Materials are attached to renderable objects and
      contain textures, lighting parameters, shaders, and a callback for
      setting shader parameters.

    - A generic animation system which issues animation callbacks with the
      change in time since the last callback.

  Code specific to the demo is fairly small; the engine was designed for future
  general-purpose use.

- Custom GUI using CEGUI instead of the given FLTK library. CEGUI allows the UI
  to be highly customizable, and the UI can be collapsed via the
  "Show Controls" checkbox to allow for a full-window experience free of GUI
  elements when they aren't needed.

##Planned Features
The following features were originally planned for the demo. Most could be very
easily implemented given the current state of the graphics engine and were cut
only due to time constraints.

- Point and Spot lights - Unlike directional lights, which are implemented as
  full-screen effects, point and spot lights in deferred shading are best
  rendered by using a stencil buffer technique similar to shadow volumes. This
  allows the light to only be drawn in areas of the screen where it intersects
  with the scene geometry. Original design plans called for dozens of point
  lights to be circling the colony's central tower as a demonstration of how
  well deferred shading supports a high number of lights. Problems with
  performing stencil operations in shaders and a lack of time led to this being
  cut. See the `point_lights` branch for current progress in this area.

- Additional Post Processing - Modern video games and 3D graphics applications
  often feature screen-wide post processing effects such as bloom, depth of
  field, and motion blur. Such effects could be easily implemented using the
  post processing system already in place for deferred shading.

- Specular maps - Since the deferred system already has a channel reserved for
  a specular coefficient, using a specular map to control shininess would be
  simple to implement. The only things stopping us from doing so was the time it
  would have taken to create the textures.

##Dependencies:

- This program uses freeglut (<http://freeglut.sourceforge.net/>) as a GLUT
  implementation.

- CEGUI (<http://www.cegui.org.uk/wiki/index.php/Main_Page>) is used for window
  rendering (instead of FLTK).

- Texture loading is done using CEGUI's image loading library, SILLY.

- Shaders functionality is provided with NVIDIA's Cg
  (<http://developer.nvidia.com/cg-toolkit>).

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

- The example code was not used for two reasons:

    1. I had already built a good deal of the graphics engine for the previous
       project

    2. Using deferred shading cannot be done without extensive use of shaders.
       Since the example code is built around the fixed-function pipeline, using
       it seemed like a poor choice.

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
