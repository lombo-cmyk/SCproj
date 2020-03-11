#include <iostream>
#include <thread>
void FunctionThreadInfo(std::string s){
        std::cout << s << "my id is:" <<std::this_thread::get_id();
}

class ThreadClass
{
public:
    static void ClassMethod(std::string s){
        std::cout << s << "my id is:" <<std::this_thread::get_id();
    }
};
struct ThreadStruct{
    void operator()(std::string s) {
        std::cout << s << "my id is:" <<std::this_thread::get_id();
    }
};

int main() {
    ThreadClass watek;
    std::thread t1(FunctionThreadInfo,"\nI'm thread 1, ");
    std::thread t2(ThreadClass::ClassMethod,"\nI'm thread 2, ");
    std::thread t3(ThreadStruct(),"\nI'm thread 3, ");
    std::thread t4([](std::string s){
        std::cout << s << "my id is:" <<std::this_thread::get_id();
        },"\nI'm thread 4, ");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
