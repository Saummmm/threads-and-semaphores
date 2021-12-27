#include <iostream>
#include "thread.h"
#include "SharedObject.h"
#include "MyShared.h"
#include <unistd.h>
#include "Semaphore.h"

class WriterThread : public Thread {
	public:

		int threadID=0;
		int reportNum=0;
		int delay=0;
		bool flag=false;

		WriterThread(int threadIDd, int delayd):Thread(8*1000) {
			this->threadID = threadIDd;
			this->delay = delayd;
		};

		virtual long ThreadMain(void) override {
			Shared<MyShared> shared("sharedMemory");

			Semaphore read("read");
			Semaphore write("write");

			time_t prevReport;
			time(&prevReport);
			int seconds;

			while(true) {
				write.Wait();

				shared->threadID = this->threadID;
				shared->reportID = this->reportNum += 1;
				
				if (shared->reportID == 1) {
					shared->reportDelay = 0;
				}
				else {
					time_t now;
					time(&now);
					seconds = difftime(now, prevReport);
					prevReport = now;
					shared->reportDelay = seconds;
				}

				sleep(this->delay);
				if (flag) {
					break;
				}

				read.Signal();
			}
			return 0;	
		}
};

int main(void)
{
	std::cout << "I am a Writer" << std::endl;
	
	int delayInput;
	std::string threadInput;
	std::list<WriterThread *> threads;

	Shared<MyShared> shared("sharedMemory", true);

	Semaphore read("read", 0, true);
	Semaphore write("write", 1, true);

	while (true)
	{
		std::cout << "Whould you like to create a thread? (Y/N)" << std::endl;
		std::cin >> threadInput;

		if (threadInput == "Y") {
			std::cout << "What delay would you like?" << std::endl;
			std::cin >> delayInput;

			WriterThread *temp = new WriterThread(threads.size(), delayInput);
			threads.push_back(std::move(temp));
		} else {
			std::list<WriterThread *> :: iterator it;

			for (it = threads.begin(); it != threads.end(); ++it) {
				(*it)->flag;
			}
			break;
		}
	}
	
};


