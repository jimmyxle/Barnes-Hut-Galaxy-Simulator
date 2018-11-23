Latest:
Fixed the calc_dist.cl kernel. 
did debugging on my opencl memory objects
fixed galaxy behaviour and cleaned up the console output
removed the for loop that resets the forces 


So got to set up openCL1.2 on visualStudio 2017.
linked lib, got my device information, ran .cl files
uploaded samples from https://www.youtube.com/watch?v=YU_pRT-Be0c&list=PLzy5q1NUJKCJocUKsRxZ0IPz29p38xeM-&index=1
we went over reading a char, doing array manipulation, how to use memory fences for local mem.
could also do fences for global and gotta learn about cl_int4 (faster ?)

add these to cuda patch
/*
std::thread cmd_th(&QuadNode::computeMassDistribution, root);
std::thread display_th( &Galaxy::displayParticles, this,std::ref(allParticles), std::ref(window));

cmd_th.join();
display_th.join();
*/

/*
How to link directories and stuff 
go to youtube link:
https://www.youtube.com/watch?v=m0nhePeHwFs&list=PLKK11Ligqititws0ZOoGk3SW-TZCar4dK&index=1


keep this handy:
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0
to easily find incl and lib (use win32)

1. remember to get build dependencies and add CUDA 5.0
2. vcc (incl and lib)
3. linker $(CudaToolkitLibdir)\cudart.lib

go to file and change item type to cuda/c++ (make sure to compile and make .exe file)

*/

latest commit:

fixed the NaN bug where the galaxy would disappear when the x,y,vx,vy would become too big/small
changed two galaxy behaviour
added attract to center
added a large body to the center

A1 submission:

added two galaxy compatability
added some initial multithreading
sluggish at 300 threads

fourth commit
task completed:
fixed opengl bug
fixed tree bug when two particles on top of each other (implement regenade vector)
wrote compute center of mass


to do:
renegades vector
calculate forces 

First commit

tasks completed:
+added opengl code for particles, quadrants
+designed the quadtree nodes
+vector2D class
+particle class

current task:
get subdivide to work 

future:
get the reset() to work properly to remove all ptrs
