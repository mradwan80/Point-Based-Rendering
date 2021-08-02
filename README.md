# Point-Based-Rendering

This program renders scenes of point clouds. Points are rendered as splats to fill holes, and the splates are blended for a smoother appearance. The data is passed through 3 shaders to be rendered: visibility, blending, normalization. This three rendering passes technique is used by almost all point based rendering methods, such as [1], [2].

The model used is taken from the dataset available in [3]

The program is cross platform (Windows and Linux).

********************************************************************************

## Running on Windows

coming soon.

********************************************************************************

## Running on Linux

1- You will need to define the environment variables: 
* CUDA_INCLUDE_DIRECTORY, 
* CUDA_LIBRARY_DIRECTORY,
* CUDACXX. 
  
2- Then move to the directory of the project, and:

* mkdir build
* cd build
* cmake ..
* make
* ./pbr


The project was tested on a Linux system, with the following specifications:

* Ubuntu-20.04
* Cuda 11.3
* gcc-9.3, g++-9.3
* CMake 3.16.3
* GeForce RTX 2070 SUPER


and also on WSL2 (Windows Subsystem for Linux 2), with the following specifications:

* Ubuntu-20.04
* Cuda 11.0
* gcc-9, g++-9
* CMake 3.16.3
* GeForce GTX 950M
* VcXsrv

********************************************************************************

## References

[1] GUENNEBAUD G.. PAULIN M.: Efficient screen space approach for hardware accelerated surfel renderjng. In Pmc. of Vuion, Modeling, and Ksualizarim 03 (2003). 

[2] BOTSCH M., HORNUNG A., ZWICKER M., KOBBELT L.: High-quality surface splatting on today’s gpus. In Point-Based Graphics, 2005. Eurographics/IEEE VGTC Symposium Proceedings (june 2005), pp. 17 – 141.

[3] http://buildingparser.stanford.edu/dataset.html
