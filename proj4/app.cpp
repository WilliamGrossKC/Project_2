#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"

// using namespace rlbox;
//Callback on completion of library function

/*
void get_hash_cb(rlbox_sandbox<rlbox_noop_sandbox>& _,
              tainted<const char*, rlbox_noop_sandbox> str) {
  auto checked_string =
    str.copy_and_verify_string([](std::unique_ptr<char[]> val) {
        return std::strlen(val.get()) < 1024 ? std::move(val) : nullptr;
    });
  printf("hello_cb: %s\n", checked_string.get());
}
*/

void on_completion(rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox>& _,
                    rlbox::tainted<char*,rlbox::rlbox_wasm2c_sandbox> tainted_str) {
  
    char result_str[100];
    strcpy(result_str, tainted_str.UNSAFE_unverified());
    printf("Done: %s\n", result_str);
  
}


int main(int argc, char const *argv[])
{
    //check for input from command line
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " COPY_STRING RESULT_STRING" << std::endl;
        return 1;
    }
    
    rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox> sandbox;
    sandbox.create_sandbox("./my_lib.so");
    
    char* copy_str = (char*)argv[1];
    size_t copySize = strlen(copy_str) + 1;
    auto taintedStr1 = sandbox.malloc_in_sandbox<char>(copySize);
    std::strcopy(taintedStr1.UNSAFE_unverified(), copy_str, copySize);
            
    char* result_str = (char*)argv[2];
    size_t resultSize = strlen(result_str) + 1;
    auto taintedStr2 = sandbox.malloc_in_sandbox<char>(resultSize);
    std::strcopy(taintedStr2.UNSAFE_unverified(), result_str, resultSize);
  
    sandbox.invoke_sandbox_function(print_version);
    auto cb = sandbox.register_callback(on_completion);
    auto hash = sandbox.invoke_sandbox_function(get_hash,taintedStr1, on_completion, taintedStr2);
    long long hash2 = hash.UNSAFE_unverified();
    printf("Hash = %llx\n", hash2);

    sandbox.destroy_sandbox();
    return 0;
}
