# N-Body simulator using the Barnes-Hut algorithm 
### found on: http://arborjs.org/docs/barnes-hut

Simulator that calculates forces between N number of particles. Decreases the number of calculations from N^2 to NLOG(N) by only calculating the force between particles if the distance between particles are smaller than a certain threshold. 

### Software used:
openGL
openCL
CUDA
Intel TBB

### Next goals:
Do openGL calls through openCL's API. 
Support more particles
Add particle collisions
