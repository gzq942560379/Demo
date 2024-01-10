#pragma once
#include <unistd.h>         // for getpid and usleep
#include <sys/sysinfo.h>    // for get_nprocs
#include <cstdio>    
#include <cstdlib>    
#include <cassert>    
#include <cstring>
#include <mpi.h>

// 内存信息的结构体，读文件/proc/meminfo
struct MemInfo {
    unsigned long total;        // MemTotal 总内存
    unsigned long free;         // MemFree  空闲内存
    unsigned long avaliable;    // MemAvailable 可用内存，约等于 MemFree + Buffer + Catch
};

// 解析/proc/meminfo文件，获取内存信息
// MemTotal %lu
// MemFree %lu
// MemAvailable %lu (since Linux 3.14)
static void GetSystemMeminfo(MemInfo *info) {
    memset(info, 0, sizeof(MemInfo));
    char buf[128];
    char name[64];
    char unit[64];
    FILE *fd = fopen("/proc/meminfo", "r");
    assert(fd != nullptr);
    fgets(buf, sizeof(buf), fd);
    sscanf(buf, "%s %lu %s", name, &info->total, unit);
    fgets(buf, sizeof(buf), fd);
    sscanf(buf, "%s %lu %s", name, &info->free, unit);
    fgets(buf, sizeof(buf), fd);
    sscanf(buf, "%s %lu %s", name, &info->avaliable, unit);
    fclose(fd);
    // fprintf(stdout, "mem: %u %u %u\n", info->total, info->free, info->avaliable);
}

// 获取系统已使用的内存，单位GB。total为输出参数，表示总内存

double GetSystemMemTotal() {
    MemInfo info;
    GetSystemMeminfo(&info);
    double system_total = info.total / 1024. / 1024.;  // GB
    return system_total;
}

double GetSystemMemUsed() {
    MPI_Barrier(MPI_COMM_WORLD);
    MemInfo info;
    GetSystemMeminfo(&info);
    double system_used = (info.total - info.avaliable) / 1024. / 1024.;  // GB
    MPI_Barrier(MPI_COMM_WORLD);
    return system_used;
}


