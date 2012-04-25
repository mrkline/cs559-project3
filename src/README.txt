Project 3, by Michael Sutherland and Matt Kline

Code features:

- The scene is rendered using a scene graph, which allows scene nodes to be
  arbitrarily added to the scene. Scene nodes can be nested with no arbitrary
  limitations, and the transform of a node's parent is used in conjunction with
  its own local transform to calculate its absolute transform.

- Each scene node maintains a list of renderable objects which are rendered
  using its absolute transform. Renderable objects range from cameras to lights
  (only one light is currently supported) as well as actual objects to be drawn.

- The camera system allows for multiple cameras to exist in the scene graph at
  once. The active camera is set via SceneManager::setActiveCamera, making
  switching viewpoints extremely trivial.

- A material system was created to eliminate the need to manually set up
  material parameters for each object. Material objects are generally contained
  by Renderable objects, and the material to render with is set using
  setActiveMaterial. Materials determine whether an object is rendered as a
  solid or a wireframe, whether lighting is enabled, and unlit color, ambient,
  diffuse, and specular coefficients.

- Unlike the framework code, shadows are properly drawn (and occluded by objects
  in front of them) regardless of the order in which objects are drawn.

Dependencies:

- This program uses freeglut (http://freeglut.sourceforge.net/)
  and CEGUI (http://www.cegui.org.uk/wiki/index.php/Main_Page)
  for window rendering (instead of FLTK)

- The Visual Studio solution to compile the project makes the following
  assumptions:
    - CEGUI has been built (or downloaded) in a directory pointed to by the
	  environment variable CEGUI_HOME. The solution searches for CEGUI's headers
	  in CEGUI_HOME\cegui\include\ and library files in CEGUI_HOME\lib
	- Freeglut has been built in a directory pointed to by the environment
	  variable FREEGLUT_HOME. The solution searches for Freeglut headers in
	  FREEGLUT_HOME\include\ and library files in FREEGLUT_HOME\lib\x86
	- CEGUI's SILLY image loading library is in a directory pointed to by the
	  environment variable SILLY_HOME. The solution searches for headers
	  in SILLY_HOME\include and library files in SILLY_HOME\lib\dynamic.
	  Since SILLY is dependency of CEGUI, SILLY_HOME is usually set to
	  CEGUI_HOME\dependencies
	- NVIDIA's Cg has been installed. The solution searches for Cg headers in
	  CG_INC_PATH and library files in CG_LIB_PATH.
	- GLEW has been built or downloaded in a directory pointed to by the
	  environment variable GLEW_HOME. The solution searches for headers in
	  GLEW_HOME\include and library files in GLEW_HOME\lib

Program notes:

- Comments are formatted in Doxygen (http://www.stack.nl/~dimitri/doxygen/)
  style.

- Transform and vector classes as well as MKMath.hpp were taken from an ongoing
  collection of code I've developed. See https://github.com/slavik262/MKb
  Their design is based on similar classes in the Irrlicht graphics engine,
  which I have worked with in the past. Irrlicht can be found at
  http://irrlicht.sourceforge.net/

- The SceneManager concept is inspired by Irrlicht and OGRE
  (http://www.ogre3d.org/). Instead of pushing and popping matrices using the
  OpenGL matrix stack system, absolute matrices are calculated using a
  Scene Node's matrix and that of its parents. This allows scenes of any
  complexity instead of limiting scenes by the matrix stack's size.
