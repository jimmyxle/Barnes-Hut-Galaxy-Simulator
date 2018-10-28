#include "Galaxy.h"

int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------

	int NUM_P = 150;

	Galaxy* g1 = new Galaxy(-0.1, -0.1, 60000, NUM_P );
	
	Galaxy* g2 = nullptr;

	
	g2 = new Galaxy( 0.0 , 0.0 , 50000, NUM_P  );
	g1->add_galaxy(*g2);
	



	//-----------------------
	bool repeat = true;
	int a = 0;

	while (repeat)
	{
		if (g2)
		{
			g1->two_running_display(*g2);
		}
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
