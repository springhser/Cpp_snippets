#ifndef CYC_THREAD_HPP
#define CYC_THREAD_HPP

namespace CHEN
{
class CycThread
{
public:
	CycThread() {}
	CycThread(std::thread& p_t) : m_t(std::move(p_t)), m_started(false){}
	~CycThread()
	{
		if (m_t.joinable())
		{
			m_t.join();
		}
	}
	void registerFunction(std::function<void()>&& p_fnt)
	{
		m_fnt = std::move(p_fnt);
	}

	void start()
	{
		m_started = true;

		m_t = std::thread([this]() {
			while (m_started)
			{
				m_fnt();
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		});
	}

	void stop()
	{
		m_started = false;
	}

private:
	std::function<void()> m_fnt;
	std::thread m_t;
	std::atomic_bool m_started;
};

}

using namespace CHEN;

class ATest
{
public:
	ATest() {}
	~ATest() {}

	void print()
	{
		std::cout << "Hello world! ATest" << std::endl;
	}
};
class BTest
{
public:
	BTest() {}
	~BTest() {}

	void print()
	{
		std::cout << "Hello world! BTest" << std::endl;
	}
};


void test()
{
	ATest a;
	CycThread t1;
	t1.registerFunction(std::bind(&ATest::print, &a));

	BTest b;
	CycThread t2;
	t2.registerFunction(std::bind(&BTest::print, &b));

	t1.start();
	t2.start();

	int count = 0;
	while (count < 5)
	{
		std::cout << "main thread: " << count << std::endl;
		++count;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}

	//while (true)
	//{
	//	//std::cout << "main thread: " << count << std::endl;
	//	//++count;
	//	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}

	t1.stop();
	t2.stop();

	while (count < 10)
	{
		std::cout << "main thread: " << count << std::endl;
		++count;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}



#endif
