import random

# O(n^2)
def count_valid_pairs_naive(kds: list[tuple[int]]) -> int:
    count = 0
    for i in range(len(kds) - 1):
        for j in range(i + 1, len(kds)):
            if (kds[i][0] - kds[j][0]) * (kds[i][1] - kds[j][1]) > 0:
                count += 1
    return count

# O(nlogn)
def count_valid_pairs_merge(kds: list[tuple[int]]) -> int:
    kds.sort(key=lambda x: (x[0], -x[1])) # sort by kds[0] ascending, kds[1] descending
    tmp = kds.copy() # for storing sorted kds by kds[1]
    count = 0
    w = 1
    while True:
        w = w * 2
        l = 0
        while l < len(kds):
            r = min(l + w - 1, len(kds) - 1)
            m = min(int(l + w / 2 - 1), len(kds) - 1)
            lp = l
            rp = m + 1
            p = l
            while (lp <= m) and (rp <= r):
                if (kds[lp][1] < kds[rp][1]):
                    count += r - rp + 1
                    tmp[p] = kds[lp]
                    lp += 1
                else:
                    tmp[p] = kds[rp]
                    rp += 1
                p += 1
            while lp <= m:
                tmp[p] = kds[lp]
                lp += 1
                p += 1
            while rp <= r:
                tmp[p] = kds[rp]
                rp += 1
                p += 1
            for i in range(l, r + 1):
                kds[i] = tmp[i]
            l += w
        if w >= len(kds):
            break
    return count

def gen_test_case(n: int) -> list[tuple[int]]:
    return [(random.randint(1, 100), random.randint(1, 100) / 10) for _ in range(n)]

def run_pair_tests(n_min: int = 1, n_max: int = 100, num_cases: int = 100) -> int:
    for n in range(n_min, n_max + 1):
        print(f"===== n = {n} =====")
        for case in range(num_cases):
            kds = gen_test_case(n)
            print(f"Case {case}:", end=" ...")
            naive_count = count_valid_pairs_naive(kds)
            merge_count = count_valid_pairs_merge(kds)
            if naive_count == merge_count:
                print(f"Pass")
            else:
                print(f"Fail (naive: {naive_count}, merge: {merge_count})")
                return 1
    return 0

if __name__ == "__main__":
    testcase = [(2, 1.4), (3, 1.6), (1, 0.3), (6, 2.3), (4, 1.0)]
    naive_count = count_valid_pairs_naive(testcase)
    print(naive_count)
    merge_count = count_valid_pairs_merge(testcase)
    print(merge_count)
    
    run_pair_tests()