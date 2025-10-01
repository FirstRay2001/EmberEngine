#include "Source/Core/Application.h"
#include "test.h"

int main()
{

	Application AppInstance(800, 600, "EmberEngine 0.0.1");

	MyTest();

	AppInstance.Run();
	return 0;
}