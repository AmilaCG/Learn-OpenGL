# Learn-OpenGL
My journey on https://learnopengl.com/  

## End of the Lighting chapter
![Lighting demo](README_resources/LightingDemo.gif)

## Prerequisites on Windows
1. [Git](https://git-scm.com/download/win)
2. OpenGL 3.3 or above supported graphics hardware
3. [Visual Studio](https://visualstudio.microsoft.com/downloads/) 2022 Version 17.6 or above   
   - Newer VS versions (17.6 and above) support vcpkg manifests where all the required dependencies will be auto-downloaded and linked when building
   - Alternatively, you can [use CMake](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started-vs?pivots=shell-cmd) to utilize the [vcpkg.json](vcpkg.json) included in this project
   - If you are using an older VS version and not using the `vcpkg.json`, make sure to [install vcpkg manually](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started?pivots=shell-cmd) and install following packages:
  `glfw3`, `glad`, `glm`, `stb`, `assimp`

## How to set up
* Clone the repository
  ```
  git clone git@github.com:amila93/Learn-OpenGL.git
  ```
* Open `Learn-OpenGL/Learn Opengl.sln` using Visual Studio
