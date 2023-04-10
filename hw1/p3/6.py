import random
import unittest
from colorama import Fore, Style
from collections import deque

def print_queues(q1: deque, q2: deque):
    print("q1: {}".format(q1))
    print("q2: {}".format(q2))
    print("------------------------")

def calc_last_distance(pos: list[float], m: int) -> float:
    # suppose len(pos) >= 2 and m >= 1
    q1 = deque()
    q2 = deque()
    # enquene all distances into q1 -> Time: O(n), Space: O(n)
    for i in range(len(pos) - 1):
        q1.append(pos[i + 1] - pos[i])
    # repeat m times
    for _ in range(m):
        d1 = q1[0] # peek left
        d2 = q2[0] if len(q2) > 0 else 0
        if d1 >= d2:
            q1.popleft()
            dist = d1 / 2
        else:
            q2.popleft()
            dist = d2 / 2
        q2.append(dist)
        q2.append(dist)
        
        if len(q1) == 0:
            q1, q2 = q2, q1
    return dist

def calc_last_distance_naive(pos: list[float], m: int) -> float:
    l = list()
    for i in range(len(pos) - 1):
        l.append(pos[i + 1] - pos[i])
    for _ in range(m):
        dist = l[0] / 2
        l[0] = dist
        l.append(dist)
        l.sort(reverse=True)
    return dist

def gen_test_case(n: int) -> tuple[list[float], int]:
    m = random.randint(1, 10 * n)
    dists = list(random.sample(range(1, 3 * n), n))
    dists.sort(reverse=True)
    pos = [0]
    for d in dists:
        pos.append(pos[-1] + d)
    return pos, m

# Make a unittest class for the function calc_last_distance using the following test cases:
class TestCalcLastDistance(unittest.TestCase):
    trials = 100
    n = 2
    
    def test_calc_last_distance(self):
        for _ in range(self.trials):
            pos, m = gen_test_case(self.n)
            print(f"pos={pos}, m={m}", end=" ...")
            ans1 = calc_last_distance(pos, m)
            ans2 = calc_last_distance_naive(pos, m)
            self.assertEqual(ans1, ans2, msg=Fore.RED + "FAILED" + Style.RESET_ALL)
            print(Fore.GREEN + f"PASSED (ans={ans1})" + Style.RESET_ALL)
                
if __name__ == "__main__":
    unittest.main()