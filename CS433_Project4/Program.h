/*
Jim Counts and Joshua Pantarotto
CS433 Operating Systems
Project 4 - Due Wednesday April 18
Program.h
*/
#include <vector>
#include <stack>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef PROGRAM_H
#define PROGRAM_H
using namespace std;
/*
| Runs a producer/consumer queue simulation.
*/
class Program
{
	BufferItem buffer[BUFFER_SIZE];
	pthread_mutex_t mutex;
	sem_t empty;
	sem_t full;
	stack<pthread_t> threads;
		
	int tail;
	int head;
	int totalRunTime;
	int consumerCount;
	int producerCount;
	bool running;
			
	/*
	| Gets a string formatted to display the current contents of the buffer.
	| filledBufferSize: The number of used buffer slots.
	| buffer: The buffer.
	| Returns the formatted string.
	*/
	string BufferContentsToString(int filledBufferSize, BufferItem buffer[])
	{
		stringstream ss;
		ss << "Current buffer contents [ ";
		for(int i = 0; i < filledBufferSize; i++)
		{
			ss << buffer[i] << " ";
		}
		
		ss << "]";
		return ss.str();
	}
		
	/*
	| Creates a thread for the specified task
	*/
	void CreateThread(void *(*start_routine)(void*))
	{
		pthread_t      tid;
		int result = pthread_create(&tid, NULL, start_routine, this); 
		if(result == 0)
		{			
			threads.push(tid);
		}
		else
		{
			cout << "Failed to create another thread!" << endl;
		}			
	}
	
	/*
	| Gets the command line argument at the specified position.
	| argc: the number of command line arguments provided to main.
	| argv: the collection of command line arguments.
	| position: the requested argument.
	| defaultValue: the default to use if the requested argument is out of range.
	| Returns the argument if provided, otherwise the specified default value.
	*/
	string GetArgumentAt(int argc, char *argv[], int position, string defaultValue)
	{				
		if(position < argc)
		{
			return string(argv[position]);			
		}
		
		return defaultValue;
	}
		
	/* 
	| Converts a string to an int
	| value: a string containing an integer.
	| Returns the result of converting the string.
	*/
	int ToInteger(string value)
	{
		stringstream ss(value);   
		int i;
		ss >> i;
		return i;
	}

	public:
	/*
	| Initializes a new instance of the Program class.
	*/
	Program()
	{
		head = 0;
		tail = 0;	
		pthread_mutex_init(&mutex, NULL);
		sem_init(&empty, 0, BUFFER_SIZE);
		sem_init(&full, 0, 0);		
		srand(time(NULL));
	}
	
	/*
	| Allows a task that produces items to be removed from the buffer to be assigned
	| to a posix thread.
	| param: a pointer to an instance of the Program class.
	*/
	static void *ConsumerTask(void *param)
	{
		((Program*)param)->InternalConsumerTask();
		pthread_exit(0);
	}
	
	/*
	| Allows a task that produces items to be placed onto the buffer to be assigned to 
	| a posix thread.
	| param: a pointer to an instance of the Program class.
	*/
	static void *ProducerTask(void *param)
	{
		((Program*)param)->InternalProducerTask();
		pthread_exit(0);
	}
	
	/*
	| Simulates a task that removed items from the buffer.
	*/
	void InternalConsumerTask()
	{
		BufferItem item;		
		while(running)
		{
			int sleepTime = rand() % (totalRunTime / 5);			
			usleep(sleepTime);
			
			sem_wait(&full);
			pthread_mutex_lock(&mutex);
			
			if(running)
			{
				if(RemoveItem(&item) != 0)
				{
					cout << "Could not consume an item!" << endl;
				}
				else
				{
					cout << GetConsumptionMessage(item, BUFFER_SIZE, buffer) << endl;	
				}
			}	
			
			pthread_mutex_unlock(&mutex);
			sem_post(&empty);
		}			
	}
	
	/*
	| Simulates a task that produces items and puts them into the buffer.
	*/
	void InternalProducerTask()
	{
		BufferItem item;
		
		while(running)
		{			
			item = CreateItem(totalRunTime);
			usleep(item);

			sem_wait(&empty);
			pthread_mutex_lock(&mutex);
			if(running)
			{
				if(InsertItem(item) != 0)
				{
					cout << "Could not insert [" << item << "]!" << endl;
				}
				else
				{
					cout << GetProductionMessage(item, BUFFER_SIZE, buffer) << endl;
				}
			}
			
			pthread_mutex_unlock(&mutex);
			sem_post(&full);	
		}
			
	}
	
	/*
	| Gets a message used by consumers when they consume an item.
	| item: The consumed item.
	| filledBufferSize: The number of used buffer slots.
	| buffer: The buffer.
	| Returns the message.
	*/
	string GetConsumptionMessage(BufferItem item, int filledBufferSize, BufferItem buffer[])
	{
		stringstream ss;
		ss << "consumer consumed " << item << ". " << BufferContentsToString(filledBufferSize, buffer);
		return ss.str();
	}
	
	/*
	| Gets a message used by producers when they produce an item.
	| item: The produced item.
	| filledBufferSize: The number of used buffer slots.
	| buffer: the buffer.
	| Returns the message.
	*/
	string GetProductionMessage(BufferItem item, int filledBufferSize, BufferItem buffer[])
	{
		stringstream ss;
		ss << "producer inserted " << item << ". " << BufferContentsToString(filledBufferSize, buffer);
		return ss.str();
	}
	
	/*
	| Gets a vector with the current contents of the private buffer.
	*/
	vector<BufferItem> GetBufferContents()
	{
		vector<BufferItem> b;		
		for(int i = 0; i < BUFFER_SIZE; i++)
		{		
			b.push_back(buffer[i]);
		}
		
		return b;
	}	
	
	/*
	| Initializes each element of the buffer to the specified value.
	*/
	void InitializeBuffer(int value)
	{
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			buffer[i] = value;
		}
	}
	
	/*
	| Create a random item.
	| runTime: the total simulation runtime used to scale the size of the items.
	| Returns an item less than or equal to 10% of the total runtime.
	*/
	BufferItem CreateItem(int runTime)
	{
		int r = rand();
		return r % (runTime / 10);
	}
	
	/*
	| Creates a posix thread and assigns the consumer task to it.
	*/
	void CreateConsumerThread()
	{
		void *(*start_routine)(void*) = &Program::ConsumerTask;
		CreateThread(start_routine);
	}
	
	/*
	| Creates a posix thread and assigns the producer task to it.
	*/
	void CreateProducerThread()
	{
		void *(*start_routine)(void*) = &Program::ProducerTask;
		CreateThread(start_routine);
	}
	
	/*
	| Add an item to the buffer.
	| item: The item to add.
	| Returns 0 if successful; otherwise -1 to indicate an error condition.
	*/
	int InsertItem(BufferItem item)
	{		
		int idx = tail++ % BUFFER_SIZE;
		buffer[idx] = item;			
		return 0;
	}
	
	/*
	| Remove an item from the buffer.
	| item: The location to store the removed item in.
	| Returns 0 if sucessful; otherwise -1 to indicate an error condition.
	*/
	int RemoveItem(BufferItem *item)
	{	
		int idx = head++ % BUFFER_SIZE;
		*item = buffer[idx];
		buffer[idx] = -1;		
		return 0;
	}
	
	/*
	| Runs the simulation.
	| argc: The argument count provided to main.
	| argv: The collection of command line arguments.
	*/
	void Run(int argc, char *argv[])
	{
		Start();
				
		SetTotalRunTime(GetRunTime(argc, argv));
		SetProducerCount(GetProducerThreadCount(argc, argv));
		SetConsumerCount(GetConsumerThreadCount(argc, argv));
		
		InitializeBuffer(-1);
		for(int i = 0; i < producerCount; i++)
		{
			CreateProducerThread();
		}
		
		for(int j = 0; j < consumerCount; j++)
		{
			CreateConsumerThread();
		}
		
		usleep(totalRunTime);		
		Stop();
	}
		
	/*
	| Gets a value from the command line indicating how long to run before terminating.
	| argc: The argument count provided to main.
	| argv: The collection of command line arugments.
	| Returns the value provided on the command line if possible; otherwise 0.
	*/
	int GetRunTime(int argc, char *argv[])
	{	
		return ToInteger(GetArgumentAt(argc, argv, 1, "100"));
	}
	
	/* 
	| Gets a value from the command line indicating how many producer threads to create.
	| argc: The argument count provided to main.
	| argv: The collection of command line arguments.
	| Returns the value provided on the command line if possible; otherwise 1.
	*/
	int GetProducerThreadCount(int argc, char *argv[])
	{
		return ToInteger(GetArgumentAt(argc, argv, 2, "1"));
	}
	
	/* 
	| Gets a value from the command line indicating how many consumer threads to create.
	| argc: The argument count provided to main.
	| argv: The collection of command line arguments.
	| Returns the value provided on the command line if possible; otherwise 1.
	*/
	int GetConsumerThreadCount(int argc, char *argv[])
	{
		return ToInteger(GetArgumentAt(argc, argv, 3, "1"));
	}
	
	/*
	| Sets the number of consumer threads.
	| count: the number of threads.
	| Returns the new number of threads that the instance supports.
	*/
	int SetConsumerCount(int count)
	{
		if(count < 128)
		{
			consumerCount = count;
		}
		else
		{
			cout << "Max consumers is 128, using 128." << endl;
			consumerCount = 128;
		}
		
		return consumerCount;
	}
	
	/*
	| Sets the number of producer threads.
	| count: the number of threads.
	| Returns the new number of threads that the instance supports.
	*/
	int SetProducerCount(int count)
	{
		if(count < 128)
		{
			producerCount = count;
		}
		else
		{
			cout << "Max producers is 128. Using 128." << endl;
			producerCount = 128;
		}
		
		return producerCount;
	}
	
	/*
	| Sets the total runtime.
	| runtime: the run time.
	| Returns the new total run time.
	*/
	int SetTotalRunTime(int runtime)
	{
		totalRunTime = runtime;
		return totalRunTime;
	}
	
	/*
	| Gets a value indicating whether the program should run the associated unit tests instead of the 
	| main simulation.
	| argc: The argument count provided to main.
	| argv: The collection of command line arguments.
	| Returns true if the tests should run; otherwise false
	*/
	bool ShouldRunTests(int argc, char *argv[])
	{
		string expected("TEST");		
		if(4 < argc)
		{
			string actual(argv[4]);		
			return expected.compare(actual) == 0;
		}	
				
		return false;
	}
	
	/*
	| Let the threads start.
	*/
	void Start()
	{
		running = true;
	}
	
	/*
	| Stop the threads
	*/
	void Stop()
	{
		pthread_mutex_lock(&mutex);
		running = false;
		pthread_mutex_unlock(&mutex);
		
		cout << "Cancelling threads." << endl;
		while(!threads.empty())
		{			
			pthread_cancel(threads.top());
			threads.pop();			
		}
	}	
};
#endif
