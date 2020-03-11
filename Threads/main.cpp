#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
static std::mutex locker;
std::atomic_int GlobalIntValue = 0;
//int GlobalIntValue = 0;
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

void IncrementValue(){
    auto start=std::chrono::system_clock::now();
    while(GlobalIntValue<10000000){
        locker.lock();
        GlobalIntValue++;
        locker.unlock();
    }
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;
    locker.lock();
    std::cout << "Execution time for "<< std::this_thread::get_id() <<" is: "<< duration.count() << std::endl;
    locker.unlock();
}

int main() {
    /*Task one*/
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

    /*Task 2*/
    std::thread PrintTh[20];
    for(int j=0; j<20; j++){
        PrintTh[j]=std::thread(Print50Times, ("Thread "+ std::to_string(j)));
    }
    for(int j=0; j<20; j++){
        PrintTh[j].join();
    }

    /*Task 3*/
    std::vector<std::thread> VectorOfThreads;
    for( int i=0; i<10; i++){
        VectorOfThreads.emplace_back(IncrementValue);
    }
    for( int i=0; i<10; i++){
        VectorOfThreads[i].join();
    }
    GlobalIntValue=0;
    IncrementValue();

    return 0;
}
