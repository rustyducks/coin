#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

namespace rd {
class Timer {
   public:
    Timer(double duration) : isStarted_(false), duration_(duration) {}

    void start() {
        if (isStarted_ && clock::now() < toStopPoint_) {
            std::cout << "[Timer Warning] Restarted an unfinished timer." << std::endl;
        }
        isStarted_ = true;
        toStopPoint_ = clock::now() + std::chrono::milliseconds((int)(duration_ * 1000.));
    }
    bool check() {
        if (isStarted_ && clock::now() >= toStopPoint_) {
            return true;
        }
        return false;
    }

    bool isStarted() { return isStarted_; }

   protected:
    bool isStarted_;
    double duration_;
    using clock = std::chrono::system_clock;
    clock::time_point toStopPoint_;
};
}  // namespace rd

#endif /* TIMER_H */
