#include "Galaxy.h"

int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------

	int NUM_P = 1;
	unsigned int solar_mass = 1 * pow(10, 8);
	double initial_x = 0;

	Galaxy* g1 = new Galaxy(-0.1, -0.1, solar_mass , NUM_P, initial_x, 0 );
	
	Galaxy* g2 = nullptr;

	
	g2 = new Galaxy( 0.3, 0.3, solar_mass , NUM_P, -1*initial_x, 0  );
	g1->add_galaxy(*g2);
	


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
