/*
#include "tbb/tbb.h"
#include <iostream>

#include <mutex>

std::mutex mu;

using namespace tbb;

void Foo(float a)
{
	std::lock_guard<std::mutex> lock(mu);
	std::cout << a<<"\n";
}

void ParallelApplyFoo(float a[], size_t n) {
	tbb::parallel_for(size_t(0), n, [&](size_t i) {
		Foo(a[i]);
	});
}

int main(void)
{
	const unsigned int max = 10000;
	float a[max];
	for (int i = 0; i < max; i++)
	{
		a[i] = i;
	}
	std::cout << "hey\n";
	ParallelApplyFoo(a, max);

	int b = 0;
	std::cin >> b;
	return 0;
}
*/

#include "Galaxy.h"
int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------

	

	Galaxy* g1 = new Galaxy(-0.5, -0.5);
	
	Galaxy* g2 = new Galaxy(0.5,0.5);
	g1->add_galaxy(*g2);
	



	//-----------------------
	bool repeat = true;
	int a = 0;

	while (repeat)
	{

		//g1->running_display();

		g1->two_running_display(*g2);




		std::cout << "Enter '1' to continue..." << std::endl;

		std::cin >> a;
		if (a != 1)
		{
			repeat = false;
		}

	}
	
	//----------------------
	
	std::cout << "End" << std::endl;


	
	return 0;
}
