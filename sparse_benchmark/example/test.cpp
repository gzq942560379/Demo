#include <sparsebench.hpp>


int main(){

    string filename("/vol0001/hp230257/guozhuoqiang/DeepFlame/deepflame-dev/examples/dfLowMachFoam/threeD_reactingTGV/CH4/Grid128_2pi_8_2x2x2/sparse_pattern_0.mtx");
    COO<int64_t, double> coo = read_coo_from_pattern_mtx(filename);
    CSR<int64_t, double> csr(coo);
    csr.spmv_benchmark();
    return 0;
}
