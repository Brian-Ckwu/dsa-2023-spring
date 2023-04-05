import random
from typing import Callable

MAX_N = 10

def findMissingID(a: list[int], l: int, r: int) -> int:
    while r - l > 1:
        m = (r + l) // 2
        lv = a[l] if l >= 0 else 0
        mv = a[m] if m >= 0 else 0
        if mv - lv == m - l:
            l = m
        else: # a[m] - a[l] == m - l
            r = m
    lv = a[l] if l >= 0 else 0
    return lv + 1

def findTwoMissingIDs(a: list[int]) -> tuple[int]:
    l, r = -1, len(a)
    while (r - l) > 1:
        m = (r + l) // 2
        lv = a[l] if l >= 0 else 0
        mv = a[m] if m >= 0 else 0
        if mv - lv == m - l: # right part has 2 missing numbers
            l = m
        elif mv - lv == m - l + 1: # both parts have 1 missing number
            return (findMissingID(a, l, m), findMissingID(a, m, r))
        else: # left part has 2 missing numbers
            r = m
    lv = a[l] if l >= 0 else 0
    return lv + 1, lv + 2

# Generate a test case for findTwoMissingIDs
def generate_test_case(n: int, k: int = 2) -> tuple[list[int], tuple[int]]:
    # sample k distinct number from [1, n]
    a = random.sample(range(1, n + 1), k)
    a.sort()
    n1, n2 = a[0], a[1]
    l = list()
    for i in range(1, n + 1):
        if i != n1 and i != n2:
            l.append(i)
    return (l, (n1, n2))

def test_algo(algo: Callable, test_gen: Callable, num_trials: int = 100):
    for _ in range(num_trials):
        n = random.randint(2, MAX_N)
        a, ans = test_gen(n)
        assert algo(a) == ans, f"Failed on {a}"
        print(f"Array length: {n}, answer: {ans} ...OK")

if __name__ == '__main__':
    test_algo(findTwoMissingIDs, generate_test_case, num_trials=100000)