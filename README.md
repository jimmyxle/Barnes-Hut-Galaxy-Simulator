
1.build dependencies => cuda 8.0 
2. include + lib
3. $(CudaToolkitLibdir)\cudart.lib
4. type == cuda 

/*
How to link directories and stuff 
go to youtube link:
https://www.youtube.com/watch?v=m0nhePeHwFs&list=PLKK11Ligqititws0ZOoGk3SW-TZCar4dK&index=1


keep this handy:
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0
to easily find incl and lib (use win32)

remember to get build dependencies and add CUDA 5.0
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
