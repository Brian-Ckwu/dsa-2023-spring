#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wrong_algo(char* s, char* key) {
    int m = strlen(key);
    int n = strlen(s);
    int idx_s = 0;
    while (idx_s <= n - m) {
        int idx_key = 0;
        while (idx_key < m && s[idx_s] <= key[idx_key]) {
            if (s[idx_s] < key[idx_key]) {
                int idx_ans = idx_s - idx_key;
                return idx_ans;
            }
            idx_key++;
            idx_s++;
        }
        if (idx_key == 0) {
            idx_s++;
        }
    }
    return -1;
}

int corrected_algo(char* s, char* key) {
    int m = strlen(key);
    int n = strlen(s);
    int idx_s = 0;
    while (idx_s <= n - m) {
        int idx_key = 0;
        while (idx_key < m && s[idx_s] <= key[idx_key]) {
            if (s[idx_s] < key[idx_key]) {
                int idx_ans = idx_s - idx_key;
                return idx_ans;
            }
            idx_key++;
            idx_s++;
        }
        idx_s = idx_s - idx_key + 1; // move the sliding window to the next position
    }
    return -1;
}

int main(void) {
    char* s = "edcba";
    char* key = "eda";

    printf("GPT's algorithm: %d\n", wrong_algo(s, key));
    printf("Corrected algorithm: %d\n", corrected_algo(s, key));

    return 0;
}