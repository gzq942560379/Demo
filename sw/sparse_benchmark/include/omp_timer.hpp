#pragma once

#ifndef _OPENMP
#error "must open -fopenmp flag!!!"
#endif

#define dtime() omp_get_wtime()

class omp_timer{

    typedef double timeType;

    timeType startTime_;
    mutable timeType lastTime_;
    mutable timeType newTime_;

    //- Retrieve the current time values from the system
    static void getTime(timeType&);

    //- Difference between two times
    static double timeDifference(const timeType& beg, const timeType& end);

public:
    omp_timer();

    double elapsedTime() const;
    
    double timeIncrement() const;
};
