#include "list_poolex.h"
#include "elimination_queue.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <time.h>

class Test
{
	virtual void test() {}
	int a = 1;
};

class Test2 : public Test
{
	virtual void test() {}
	int a = 2;
};

class Test3 : public Test
{
	virtual void test() {}
	int a = 3;
};

class Test4 : virtual public Test2, virtual public Test3
{
	virtual void test() {}
	int a = 4;
};

void test(EliminationQueue::TListPoolEx<Test4>& pool)
{
	std::vector<Test4*> v_t;
	for (int i = 0; i < 1000; ++i)
	{
		v_t.push_back(pool.FetchObj());
	}
	pool.Eliminate();
	pool.FetchObj();
	pool.Eliminate();
	pool.FetchObj();
	pool.Eliminate();
	pool.FetchObj();
	pool.Eliminate();
	pool.FetchObj();
	pool.Eliminate();
	pool.FetchObj();
	pool.Eliminate();
	pool.FetchObj();
	std::random_shuffle(v_t.begin(), v_t.end());
	while (v_t.size())
	{
		pool.ReleaseObj(v_t.back());
		v_t.pop_back();
	}
}

void t(const int& ref) {}

int main()
{

	EliminationQueue::Queue<int> q(&t);
	//q.m_pfunOnElimination = t;

	q.Init(1000);

	srand(time(0));

	while (true)
	{
		//test(pool);
		q.AddToQueue(rand());
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}


	return 0;
}

