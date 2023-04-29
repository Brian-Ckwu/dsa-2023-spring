import time
import random

def count_reversions_naive(arr: list[int]) -> tuple[int, float]:
    start = time.time()
    count = 0
    for i in range(len(arr) - 1):
        for j in range(i + 1, len(arr)):
            if arr[i] > arr[j]:
                count += 1
    end = time.time()
    return count, end - start

def count_reversions_merge(arr: list[int]) -> tuple[int, float]:
    start = time.time()
    count = 0
    tmp = arr.copy()
    w = 1
    while True:
        w *= 2
        l = 0
        while l < len(arr):
            r = min(l + w - 1, len(arr) - 1)
            m = min(l + w // 2 - 1, len(arr) - 1)
            lp = l
            rp = m + 1
            p = l
            while (lp <= m) and (rp <= r):
                if arr[lp] > arr[rp]:
                    count += r - rp + 1
                    tmp[p] = arr[lp]
                    lp += 1
                else:
                    tmp[p] = arr[rp]
                    rp += 1
                p += 1
            while (lp <= m):
                tmp[p] = arr[lp]
                lp += 1
                p += 1
            while (rp <= r):
                tmp[p] = arr[rp]
                rp += 1
                p += 1
            for i in range(l, r + 1):
                arr[i] = tmp[i]
            l += w
        if w >= len(arr):
            break
    end = time.time()
    return count, end - start

def gen_test_case(n: int) -> list[int]:
    return [random.randint(0, 100) for _ in range(n)]

def run_pair_tests(n_trials: int = 10, n_max: int = 1000) -> bool:
    for n in range(1, n_max + 1):
        for trial in range(n_trials):
            arr = gen_test_case(n)
            naive_count, naive_time = count_reversions_naive(arr)
            merge_count, merge_time = count_reversions_merge(arr)
            if naive_count != merge_count:
                print(f"Test #{trial}: failed (naive_time = {naive_time}, merge_time = {merge_time})")
                return False
            else:
                print(f"Test #{trial}: passed (naive_time = {naive_time}, merge_time = {merge_time}))")
    return True

if __name__ == "__main__":
    testcase = [6, 5, 9, 2, 0, 5, 8, 1]
    naive_count = count_reversions_naive(testcase)
    merge_count = count_reversions_merge(testcase)
    print(naive_count, merge_count)
    
    run_pair_tests(1, 10000)