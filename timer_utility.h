#include<chrono>
#include<iostream>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

class TimerUtility {

    public:
        
        void startTimer() {
            start = high_resolution_clock::now();
        }
        void getDuration(std::string comment = "") {
            setEndTime();
            setDuration();
            if(duration_ms.count() >= 1000) {
                std::cout << duration_ms.count() / 1000 << "s: " << comment << std::endl;
            } else {
                std::cout << duration_ms.count() << "ms: " << comment << std::endl;
            }  
        }

    private:
    
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
        duration<double, std::milli> duration_ms;

        void setEndTime() {
            end = high_resolution_clock::now();
        }
        void setDuration() {
            duration_ms = end - start;
        }
};