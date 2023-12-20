#include<chrono>
#include<iostream>
#include<string>

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
            sendMessage(&comment);
        }

    private:
    
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
        duration<double, std::milli> duration_ms;
        std::string message = "";
        void setEndTime() {
            end = high_resolution_clock::now();
        }
        void setDuration() {
            duration_ms = end - start;
        }
        void sendMessage(std::string *comment) {
            if(duration_ms.count() >= 1000) {
                message += std::to_string(duration_ms.count() / 1000);
                message = message.substr(0, 6);
                message += +"s";
            } else {
                message += std::to_string(duration_ms.count());
                message = message.substr(0, 6);
                message += "ms";
            }
            if((*comment).length()) {
                message.append(10 - message.length(), ' ');
                message += ": " + (*comment);
            }
            message += '\n';
            std::cout << message;
            message.clear();
        }
};