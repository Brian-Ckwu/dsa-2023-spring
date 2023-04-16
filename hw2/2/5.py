import time
import math
import random
from colorama import Fore, Style
# generate two sorted array of size n and a k <= n
def gen_test_case(n: int, max_element: int = 100):
    assert n >= 1
    a = [random.randint(0, max_element) for i in range(n)]
    b = [random.randint(0, max_element) for i in range(n)]
    a.sort()
    b.sort()
    k = random.randint(1, n)
    return a, b, k

def find_kth_nlogn(a, b, k):
    c = a + b
    c.sort()
    return c[k - 1]

def find_kth_n(a, b, k):
    c = []
    p1 = 0
    p2 = 0
    while (p1 < len(a)) and (p2 < len(b)):
        if a[p1] < b[p2]:
            c.append(a[p1])
            p1 += 1
        else:
            c.append(b[p2])
            p2 += 1
    while p1 < len(a):
        c.append(a[p1])
        p1 += 1
    while p2 < len(b):
        c.append(b[p2])
        p2 += 1
    return c[k - 1]

def find_kth_logn(a, b, k):
    pa = -1
    pb = -1
    rest = k
    while rest > 0:
        steps = math.ceil(rest / 2)
        if a[pa + steps] <= b[pb + steps]:
            pa += steps
        else:
            pb += steps
        rest -= steps
    if pa == -1:
        return b[pb]
    if pb == -1:
        return a[pa]
    return max(a[pa], b[pb])

def run_trials(ntrials: int, n: int = 10, max_element: int = 100, verbose: bool = False):
    for i in range(ntrials):
        a, b, k = gen_test_case(n, max_element)
        if verbose:
            print("a = ", a)
            print("b = ", b)
            print("k = ", k)
            print(f"ans = {find_kth_nlogn(a, b, k)}")
        if find_kth_nlogn(a, b, k) == find_kth_n(a, b, k) == find_kth_logn(a, b, k):
            print(f"Test {i}: OK")
        else:
            raise ValueError(Fore.RED + f"Test {i}: Wrong Answer" + Style.RESET_ALL)
        
def calc_execute_time(func, a, b, k):
    start = time.time()
    func(a, b, k)
    end = time.time()
    return end - start

if __name__ == "__main__":
    ntrials = 100000
    max_element = 10
    n = 10
    # a, b, k = gen_test_case(n, max_element)
    # # calc execute time of three functions
    # nlogn_time = calc_execute_time(find_kth_nlogn, a, b, k)
    # logn_time = calc_execute_time(find_kth_logn, a, b, k)
    # # print out the result
    # for name, t in zip(["nlogn", "logn"], [nlogn_time, logn_time]):
    #     print(f"Time of {name} is {t} seconds")
    run_trials(ntrials, n, max_element)