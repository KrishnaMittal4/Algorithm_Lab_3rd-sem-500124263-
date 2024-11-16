#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <time.h>  

// Naive String Matching Algorithm  
void naive_search(const char *text, const char *pattern, int *match_count) {  
    int text_len = strlen(text);  
    int pattern_len = strlen(pattern);  
    *match_count = 0;  

    for (int i = 0; i <= text_len - pattern_len; i++) {  
        int j;  
        for (j = 0; j < pattern_len; j++) {  
            if (text[i + j] != pattern[j]) {  
                break;  
            }  
        }  
        if (j == pattern_len) {  
            (*match_count)++;  
        }  
    }  
}  

// Rabin-Karp Algorithm  
void rabin_karp_search(const char *text, const char *pattern, int *match_count) {  
    int text_len = strlen(text);  
    int pattern_len = strlen(pattern);  
    int p = 31; // A prime number  
    int m = 1e9 + 9; // A large prime number  
    long long pattern_hash = 0, text_hash = 0, p_pow = 1;  
    *match_count = 0;  

    for (int i = 0; i < pattern_len; i++) {  
        pattern_hash = (pattern_hash + (pattern[i] - 'a' + 1) * p_pow) % m;  
        text_hash = (text_hash + (text[i] - 'a' + 1) * p_pow) % m;  
        if (i < pattern_len - 1) {  
            p_pow = (p_pow * p) % m;  
        }  
    }  

    for (int i = 0; i <= text_len - pattern_len; i++) {  
        if (pattern_hash == text_hash) {  
            (*match_count)++;  
        }  
        if (i < text_len - pattern_len) {  
            text_hash = (text_hash - (text[i] - 'a' + 1)) % m;  
            text_hash = (text_hash + m) % m; // Ensure non-negative  
            text_hash = (text_hash * p) % m;  
            text_hash = (text_hash + (text[i + pattern_len] - 'a' + 1)) % m;  
        }  
    }  
}  

// KMP Algorithm  
void compute_lps(const char *pattern, int *lps) {  
    int length = 0;  
    lps[0] = 0;  
    int i = 1;  
    int pattern_len = strlen(pattern);  

    while (i < pattern_len) {  
        if (pattern[i] == pattern[length]) {  
            length++;  
            lps[i] = length;  
            i++;  
        } else {  
            if (length != 0) {  
                length = lps[length - 1];  
            } else {  
                lps[i] = 0;  
                i++;  
            }  
        }  
    }  
}  

void kmp_search(const char *text, const char *pattern, int *match_count) {  
    int text_len = strlen(text);  
    int pattern_len = strlen(pattern);  
    int *lps = (int *)malloc(pattern_len * sizeof(int));  
    compute_lps(pattern, lps);  
    *match_count = 0;  

    int i = 0; // index for text  
    int j = 0; // index for pattern  

    while (i < text_len) {  
        if (pattern[j] == text[i]) {  
            i++;  
            j++;  
        }  
        if (j == pattern_len) {  
            (*match_count)++;  
            j = lps[j - 1];  
        } else if (i < text_len && pattern[j] != text[i]) {  
            if (j != 0) {  
                j = lps[j - 1];  
            } else {  
                i++;  
            }  
        }  
    }  
    free(lps);  
}  

// Function to measure execution time  
double measure_time(void (*func)(const char*, const char*, int*), const char *text, const char *pattern, int *match_count, int repeats) {  
    struct timespec start, end;  
    clock_gettime(CLOCK_MONOTONIC, &start);  
    for (int i = 0; i < repeats; i++) {  
        func(text, pattern, match_count);  
    }  
    clock_gettime(CLOCK_MONOTONIC, &end);  
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; // Return time in seconds  
}  

int main() {  
    // Define five sets of text and patterns  
    const char *texts[5] = {  
        "abracadabraabracadabraabracadabraabracadabra",  
        "thequickbrownfoxjumpsoverthelazydog",  
        "cprogrammingisfunandinteresting",  
        "hellohelloworldhellohello",  
        "repeatrepeatrepeatrepeat"  
    };  

    const char *patterns[5] = {  
        "abra",  
        "fox",  
        "isfun",  
        "hello",  
        "repeat"  
    };  

    int match_count = 0;  
    int repeats = 10000; // Number of repetitions for timing  

    // Loop through all inputs  
    for (int i = 0; i < 5; i++) {  
        printf("Test %d:\n", i + 1);  

        // Naive String Matching  
        match_count = 0;  
        double naive_time = measure_time(naive_search, texts[i], patterns[i], &match_count, repeats);  
        printf("Naive String Matching:\nTime taken: %f seconds\nMatches found: %d\n\n", naive_time, match_count);  

        // Rabin-Karp Algorithm  
        match_count = 0;  
        double rabin_karp_time = measure_time(rabin_karp_search, texts[i], patterns[i], &match_count, repeats);  
        printf("Rabin-Karp Algorithm:\nTime taken: %f seconds\nMatches found: %d\n\n", rabin_karp_time, match_count);  

        // KMP Algorithm  
        match_count = 0;  
        double kmp_time = measure_time(kmp_search, texts[i], patterns[i], &match_count, repeats);  
        printf("Knuth-Morris-Pratt Algorithm:\nTime taken: %f seconds\nMatches found: %d\n\n", kmp_time, match_count);  
    }  

    return 0;  
}