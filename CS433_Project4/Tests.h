/*
Jim Counts and Joshua Pantarotto
CS433 Operating Systems
Project 4 - Due Wednesday April 18
Test.h
*/
#include <vector>
#include "Buffer.h"
#include "TestFramework.h"
#include "Program.h"

#ifndef TESTS_H
#define TESTS_H
class Tests
{
	TestFramework tf;
	int sharedState;
		
	public:			
	// ShouldRunTests should return true when "TEST" is passed on the command line.
	void GetTestParameter()
	{
		string args[] = { "Program", "0", "0", "0", "TEST" };
		int argc = sizeof(args) / sizeof(string);		
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);		
		
		Program p;		
		tf.ASSERT_TRUE(p.ShouldRunTests(argc, argv), "GetTestParameter");	
	}
	
	// ShouldRunTests should return false when "TEST" is not passed on the command line.
	void DoNotGetTestParameter()
	{
		string args[] = { "Program", "0", "0", "0" };
		int argc = sizeof(args) / sizeof(string);
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);
		
		Program p;		
		tf.ASSERT_TRUE(!p.ShouldRunTests(argc, argv), "DoNotGetTestParameter");
	}
	
	// ShouldRunTests should return false when some other value is passed on the command line.
	void NotTestParameter()
	{			
		string args[] = { "Program", "0", "0", "0", "FOO" };
		int argc = sizeof(args) / sizeof(string);
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);
		
		Program p;
		tf.ASSERT_TRUE(!p.ShouldRunTests(argc, argv), "NotTestParameter");
	}
	
	/*
	| Get how long to sleep before terminating from the command line.	
	*/
	void GetRunTime()
	{
		string args[] = { "Program", "100", "0", "0" };
		int argc = sizeof(args) / sizeof(string);
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);
		
		Program p;
		int runTime = p.GetRunTime(argc, argv);
		tf.ASSERT_INT_EQUALS(100, runTime, "GetRunTime");
	}
	
	/*
	| Get the number of producer threads to create.
	*/
	void GetProducerThreadCount()
	{
		string args[] = { "Program", "100", "5", "0" };
		int argc = sizeof(args) / sizeof(string);
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);
		
		Program p;
		int producerCount = p.GetProducerThreadCount(argc, argv);
		tf.ASSERT_INT_EQUALS(5, producerCount, "GetProducerThreadCount");
	}
	
	/*
	| Get the number of consumer threads to create.
	*/
	void GetConsumerThreadCount()
	{
		string args[] = { "Program", "100", "5", "10" };
		int argc = sizeof(args) / sizeof(string);
		char *argv[argc];
		tf.ToCStrArray(argc, args, argv);
		
		Program p;
		int consumerCount = p.GetConsumerThreadCount(argc, argv);
		tf.ASSERT_INT_EQUALS(10, consumerCount, "GetConsumerThreadCount");
	}
	
	/*
	| The buffer size should be five.
	*/
	void BufferSize()
	{
		tf.ASSERT_INT_EQUALS(5, BUFFER_SIZE, "BUFFER_SIZE");
	}
	
	/*
	| Add item to buffer on insert.
	*/
	void AddBufferItemOnInsert()
	{
		BufferItem item = 43;
		vector<BufferItem> expectedContents;
		expectedContents.push_back(item);
		expectedContents.push_back(-1);
		expectedContents.push_back(-1);
		expectedContents.push_back(-1);
		expectedContents.push_back(-1);
		Program p;
		p.InitializeBuffer(-1);
		int result = p.InsertItem(item);
		tf.ASSERT_INT_EQUALS(0, result, "InsertItem result");
		tf.ASSERT_INT_SEQUENCE_EQUALS(expectedContents, p.GetBufferContents(), "Buffer contents after InsertItem");
	}
	
	/*
	| Take item from buffer on remove
	*/ 
	void TakeBufferItemOnRemove()
	{
		BufferItem item;
		vector<BufferItem> expectedContents;
		expectedContents.push_back(-1);
		expectedContents.push_back(41);
		expectedContents.push_back(-1);
		expectedContents.push_back(-1);
		expectedContents.push_back(-1);
		Program p;
		p.InsertItem(43);
		p.InsertItem(41);
		int result = p.RemoveItem(&item);
		tf.ASSERT_INT_EQUALS(0, result, "RemoveItem result");
		tf.ASSERT_INT_EQUALS(43, item, "Removed item");
		tf.ASSERT_INT_SEQUENCE_EQUALS(expectedContents, p.GetBufferContents(), "Buffer contents after RemoveItem");
	}
	
	/*
	| Initalize buffer to a specific value.
	*/
	void InitializeBuffer()
	{
		vector<BufferItem> expectedContents;
		expectedContents.push_back(100);
		expectedContents.push_back(100);
		expectedContents.push_back(100);
		expectedContents.push_back(100);
		expectedContents.push_back(100);
		Program p;
		p.InitializeBuffer(100);
		tf.ASSERT_INT_SEQUENCE_EQUALS(expectedContents, p.GetBufferContents(), "InitializeBuffer");
	}
	
	/*
	| Non static method actually used to set the flag from a thread.
	*/
	void InternalRunner()
	{
		sharedState = 1;
	}
	
	/*
	| Sets a flag if run by a thread.
	*/
	static void *runner(void *param)
	{
		((Tests*)param)->InternalRunner();
		pthread_exit(0);
	}
	
	/*
	| Create a thread.
	*/
	void CreateThread()
	{					
		pthread_t tid;            // Thread id.
		pthread_attr_t attr;      // Set of thread attributes.
		
		sharedState = 0;
		pthread_attr_init(&attr);							 // Get the default attributes.
		pthread_create(&tid, &attr, Tests::runner, this);   // Create the thread.
		pthread_join(tid, NULL);							 // Wait for the thread.
		tf.ASSERT_INT_EQUALS(1, sharedState, "Run Thread");
	}
	
	/*
	| Create a producer thread.
	*/
	void CreateProducerThread()
	{
		Program p;
		p.InitializeBuffer(-1);
		p.SetTotalRunTime(5000);
		p.CreateProducerThread();			
		usleep(5000);					
	}
	
	/*
	| Verify the message used when a producer produces an item.
	*/
	void ProductionMessage()
	{
		BufferItem buffer[BUFFER_SIZE] = { 4, 5, 2};
		string expected = "producer inserted 2. Current buffer contents [ 4 5 2 ]";
		Program p;
		string actual = p.GetProductionMessage(2, 3, buffer);
		tf.ASSERT_STRING_EQUALS(expected, actual, "ProductionMessage");
	} 
			
	/*
	| Randomly produced items should be less than or equal to 10% of the total runtime.
	*/
	void CreateItem()
	{
		int runTime = 500;
		int itemMax = 500 / 10;
		Program p;
		BufferItem item = p.CreateItem(runTime);
		tf.ASSERT_TRUE(item <= itemMax, "CreateItem");
	}
	
	/*
	| Create a consumer thread.
	*/
	void CreateConsumerThread()
	{
		Program p;
		p.InitializeBuffer(-1);
		p.InsertItem(43);
		p.InsertItem(1);
		p.InsertItem(17);
		p.InsertItem(2);
		p.InsertItem(499);
						
		p.SetTotalRunTime(5000);		
		p.CreateConsumerThread();
		usleep(5000);	
	}
	
	/*
	| Create a producer and consumer thread.
	*/
	void CreateProducerAndConsumer()
	{
		int runTime = 5000;
		Program p;
		p.InitializeBuffer(-1);
		p.SetTotalRunTime(runTime);
		p.CreateProducerThread();
		p.CreateConsumerThread();
		//p.CreateConsumerThread();
		usleep(runTime);		
	}
	
	/*
	| Verify the message used when a consumer consumes an item.
	*/
	void ConsumptionMessage()
	{
		BufferItem buffer[BUFFER_SIZE] = { 4, 5, 2 };
		string expected = "consumer consumed 2. Current buffer contents [ 4 5 2 ]";
		Program p;
		string actual = p.GetConsumptionMessage(2, 3, buffer);
		tf.ASSERT_STRING_EQUALS(expected, actual, "ConsumptionMessage");
	}
	
	/*
	| Set the number of consumer threads.
	*/
	void SetConsumerCount()
	{
		int expectedCount = 5;
		Program p;
		int actualCount = p.SetConsumerCount(expectedCount);
		tf.ASSERT_INT_EQUALS(expectedCount, actualCount, "SetConsumerCount");
	}
	
	/*
	| Set the total runtime for the simulation.
	*/
	void SetTotalRunTime()
	{
		int expectedRunTime = 5000;
		Program p;
		int actualRunTime = p.SetTotalRunTime(expectedRunTime);
		tf.ASSERT_INT_EQUALS(expectedRunTime, actualRunTime, "SetTotalRunTime");
	}
	
	void RunTests()
	{
		GetTestParameter();		
		DoNotGetTestParameter();
		NotTestParameter();
		GetRunTime();
		GetProducerThreadCount();
		GetConsumerThreadCount();
		BufferSize();
		AddBufferItemOnInsert();
		TakeBufferItemOnRemove();
		InitializeBuffer();
		CreateThread();
		ProductionMessage();
		CreateProducerThread();
		CreateItem();
		CreateConsumerThread();
		ConsumptionMessage();
		SetConsumerCount();
		SetTotalRunTime();
		CreateProducerAndConsumer();
		cout << tf.GetPassCount() << "/" << tf.GetPassCount() + tf.GetFailCount() << endl;
	}
};
#endif
