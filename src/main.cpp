#include <iostream>
#include <cstdlib>
#include "final_project.h"

int main()
{
	try
	{
		FinalProject app({
			.windowTitle = "CG",
			.windowWidth = 1280,
			.windowHeight = 720,
			.windowResizable = false,
			.vSync = true,
			.msaa = true,
			.glVersion = {3, 3},
			.backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f}
		});
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
