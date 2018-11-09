


#include <cuda.h>
#include <iostream>

using namespace std;

__global__ void AddIntsCUDA(int* a, int *b)
{
	a[0] += b[0];
		
}

int main()
{
	//host
	int a = 5, b = 9;
	//device
	int *d_a, *d_b;

	cudaMalloc(&d_a, sizeof(int));
	cudaMalloc(&d_b, sizeof(int));

	cudaMemcpy(d_a, &a, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, &b, sizeof(int), cudaMemcpyHostToDevice);


	AddIntsCUDA<<<1,1>>>(d_a, d_b);

	cudaMemcpy(&a, d_a, sizeof(int), cudaMemcpyDeviceToHost);
	cout << "the answer is "<<a<<endl;

	cudaFree(d_a);
	cudaFree(d_b);

	int c;
	cin >> c;

	return 0;

}

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