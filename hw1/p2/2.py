def validate_a(a: list[int]) -> bool:
    if not a:
        return True
    l = 0
    r = 0
    while (l < len(a)) and (r < len(a)):
        while (r < len(a)):
            if (a[r] == 2 * a[l]):
                a[l] = 0
                a[r] = 0
                break
            r += 1
        while (l < len(a)) and (a[l] == 0):
            l += 1
    return l == len(a)

if __name__ == "__main__":
    testcases = [
        ([1, 2, 2, 3, 4, 5, 6, 10], True),
        ([1, 2, 2, 4, 7, 8, 10, 14], False),
        ([1, 1, 1, 1, 2, 2, 2, 2], True),
        ([1, 2], True),
        ([1, 1], False),
        ([], True),
        ([1, 1, 1, 1, 1, 1, 1, 1], False),
        ([1, 1, 2, 2, 3, 3, 4, 4], False),
        ([1, 1, 2, 2, 3, 3, 4, 4, 6, 6, 8, 8], True),
        ([10, 20], True),
        ([1, 1], False),
        ([0, 1], False),
        ([1, 0], False),
        ([1, 2, 3, 5, 6, 10], True),
        ([1, 2, 3, 4, 5, 6], False),
        ([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], False)
    ]
    for testcase, ans in testcases:
        print(testcase, end=" -> ")
        pred = validate_a(testcase)
        if pred == ans:
            print(f"{testcase} = {pred} ...ok")
        else:
            print(f"{testcase}...failed, expected {ans}, got {pred}")
            raise ValueError("testcase failed")
