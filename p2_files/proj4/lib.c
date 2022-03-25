#include "stdio.h"
#include "string.h"
#include "lib.h"

void print_version(){
    printf("VERSION: 1.0.0\n");
}

long long compute_hash_alt(char *in)
{
    unsigned long hash = 5381;
    int c;

    while (c = *in++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


long long compute_hash(char* in) {
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (unsigned idx = 0; idx < strlen(in); idx++){
    //for (char c : in) {
        char c = in[idx];
        hash_value = (hash_value + (c - ' ' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

// void invoke_on_completion(OnCompletion* on_completion, char* result_msg){
//     on_completion(result_msg);
// }

// long long get_hash(char* in)

long long get_hash_alt(char* in, OnCompletion* on_completion, char* result_msg){
    char tmp[100];
    strcpy(tmp, in);
    long long hash = compute_hash_alt(tmp);
    on_completion(result_msg);
    return hash;
}

long long get_hash(char* in, OnCompletion* on_completion, char* result_msg){
    char tmp[100];
    strcpy(tmp, in);
    long long hash = compute_hash(tmp);
    on_completion(result_msg);
    return hash;
}
