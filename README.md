# terramagna

An old voxel game project I've maintained up until 2015, which utilized GLFW and GLEW to generate an OpenGL context and provide support for vertex buffer objects (VBOs), libPNG and FreeType to handle rasterization of .ttf font files, and libnoise to generate Perlin noise for terrain and other procedurally-generated content.

# An important note
I have open sourced the code base for this project for **documentation purposes** only. Be aware of incomplete code and functionality that I never managed to complete, such as the model animation system and material system. I do not guarantee that this game still functions or compiles (I know myself that it has missing shader files that I was unable to recover) nor that I will continue to maintain it, but you are free to use or reference my code however you like under the MIT license.

My future plans for this project is to rewrite it from scratch with portability in mind. Instead of using the Visual C++ compiler and bundled libraries I will instead use the GNU Compiler Collection (GCC) in tandem with CMake, and port my rendering code over from OpenGL to Vulkan.
