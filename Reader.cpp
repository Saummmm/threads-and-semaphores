#include <iostream>
#include <unistd.h>
#include "SharedObject.h"
#include "MyShared.h"
#include "Semaphore.h"

int main(void)
{
	std::cout << "I am a reader" << std::endl;

	Shared<MyShared> shared("sharedMemory", false);

	Semaphore write("write");
	Semaphore read("read");

	while (true)
	{
		read.Wait();

		std::cout << "Reader Thread: " << shared.get()->threadID << " " << shared.get()->reportID << " " << shared.get()->reportDelay <<std::endl;
		
		write.Signal();
	}
	
}
