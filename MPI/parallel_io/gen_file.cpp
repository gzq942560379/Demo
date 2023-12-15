#include "common.h"

int main(){

    std::vector<element_type> data_write(element_count);

    for(uint64_t i = 0;i < element_count; ++i){
        data_write[i] = i;
    }

    std::ofstream fout(write_filepath, std::ios::binary);
    fout.write(reinterpret_cast<char*>(data_write.data()), element_count * sizeof(element_type));
    fout.close();

    // check
    std::vector<element_type> data_read(element_count);
    std::ifstream fin(write_filepath, std::ios::binary);
    fin.read(reinterpret_cast<char*>(data_read.data()), element_count * sizeof(element_type));
    fin.close();

    for(uint64_t i = 0;i < element_count; ++i){
        assert(data_write[i] == data_read[i]);
    }

    std::cout << "write success !!!" << std::endl;

    return 0;
}

