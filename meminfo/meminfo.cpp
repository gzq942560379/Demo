#include "meminfo.h"
#include <iostream>

int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    double system_total =  GetSystemMemTotal();
    std::cout << "system_total : " << system_total << std::endl;


    double system_used = GetSystemMemUsed();

    std::cout << "---------------------------------" << std::endl;
    std::cout << "system_used : " << system_used << std::endl;


    size_t size = 1024 * 1024 * 1024;
    size_t element_count = size / sizeof(double);

    double* data = new double[element_count];

    for(size_t i = 0; i < element_count; ++i){
        data[i] = i;
    }

    double result = 0;
    for(size_t i = 0; i < element_count; ++i){
        result += data[i];
    }

    std::cout << "result : " << result << std::endl;

    system_used = GetSystemMemUsed();
    std::cout << "---------------------------------" << std::endl;
    std::cout << "system_used : " << system_used << std::endl;

    delete [] data;

    system_used = GetSystemMemUsed();
    std::cout << "---------------------------------" << std::endl;
    std::cout << "system_used : " << system_used << std::endl;

    MPI_Finalize();
    return 0;
}