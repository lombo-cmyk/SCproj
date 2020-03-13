#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
static std::mutex locker;
std::atomic_int GlobalAtomicValue;
int GlobalIntValue;
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
    const int NumberOfPrints = 50;
    for (int i=0; i<NumberOfPrints; i++){
        std::lock_guard<std::mutex> lock(locker);
        std::cout << "Passed string: " << s << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(i));
    }
}

void IncrementIntValue(){
    auto start=std::chrono::system_clock::now();
    while(GlobalIntValue<10000000){
        std::lock_guard<std::mutex> lock(locker);
        GlobalIntValue++;
    }
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;
    std::lock_guard<std::mutex> lock(locker);
    std::cout << "GLOBAL INT Execution time for "<< std::this_thread::get_id() <<" is: "<< duration.count() << std::endl;
}

void IncrementAtomicValue(){
    auto start=std::chrono::system_clock::now();
    while(GlobalAtomicValue++<10000000){
        //GlobalAtomicValue++;
    }
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;
    std::lock_guard<std::mutex> lock(locker);
    std::cout << "ATOMIC Execution time for "<< std::this_thread::get_id() <<" is: "<< duration.count() << std::endl;
}

void IncrementUnsynchronizedValue(){
    auto start=std::chrono::system_clock::now();
    while(GlobalIntValue<10000000){
        GlobalIntValue++;
    }
    auto stop=std::chrono::system_clock::now();
    std::chrono::duration<double> duration=stop-start;
    std::lock_guard<std::mutex> lock(locker);
    std::cout << "UNSYNCHRONIZED Execution time for "<< std::this_thread::get_id() <<" is: "<< duration.count() << std::endl;
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
    //std::thread PrintTh[20];
    std::array<std::thread, 20> PrintTh;
    const uint8_t NumberOfPrints=20;
    for(int j=0; j<NumberOfPrints; j++){
        PrintTh[j]=std::thread(Print50Times, ("Thread "+ std::to_string(j)));
    }
    for(auto& Thread : PrintTh){
        Thread.join();
    }

    /*Task 3*/
    /*Increasing Value with Atomic*/
    std::vector<std::thread> VectorOfThreads;
    const uint8_t NumberOfThreads=10;
    for( int i=0; i<NumberOfThreads; i++){
        VectorOfThreads.emplace_back(IncrementAtomicValue);
    }
    for( auto& Thread : VectorOfThreads){
        Thread.join();
    }
    GlobalAtomicValue=0;
    IncrementAtomicValue();
    /*Increasing Value with Int*/
    VectorOfThreads.clear();
    for( int i=0; i<NumberOfThreads; i++){
        VectorOfThreads.emplace_back(IncrementIntValue);
    }
    for( auto& Thread : VectorOfThreads){
        Thread.join();
    }
    GlobalIntValue=0;
    IncrementIntValue();
    /*Unsynchronized increase*/
    GlobalIntValue = 0;
    VectorOfThreads.clear();
    for( int i=0; i<NumberOfThreads; i++){
        VectorOfThreads.emplace_back(IncrementUnsynchronizedValue);
    }
    for( auto& Thread : VectorOfThreads){
        Thread.join();
    }
    GlobalIntValue=0;
    IncrementUnsynchronizedValue();
    system("Pause");
    return 0;
}
