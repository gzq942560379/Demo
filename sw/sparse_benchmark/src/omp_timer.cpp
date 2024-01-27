#include <omp_timer.hpp>
#include <omp.h>
#include <iostream>

void omp_timer::getTime(timeType& t){
    t = omp_get_wtime();
}

double omp_timer::timeDifference(const timeType& beg, const timeType& end){
    double interval = end - beg;
    if(interval < omp_get_wtick() * 100){
        std::cout << "[Warning] Timing accuracy is insufficient." << std::endl;
        std::cout << "[Warning] The timing accuracy is : " << omp_get_wtick() << std::endl;
        std::cout << "[Warning] The measurement time is : " << interval << std::endl;
    }
    return interval;
}

omp_timer::omp_timer(){
    getTime(startTime_);
    lastTime_ = startTime_;
    newTime_ = startTime_;
}

double omp_timer::elapsedTime() const {
    getTime(newTime_);
    return timeDifference(startTime_, newTime_);
}

double omp_timer::timeIncrement() const {
    lastTime_ = newTime_;
    getTime(newTime_);
    return timeDifference(lastTime_, newTime_);
}
