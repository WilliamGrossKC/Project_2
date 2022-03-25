#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Callback to indicate the task is completed
typedef void(OnCompletion)(char*);

void print_version();

long long get_hash(char* in, OnCompletion* on_completion, char* result_msg);

long long get_hash_alt(char* in, OnCompletion* on_completion, char* result_msg);


#ifdef __cplusplus
}
#endif
