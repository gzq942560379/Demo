#pragma once
#include "fastexp.h"
#include "fasttanh.h"

template<typename T>
void fastexp_accuracy_test(int range_start = -80, int range_end = 80, int split = 100);

template<typename T>
void tanh_exp_accuracy_test(int range_start = -80, int range_end = 80, int split = 100);

template<typename T>
void fastexp_accuracy_test(int range_start, int range_end, int split){
    const char* filename1 = "fastexp_1.dat";
    const char* filename2 = "fastexp_2.dat";
    const char* filename3 = "fastexp_3.dat";
    const char* filename4 = "fastexp_4.dat";
    const char* filename5 = "fastexp_5.dat";
    const char* filename6 = "fastexp_6.dat";
    const char* filename7 = "fastexp_7.dat";
    const char* filename8 = "fastexp_8.dat";

    FILE* fout1 = fopen(filename1, "w");
    FILE* fout2 = fopen(filename2, "w");
    FILE* fout3 = fopen(filename3, "w");
    FILE* fout4 = fopen(filename4, "w");
    FILE* fout5 = fopen(filename5, "w");
    FILE* fout6 = fopen(filename6, "w");
    FILE* fout7 = fopen(filename7, "w");
    FILE* fout8 = fopen(filename8, "w");
    
    for(int i = range_start; i < range_end; ++i){
        for(int j = 0; j < split; ++j){
            T x = i + 1. * j / split;
            T y = std::exp(x);

            T y1 = fastexp_1(x);
            T y2 = fastexp_2(x);
            T y3 = fastexp_3(x);
            T y4 = fastexp_4(x);
            T y5 = fastexp_5(x);
            T y6 = fastexp_6(x);
            T y7 = fastexp_7(x);
            T y8 = fastexp_8(x);

            double absolute_error1 = std::abs(y - y1);
            double absolute_error2 = std::abs(y - y2);
            double absolute_error3 = std::abs(y - y3);
            double absolute_error4 = std::abs(y - y4);
            double absolute_error5 = std::abs(y - y5);
            double absolute_error6 = std::abs(y - y6);
            double absolute_error7 = std::abs(y - y7);
            double absolute_error8 = std::abs(y - y8);

            double relative_error1 = absolute_error1 / y;
            double relative_error2 = absolute_error2 / y;
            double relative_error3 = absolute_error3 / y;
            double relative_error4 = absolute_error4 / y;
            double relative_error5 = absolute_error5 / y;
            double relative_error6 = absolute_error6 / y;
            double relative_error7 = absolute_error7 / y;
            double relative_error8 = absolute_error8 / y;

            fprintf(fout1, "%lf %.16e %.16e %.16e %.16e\n", x, y, y1, absolute_error1, relative_error1);
            fprintf(fout2, "%lf %.16e %.16e %.16e %.16e\n", x, y, y2, absolute_error2, relative_error2);
            fprintf(fout3, "%lf %.16e %.16e %.16e %.16e\n", x, y, y3, absolute_error3, relative_error3);
            fprintf(fout4, "%lf %.16e %.16e %.16e %.16e\n", x, y, y4, absolute_error4, relative_error4);
            fprintf(fout5, "%lf %.16e %.16e %.16e %.16e\n", x, y, y5, absolute_error5, relative_error5);
            fprintf(fout6, "%lf %.16e %.16e %.16e %.16e\n", x, y, y6, absolute_error6, relative_error6);
            fprintf(fout7, "%lf %.16e %.16e %.16e %.16e\n", x, y, y7, absolute_error7, relative_error7);
            fprintf(fout8, "%lf %.16e %.16e %.16e %.16e\n", x, y, y8, absolute_error8, relative_error8);
        }
    }
    fclose(fout1);
    fclose(fout2);
    fclose(fout3);
    fclose(fout4);
    fclose(fout5);
    fclose(fout6);
    fclose(fout7);
    fclose(fout8);
}

template<typename T>
void tanh_exp_accuracy_test(int range_start, int range_end, int split){
    const char* filename1 = "tanh_exp_1.dat";
    const char* filename2 = "tanh_exp_2.dat";
    const char* filename3 = "tanh_exp_3.dat";
    const char* filename4 = "tanh_exp_4.dat";
    const char* filename5 = "tanh_exp_5.dat";
    const char* filename6 = "tanh_exp_6.dat";
    const char* filename7 = "tanh_exp_7.dat";
    const char* filename8 = "tanh_exp_8.dat";

    FILE* fout1 = fopen(filename1, "w");
    FILE* fout2 = fopen(filename2, "w");
    FILE* fout3 = fopen(filename3, "w");
    FILE* fout4 = fopen(filename4, "w");
    FILE* fout5 = fopen(filename5, "w");
    FILE* fout6 = fopen(filename6, "w");
    FILE* fout7 = fopen(filename7, "w");
    FILE* fout8 = fopen(filename8, "w");

    int element_cout = (range_end - range_start) * split;

    T* X = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y1 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y2 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y3 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y4 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y5 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y6 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y7 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 
    T* Y8 = (T*)aligned_alloc(64, element_cout * sizeof(T)); 

    // fill x

    for(int i = range_start; i < range_end; ++i){
        for(int j = 0; j < split; ++j){
            int index = (i - range_start) * split + j;
            T value = i + 1. * j / split;
            X[index] = value;
        }
    }

    for(int i = 0 ; i < element_cout; ++i){
        T x = X[i];
        Y[i] = std::tanh(x);
        Y1[i] = tanh_fastexp_1(x);
        Y2[i] = tanh_fastexp_2(x);
        Y3[i] = tanh_fastexp_3(x);
        Y4[i] = tanh_fastexp_4(x);
        Y5[i] = tanh_fastexp_5(x);
        Y6[i] = tanh_fastexp_6(x);
        Y7[i] = tanh_fastexp_7(x);
        Y8[i] = tanh_fastexp_8(x);
    }

    // tanh_fastexp_simd_1(Y1, X, element_cout);

    for(int i = 0 ; i < element_cout; ++i){
        T x = X[i];

        T y = Y[i];
        T y1 = Y1[i];
        T y2 = Y2[i];
        T y3 = Y3[i];
        T y4 = Y4[i];
        T y5 = Y5[i];
        T y6 = Y6[i];
        T y7 = Y7[i];
        T y8 = Y8[i];

        double absolute_error1 = std::abs(y - y1);
        double absolute_error2 = std::abs(y - y2);
        double absolute_error3 = std::abs(y - y3);
        double absolute_error4 = std::abs(y - y4);
        double absolute_error5 = std::abs(y - y5);
        double absolute_error6 = std::abs(y - y6);
        double absolute_error7 = std::abs(y - y7);
        double absolute_error8 = std::abs(y - y8);

        double relative_error1 = absolute_error1 / y;
        double relative_error2 = absolute_error2 / y;
        double relative_error3 = absolute_error3 / y;
        double relative_error4 = absolute_error4 / y;
        double relative_error5 = absolute_error5 / y;
        double relative_error6 = absolute_error6 / y;
        double relative_error7 = absolute_error7 / y;
        double relative_error8 = absolute_error8 / y;

        fprintf(fout1, "%lf %.16e %.16e %.16e %.16e\n", x, y, y1, absolute_error1, relative_error1);
        fprintf(fout2, "%lf %.16e %.16e %.16e %.16e\n", x, y, y2, absolute_error2, relative_error2);
        fprintf(fout3, "%lf %.16e %.16e %.16e %.16e\n", x, y, y3, absolute_error3, relative_error3);
        fprintf(fout4, "%lf %.16e %.16e %.16e %.16e\n", x, y, y4, absolute_error4, relative_error4);
        fprintf(fout5, "%lf %.16e %.16e %.16e %.16e\n", x, y, y5, absolute_error5, relative_error5);
        fprintf(fout6, "%lf %.16e %.16e %.16e %.16e\n", x, y, y6, absolute_error6, relative_error6);
        fprintf(fout7, "%lf %.16e %.16e %.16e %.16e\n", x, y, y7, absolute_error7, relative_error7);
        fprintf(fout8, "%lf %.16e %.16e %.16e %.16e\n", x, y, y8, absolute_error8, relative_error8);
    }

    free(X);
    free(Y);
    free(Y1);
    free(Y2);
    free(Y3);
    free(Y4);
    free(Y5);
    free(Y6);
    free(Y7);
    free(Y8);

    fclose(fout1);
    fclose(fout2);
    fclose(fout3);
    fclose(fout4);
    fclose(fout5);
    fclose(fout6);
    fclose(fout7);
    fclose(fout8);
}