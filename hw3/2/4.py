from typing import List

def calc_each_num_substrings(S: str, CH: List[str]) -> List[int]:
    N = len(S) - 1
    C = len(CH) - 1
    Map = dict()
    for i in range(1, C + 1):
        Map[CH[i]] = i
    ans = [0 for _ in range(C + 1)]
    pos = [0 for _ in range(C + 1)]
    ns = [0 for _ in range(N + 1)]
    ns[1] = 1
    for i in range(2, N + 1):
        ns[i] = ns[i - 1] + i
    for i in range(1, N + 1):
        ch = S[i]
        index = Map[ch]
        num_chars = i - pos[index] - 1
        if num_chars > 0:
            num_substrings = ns[num_chars]
            ans[index] = ans[index] + num_substrings
        pos[index] = i
    for i in range(1, C + 1):
        index = Map[CH[i]]
        num_chars = (N + 1) - pos[index] - 1
        if num_chars > 0:
            ans[index] = ans[index] + ns[num_chars]
    return ans      

if __name__ == "__main__":
    testcases = [
        {"S": " abbc", "CH": [' ', 'a', 'b', 'c']},
        {"S": " apple", "CH": [' ', 'a', 'p', 'l', 'e']}
    ]
    
    for i, testcase in enumerate(testcases):
        print(f"Testcase #{i}: {calc_each_num_substrings(**testcase)}")