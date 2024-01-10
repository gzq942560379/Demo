#include "test.h"

int main(){
    fastexp_accuracy_test<__fp16>(-9, 11);
    tanh_exp_accuracy_test<__fp16>(-5, 5);
    return 0;
}

