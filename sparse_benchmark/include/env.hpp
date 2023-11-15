#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstdint>

static bool env_get_bool(const char* name, bool default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    if(std::strcmp(tmp, "true") == 0){
        return true;
    }else{
        return false;
    }
}

static int env_get_int(const char* name, int default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return std::atoi(tmp);
}

static int64_t env_get_int64(const char* name, int64_t default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return std::atol(tmp);
}
