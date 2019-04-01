#include <iostream>
#include <thread>

void hello() {
	std::cout << "hello multithreading\n";
}

int main() {

	std::thread t(hello);
	t.join();

	return 0;
}