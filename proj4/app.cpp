#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"

void on_completion(rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox>& _,
                    rlbox::tainted<char*,rlbox::rlbox_wasm2c_sandbox> tainted_str);


//Callback on completion of library function
void on_completion(char* result) {
    char result_str[100];
    strcpy(result_str, result);
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
    sandbox.create_sandbox();
    
    auto ok = sandbox.invoke_sandbox_function(gethash, 3, 4).copy_and_verify([](unsigned ret){
         printf("Adding... 3+4 = %d\n", ret);
         return ret == 7;
    });
  
    char* copy_str = (char*)argv[1];
    char* result_str = (char*)argv[2];

    print_version();    
    long long hash = get_hash(copy_str, on_completion, result_str);
    printf("Hash = %llx\n", hash);
    
    sandbox.destroy_sandbox();
    return 0;
}
#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"

void on_completion(rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox>& _,
                    rlbox::tainted<char*,rlbox::rlbox_wasm2c_sandbox> tainted_str);


//Callback on completion of library function
void on_completion(char* result) {
    char result_str[100];
    strcpy(result_str, result);
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
    sandbox.create_sandbox();
  
    char* copy_str = (char*)argv[1];
    char* result_str = (char*)argv[2];

    print_version();    
    long long hash = get_hash(copy_str, on_completion, result_str);
    printf("Hash = %llx\n", hash);
    
    sandbox.destroy_sandbox();
    return 0;
}

