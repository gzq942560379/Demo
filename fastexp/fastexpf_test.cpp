#include "test.h"

int main(){
    fastexp_accuracy_test<float>(-9, 11);
    tanh_exp_accuracy_test<float>(-5, 5);
    return 0;
}

