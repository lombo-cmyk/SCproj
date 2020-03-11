#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

static std::mutex locker;

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
void Print50Times(std::string s){
    for (int i=0; i<50; i++){
        locker.lock();
        std::cout << "Passed string: " << s << std::endl;
        locker.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(i));
    }
}

int main() {
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
    std::thread PrintTh[20];
    for(int j=0; j<20; j++){
        PrintTh[j]=std::thread(Print50Times, ("Thread "+ std::to_string(j)));
    }
    for(int j=0; j<20; j++){
        PrintTh[j].join();
    }
    return 0;
}
