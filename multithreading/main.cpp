#include <iostream>
#include <thread>


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
	f();

	return 0;
}

