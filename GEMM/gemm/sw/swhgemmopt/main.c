#include<stdio.h>
#include<strings.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>
#include "athread.h"

#if defined DOUBLE
#define TYPE double
#elif defined SINGLE
#define TYPE float
#elif defined HALF
#define TYPE uint16_t 
#endif

#define TOTAL  (M*LDA+K*LDB+M*LDC) // 74752 // 299008
#define ANUMBER (M*LDA)  // 30720  //122880
#define BNUMBER (K*LDB) //71680   //286720
#define CNUMBER (M*LDC)

#define    HPL_PTR( ptr_, al_ ) \
	( ( ( (size_t)(ptr_)+(al_)-1 ) / (al_) ) * (al_) )
#define align   (32)

#define m_rtc(result) \
{	\
	asm volatile ( "rtc\t%0" : "=r"( result ) : :"memory" );	\
}while(0);


int cgid;

extern void sw_hgemm(int M, int N, int K, TYPE * A, int LDA, TYPE * B, int LDB, TYPE * C, int LDC);

int main(int argc, char ** argv)
{
	MPI_Init(&argc, &argv);
    int M, N, K, LDA, LDB, LDC;
	TYPE *aa, *right, *aa_dst, *bb, *bb_dst, *cc, *cc_dst; 
	unsigned long r1, r2, r3, r4;

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    K = atoi(argv[3]);

    LDA = K;
    LDB = LDC = N;

	athread_init();

	aa = (TYPE *)libc_aligned_malloc(ANUMBER*sizeof(TYPE)+128*sizeof(TYPE));
	bb = (TYPE *)libc_aligned_malloc(BNUMBER*sizeof(TYPE)+128*sizeof(TYPE));
	cc = (TYPE *)libc_aligned_malloc(CNUMBER*sizeof(TYPE)+128*sizeof(TYPE));
	right = (TYPE *)libc_aligned_malloc(CNUMBER*sizeof(TYPE)+128*sizeof(TYPE));

	cgid = current_array_id();

	// aa_dst = HPL_PTR(aa,align*sizeof(TYPE));
	// bb_dst = HPL_PTR(bb,align*sizeof(TYPE));
	// cc_dst = HPL_PTR(cc,align*sizeof(TYPE));

	aa_dst = aa;
	bb_dst = bb;
	cc_dst = cc;

	int index = 0;

    for(index = 0; index < 4; index ++) {
        printf("opt hgemm...\n");

		double timer_start = MPI_Wtime();
		m_rtc(r1);
        sw_hgemm(M, N, K, aa_dst, LDA, bb_dst, LDB, cc_dst, LDC);
		m_rtc(r2);
		double timer_end = MPI_Wtime();

		double time = timer_end - timer_start;

		r3 = r2-r1;
		printf("rowmajor. M %d N %d K %d, time: %ld cycles, Eff %lf.\n", M, N, K, r3, (double)M * N * K * 2 / (4096.0) / ((double)r3 / 2.1 * 2.25));

		double flop = 2. * M * N * K;
		double TFlops = flop/time/1e12;
		double peak = TFlops / 9.216 * 100;
		printf("TFlops : %lf\n", TFlops);
		printf("Peak : %lf%%\n", peak);
	}

	athread_halt();

	MPI_Finalize();
    return 0;
}
