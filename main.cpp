#include "Galaxy.h"

//#include <cuda.h>
//#include <cuda_runtime.h>
//#include <device_launch_parameters.h>

int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------
	//set # planets per galaxy
	int NUM_P = 160;
	//mass of center particle of each
	double solar_mass = 1 * pow(10, 7);
	double solar_mass_2 = 0.5 * pow(10, 7);

	//initial velocity , x variable
	double initial_x = 0.001;
	//double initial_y = 0.005;


	Galaxy* g1 = new Galaxy(0.0, 0.0, solar_mass , NUM_P, -initial_x, 0, 0.2);

	Galaxy* g2 = nullptr;

	/* Uncomment this to add another galaxy */

	
	g2 = new Galaxy( -0.6, -0.6, solar_mass_2 , NUM_P, initial_x, 0, 0.2 );
	g1->add_galaxy(*g2, -1 * initial_x, initial_x);
	


	/**/
	//-----------------------
	bool repeat = true;
	int a = 0;

	while (repeat)
	{
		if (g2)
		{
			g1->two_running_display(*g2);
		}
		else
			g1->running_display();





		std::cout << "Enter '1' to continue..." << std::endl;

		std::cin >> a;
		if (a != 1)
			repeat = false;
		

	}
	
	//----------------------
	
	std::cout << "End" << std::endl;


	
	return 0;
}

