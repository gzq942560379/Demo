#include <iostream>
#include <mpi.h>
#include <crts.h>
#include <math.h>

#include "common_kernel.H"

#define LEN 524288

void fill_random(double* input, int64_t len, double start, double end){
    double range_len = end - start;
    for(int64_t i = 0; i < len; ++i){
        input[i] = (rand() * range_len / RAND_MAX) + start;
    }
}

void fill_n(double* input, int64_t len, double value){
    for(int64_t i = 0; i < len; ++i){
        input[i] = value;
    }
}

void array_copy(double* dst, const double* src, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void check_error(double* answer, const double* check, int64_t len){
    double a = 0.;
    double b = 0.;
    double max_relative_error = 0.;
    double max_absulte_error = 0.;
    for(int64_t i = 0; i < len; ++i){
        double absulte_error = fabs(answer[i] - check[i]);
        double relative_error = absulte_error / answer[i];
        if(relative_error > max_relative_error){
            max_relative_error = relative_error;
            max_absulte_error = absulte_error;
            a = answer[i];
            b = check[i];
        }
    }
    printf("-------------------------------------------------\n");
    printf("a : %lf\n", a);
    printf("b : %lf\n", b);
    printf("max_absulte_error : %e\n", max_absulte_error);
    printf("max_relative_error : %e\n", max_relative_error);
    printf("-------------------------------------------------\n");
}

void axpy(scalar* y, scalar alpha, const scalar* x, scalar beta, label len){
    for(label i = 0; i < len; ++i){
        y[i] += alpha * x[i];
    }
}

void axpy_test(){
    int64_t random_len = 2048;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 4);
    
    int64_t len = LEN;
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double* y = (double*)libc_aligned_malloc(len * sizeof(double));
    double* y_answer = (double*)libc_aligned_malloc(len * sizeof(double));

    double alpha = 2.;
    double beta = 1;

    for(int64_t i = 0; i < len; i += random_len){
        array_copy(x + i, random_list, random_len);
        array_copy(y + i, random_list, random_len);
        array_copy(y_answer + i, random_list, random_len);
    }

    axpy(y_answer, alpha, x, beta, len);

    df_axpy(y, alpha, x, beta, len);

    check_error(y_answer, y, len);

    libc_aligned_free(random_list);
    libc_aligned_free(x);
    libc_aligned_free(y);
    libc_aligned_free(y_answer);
}

scalar sum(const scalar* input, label len){
    double ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += input[i];
    }
    return ret;
}

void sum_test(){
    int64_t random_len = 128;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 5);
    
    int64_t len = 524288;
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double y_answer;
    double y;
    
    for(int64_t i = 0; i < len; i += random_len){
        array_copy(x + i, random_list, random_len);
    }

    y_answer = sum(x, len);

    y = df_sum(x, len);

    check_error(&y_answer, &y, 1);

    libc_aligned_free(random_list);
    libc_aligned_free(x);
}

scalar sum_mag(const scalar* input, label len){
    double ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += std::abs(input[i]);
    }
    return ret;
}

void sum_mag_test(){
    int64_t random_len = 128;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 5);
    
    int64_t len = 524288;
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double y_answer;
    double y;
    
    for(int64_t i = 0; i < len; i += random_len){
        array_copy(x + i, random_list, random_len);
    }

    y_answer = sum_mag(x, len);

    y = df_sum_mag(x, len);

    check_error(&y_answer, &y, 1);

    libc_aligned_free(random_list);
    libc_aligned_free(x);
}

scalar sum_sqr(const scalar* input, label len){
    double ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += input[i] * input[i];
    }
    return ret;
}

void sum_sqr_test(){
    int64_t random_len = 128;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 5);
    
    int64_t len = 524288;
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double y_answer;
    double y;
    
    for(int64_t i = 0; i < len; i += random_len){
        array_copy(x + i, random_list, random_len);
    }

    y_answer = sum_sqr(x, len);

    y = df_sum_sqr(x, len);

    check_error(&y_answer, &y, 1);

    libc_aligned_free(random_list);
    libc_aligned_free(x);
}

scalar sum_prod(const scalar* a, const scalar* b, label len){
    double ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += a[i] * b[i];
    }
    return ret;
}

void sum_prod_test(){
    int64_t random_len = 128;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 5);
    
    int64_t len = 524288;
    double* a  = (double*)libc_aligned_malloc(len * sizeof(double));
    double* b  = (double*)libc_aligned_malloc(len * sizeof(double));
    double y_answer;
    double y;
    
    for(int64_t i = 0; i < len; i += random_len){
        array_copy(a + i, random_list, random_len);
        array_copy(b + i, random_list, random_len);
    }

    y_answer = sum_prod(a, b, len);

    y = df_sum_prod(a, b, len);

    check_error(&y_answer, &y, 1);

    libc_aligned_free(random_list);
    libc_aligned_free(a);
    libc_aligned_free(b);
}

void copy(scalar* dst, const scalar* src, label len){
    for(label i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void copy_test(){
    int64_t random_len = 128;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 5);
    
    int64_t len = 128;
    double* src = (double*)libc_aligned_malloc(len * sizeof(double));
    double* dst = (double*)libc_aligned_malloc(len * sizeof(double));
    double* dst_answer = (double*)libc_aligned_malloc(len * sizeof(double));
    
    for(int64_t i = 0; i < len; i += random_len){
        array_copy(src + i, random_list, random_len);
    }

    fill_n(dst, len, 0.);
    fill_n(dst_answer, len, 0.);

    copy(dst_answer, src, len);

    df_copy(dst, src, len);

    for(int64_t i = 0; i < len; ++i){
        printf("%lf, %lf\n", dst_answer[i], dst[i]);
    }

    check_error(dst_answer, dst, len);


    libc_aligned_free(random_list);
    libc_aligned_free(src);
    libc_aligned_free(dst);
    libc_aligned_free(dst_answer);
}

scalar norm_factor_local(const scalar* pAPtr, scalar gPsiAvg, const scalar* yAPtr, const scalar* sourcePtr, label nCells){
    scalar ret;
    ret = 0.;
    for(label c = 0; c < nCells; ++c){
        double tmp = pAPtr[c] * gPsiAvg;
        ret += std::abs(yAPtr[c] - tmp) + std::abs(sourcePtr[c] - tmp);
    }
    return ret;
}

void norm_factor_local_test(){
    int64_t len = LEN;
    double* pAPtr  = (double*)libc_aligned_malloc(len * sizeof(double));
    double* yAPtr = (double*)libc_aligned_malloc(len * sizeof(double));
    double* sourcePtr = (double*)libc_aligned_malloc(len * sizeof(double));

    double ret;
    double ret_answer;
    
    double gPsiAvg = 2.;
    fill_n(pAPtr, len, 0.1);
    fill_n(yAPtr, len, 0.25);
    fill_n(sourcePtr, len, 0.22);

    ret_answer = norm_factor_local(pAPtr, gPsiAvg, yAPtr, sourcePtr, len);

    ret = df_norm_factor_local(pAPtr, gPsiAvg, yAPtr, sourcePtr, len);

    check_error(&ret_answer, &ret, 1);

    libc_aligned_free(pAPtr);
    libc_aligned_free(yAPtr);
    libc_aligned_free(sourcePtr);
}

void triad(scalar* z, scalar alpha, const scalar* x, scalar beta, const scalar* y, scalar gamma, label len){
    for(label i = 0; i < len; ++i){
        z[i] = alpha * x[i] + beta * y[i] + gamma * z[i];
    }
}

void triad_test(){
    int64_t len = LEN;
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double* y = (double*)libc_aligned_malloc(len * sizeof(double));
    double* z = (double*)libc_aligned_malloc(len * sizeof(double));
    double* z_answer = (double*)libc_aligned_malloc(len * sizeof(double));

    fill_n(x, len, 2.);
    fill_n(y, len, 1.);
    fill_n(z, len, 1.);
    fill_n(z_answer, len, 1.);

    double alpha = 0.5;
    double beta = 0.6;
    double gamma = 1.;

    triad(z_answer, alpha, x, beta, y, gamma, len);

    df_triad(z, alpha, x, beta, y, gamma, len);

    check_error(z_answer, z, len);

    libc_aligned_free(x);
    libc_aligned_free(y);
    libc_aligned_free(z);
    libc_aligned_free(z_answer);
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    // axpy_test();
    triad_test();

    // norm_factor_local_test();
    // sum_test();
    // sum_mag_test();
    // sum_sqr_test();
    // sum_prod_test();
    // copy_test();

    MPI_Finalize();
    return 0;
}