#include "test.h"

int main(){
    fastexp_accuracy_test<double>(-9, 11);
    tanh_exp_accuracy_test<double>(-5, 5);
    return 0;
}

