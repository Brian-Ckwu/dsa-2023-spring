def count_inverse_pairs(a: list[int]):
    count = 0
    for i in range(len(a) - 1):
        for j in range(i + 1, len(a)):
            if a[i] > a[j]:
                count += 1
    return count

if __name__ == "__main__":
    a = [1, 9, 2, 13, 10, 6, 3, 15, 14, 12, 11, 8, 7, 5, 4]
    print(count_inverse_pairs(a))