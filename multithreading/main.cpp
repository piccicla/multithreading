#include <iostream>
#include <thread>

void hello() {
	std::cout << "hello multithreading\n";
}

int main() {

	unsigned long nthreads = std::thread::hardware_concurrency();
	std::cout << "max hardware threads: " << nthreads << std::endl;
	std::thread t(hello);
	std::cout << "thread id: " << t.get_id() <<std::endl;
	t.join();

	return 0;
}