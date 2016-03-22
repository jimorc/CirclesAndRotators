# circlesandrotators
The various versions of the CirclesAndRotators program as discussed on the Using C++ blog (https://usingcpp.wordpress.com)

Each of the branches are set up to match the code discussed in a single post as listed below.

master - Drawing Circles With OpenGL (https://usingcpp.wordpress.com/2016/03/19/drawing-circles-with-opengl/)

Building The Program With Visual Studio 2015
--------------------------------------------

1. You will need the wxWidgets library to build the program. This library is available as both a set of binaries (DLLs) and as source code. I think the binaries are 32-bit only. I built my own libaries from the source code because I wanted a 64-bit Unicode static library version. Note that the Visual Studio project for this program was set up to build with this configuration of the libraries. You will have to make changes as required to the CirclesAndRotators solution to build with any other configuration of the wxWidgets libraries.
2. Fetch the appropriate branch.
3. Open the project in Visual Studio 2015. Select the configuration that you want to build. Make sure that this configuration matches the wxWidgets configuration.
4. Build the project. 

. If you receive a compile error stating that wx/wxprec.h cannot be found, you must set the WXWIN macro to point to the location of your wxWidgets files. For more information, see User Wide Settings in Visual Studio 2015 (https://usingcpp.wordpress.com/2016/03/03/user-wide-settings-in-visual-studio-2015-2/).

. If you receive a compile error stating that setup.h cannot be found, you are attempting to build a configuration of this project that does not match the configuration of your wxWidgets library.

Adapting The Code to GLUT
-------------------------

The code in this project uses wxWidgets to create and display the graphics. It is possible to use the code in CirclesAndRotatorsCanvas.cpp to create a program using GLUT instead as follows:

1. The code in the CirclesAndRotatorsCanvas constructor and any methods called from there should be placed in the init subroutine.
2. The code in CirclesAndRotatorsCanvas::OnPaint should be placed in the display subroutine.
