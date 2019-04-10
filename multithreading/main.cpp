#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>

void hello() {
	std::cout << "hello multithreading\n";
}

class Background_task
{
public:

	void operator()() const  //https://stackoverflow.com/questions/356950/what-are-c-functors-and-their-uses#356993
	{
		std::cout <<"hello from background_task\n";
	}

};



void do_something(int& i)
{
	++i;
}

struct func
{
	int& i;
	func (int& i_):i(i_){}
	void operator()()
	{
		for (int j=0;j<10;++j	)
		{
			do_something(i);  //WARNING: may access to dangling reference if thread is detached!
		}
		std::cout << i << "\n";
	}
};

void oops()
{
	int local_state = 6;
	func my_func(local_state);
	std::thread my_thread(my_func);
	//my_thread.detach();  
	my_thread.join(); //use join to avoid dangling references
}

void do_something_current_thread()
{
	std::cout << "hello current thread\n";
}
void f()
{
	int local_state = 6;
	func my_func(local_state);
	std::thread my_thread(my_func);

	try
	{
		do_something_current_thread();
	}
	catch (...)
	{
		my_thread.join();
		throw;
	}

	my_thread.join(); 
}


class thread_guard
{
	std::thread& t;
	public:
		explicit thread_guard(std::thread& t_):t(t_)
		{
			std::cout << "hello from guard constructor\n";
		}
		~thread_guard()
		{		
			if (t.joinable())
			{	
				std::cout << "guard is joining thread\n";
				t.join();
			}
		}
		thread_guard(thread_guard const&) = delete; //disable copy constructor
		thread_guard& operator=(thread_guard const&) = delete;
};
void f2()
{
	int local_state = 6;
	func my_func(local_state);
	std::thread my_thread(my_func);
	thread_guard g(my_thread);

	do_something_current_thread();

}

class scoped_thread
{
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) :
		t(std::move(t_))   //move ownership 
	{
		if (!t.joinable())
			throw std::logic_error("No thread");
	}
	~scoped_thread()
	{
		t.join();
	}
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
};

void f3()
{
	int some_local_state;
	scoped_thread t(std::thread(func(some_local_state))); //transfer ownership to thread

	do_something_current_thread();
}


void do_work(unsigned id)
{
	std::cout << "hello from " + std::to_string(id) + "\n"; 
}

void f4()
{
	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 20; ++i)
	{
		threads.emplace_back(std::thread(do_work, i));
	}
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join)); //mem_fn to store and execute a member function https://en.cppreference.com/w/cpp/utility/functional/mem_fn
	//for (auto& entry : threads) {
	//	entry.join();
	//}
}


int main() {


	//std::thread t(hello);
	//t.join();


	//p.18
	/*Background_task bop;
	std::thread t(bop);
	std::thread t{ Background_task() };
	t.join();*/

	//p.19
	/*std::thread t(
		[]{
			std::cout << "hello from lambda thread\n";
		}
	
	);
	t.join();*/

	//p.19
	//oops();

	//p.21
	//f();

	//p.22
	//f2();

	//p.28
	//f3();

	//p31
	f4();

	return 0;
}

