
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