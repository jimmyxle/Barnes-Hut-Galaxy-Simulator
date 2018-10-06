
#include "Galaxy.h"



int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------


	Galaxy* g1 = new Galaxy();



	//-----------------------
	bool repeat = true;
	int a = 0;

	while (repeat)
	{


		g1->running_display();



		std::cout << "Enter '1' to repeat..." << std::endl;

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