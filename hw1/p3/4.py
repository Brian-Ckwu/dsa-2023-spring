import unittest
from colorama import Fore, Style

def print_stacks(s0: list, s1: list, s2: list):
    print("s0: {}".format(s0))
    print("s1: {}".format(s1))
    print("s2: {}".format(s2))
    print("------------------------")

def find_largest(s: list):
    max_val = -float("inf")
    if s:
        for e in s:
            if e > max_val:
                max_val = e
    return max_val

def check_stack(s: list, m: int):
    if s:
        for i in range(len(s) - 1):
            if s[i] == m:
                return False
    return True

def verify_stack(s0: list) -> bool:
    s1, s2 = [], []
    while s0 or s1: # s0 or s1 is not empty
        m0 = find_largest(s0)
        m1 = find_largest(s1)
        m = max(m0, m1)
        while s1 and (s1[-1] == m):
            e = s1.pop()
            s2.append(e)
        valid = check_stack(s1, m)
        if not valid:
            return False
        while s0 and (m0 == m) and (s0[-1] < m): # m0 == m -> there is a largest element in s0 -> need to remove top elements (if any < m) in s0
            e = s0.pop()
            s1.append(e)
        while s0 and (s0[-1] == m):
            e = s0.pop()
            s2.append(e)
    return True

class TestVerifyStack(unittest.TestCase):
    def test_verify_stack(self):
        testcases = [
            ([3, 2, 1, 4], True),
            ([3, 2, 4, 1], True),
            ([3, 4, 2, 1], True),
            ([3, 4, 1, 2], False),
            ([3, 1, 4, 2], False),
            ([3, 1, 2, 4], False),
            ([4, 3, 2, 1], True),
            ([], True),
            ([1], True),
            ([1, 2], True),
            ([2, 1], True),
            ([1, 2, 3], True),
            ([1, 3, 2], True),
            ([10, 9, 8, 7, 6, 5, 4, 3, 2, 1], True),
            ([10, 9, 8, 7, 6, 5, 4, 3, 1, 2], False),
            ([4, 3, 2, 1, 5, 6, 7, 10, 9, 8], True),
            ([10, 9, 8, 7, 1, 2, 3, 4, 5], False),
            ([9, 8, 10, 10, 10, 2, 3, 4, 10], False),
            ([9, 8, 10, 10, 10, 4, 3, 2, 10], True),
            ([10, 10, 9, 8, 10, 10, 10, 4, 3, 2, 10], True),
            ([10, 10, 8, 9, 10, 10, 10, 4, 3, 2, 10], False),
            ([1, 1, 1, 1, 1, 1], True),
            ([2, 1, 2, 1, 2, 1], True),
            ([2, 1, 1, 2, 1, 2, 1, 1, 3], True),
            ([3, 2, 1, 1, 2, 1, 2, 1, 1, 3], False),
            ([3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1, 1], False),
            ([3, 3, 2, 2, 1, 1], True)
        ]
        for testcase in testcases:
            teststack = testcase[0].copy()
            self.assertEqual(verify_stack(testcase[0]), testcase[1], Fore.RED + "Test case failed: {}".format(teststack))
            print(Fore.GREEN + "Test case passed: {}".format(teststack) + Style.RESET_ALL)

if __name__ == "__main__":
    unittest.main()