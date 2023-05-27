def compute_prefix_function(key: str) -> list[int]:
    pi = [0 for _ in range(len(key))]
    k = 0
    for i in range(1, len(key)):
        while (k > 0) and (key[k] != key[i]):
            k = pi[k - 1]
        if key[k] == key[i]:
            k += 1
        pi[i] = k
    return pi

def kmp_matcher(s: str, key: str) -> list[int]:
    n = len(s)
    m = len(key)
    pi = compute_prefix_function(key)
    q = 0
    for i in range(n):
        while (q > 0) and (s[i] != key[q]):
            q = pi[q - 1]
        if s[i] == key[q]:
            q = q + 1
        if q == m:
            return i - m + 1
    return -1

def kmp_smaller(s: str, key: str) -> list[int]:
    n = len(s)
    m = len(key)
    pi = compute_prefix_function(key)
    q = 0
    for i in range(n):
        while (q > 0) and (s[i] > key[q]):
            q = pi[q - 1]
        if s[i] < key[q]:
            return i - q
        elif s[i] == key[q]:
            q = q + 1
        else: # s[i] > key[q]:
            pass # do nothing
        if q == m:
            q = pi[q - 1] # continue to find the next
    return -1

if __name__ == "__main__":
    # test compute_prefix_function
    print("Testing compute_prefix_function...")
    keys = ["abcabc", "ababab", "abcabdabcabc"]
    for key in keys:
        print(compute_prefix_function(key))
        
    # test kmp_matcher
    print("Testing kmp_matcher...")
    print(kmp_matcher(s="abcabdabcabc", key="abcabc"))
    
    # test kmp_smaller
    print("Testing kmp_smaller...")
    testcases = [
        {"s": "edcba", "key": "eda"},
        {"s": "abcabdabcabc", "key": "abcabc"}
    ]
    for testcase in testcases:
        print(kmp_smaller(**testcase))