#include <iostream>
#include <chrono>

int main() {

std::chrono::steady_clock hrc;

auto now_time = hrc.now();

std::chrono::duration<double>  diff = now_time.time_since_epoch();

int now_time_int = (int)((diff.count())*100);

std::cout << now_time_int << std::endl;

return 0;

}
