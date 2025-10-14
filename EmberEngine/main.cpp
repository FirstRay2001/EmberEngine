#include "Source/Core/Application.h"
#include "test.h"

int main()
{

	Application AppInstance(1600, 1200, "EmberEngine 0.0.1");

	MyTest();

	AppInstance.Run();
	return 0;
}