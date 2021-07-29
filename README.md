# Point-Based-Rendering

This program renders scenes of point clouds. Points are rendered as splats to fill holes, and the splates are blended for a smoother appearance. The data is passed through 3 shaders to be rendered: visibility, blending, normalization.

The program is cross platform (Windows and Linux).

********************************************************************************

## Running on Windows

coming soon.

********************************************************************************

## Running on Linux

You will need to define the environment variables: 
* CUDA_INCLUDE_DIRECTORY, 
* CUDA_LIBRARY_DIRECTORY,
* CUDACXX. 
  
Then move to the directory of the project, and:

* mkdir build
* cd build
* cmake ..
* make
* ./pbr


The project was tested on WSL2 (Windows Subsystem for Linux 2), with the following specifications:

* Ubuntu-20.04
* Cuda 11.0
* gcc-9, g++-9
* CMake 3.16.3
* GeForce GTX 950M
* VcXsrv
