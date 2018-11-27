#include "Galaxy.h"



int main()
{
	std::cout << "Galaxy Simulator" << std::endl;
	//-----------------------
	//set # planets per galaxy
	int NUM_P = 128;
	//mass of center particle of each
	float solar_mass = 1.0e7f;
	float solar_mass_2 =1.0e7f;

	//initial velocity , x variable
	float initial_x = 0.00f;
	float initial_y = 0.007f;


	Galaxy* g1 = new Galaxy(0.-0.5f, -0.5f, solar_mass , NUM_P, initial_x, initial_y, 0.2f);

	Galaxy* g2 = nullptr;

	/* Uncomment this to add another galaxy */

	
	g2 = new Galaxy( 0.5f, 0.5f, solar_mass_2 , NUM_P, initial_x, -initial_y, 0.2f );
	g1->add_galaxy(*g2, -1 * initial_x, initial_x);
	


	/**/
	//-----------------------
	bool repeat = true;
	int a = 0;

	do 
	{
		if (g2)
		{
			g1->two_running_display(*g2);
		}
		else
			g1->running_display();
	} while (!repeat);
	
	//----------------------
	
	std::cout << "End" << std::endl;


	
	return 0;
}

