from heapq import heapify, heappop, heappush

def fuse_materials(a: list[int]) -> int:
    cost = 0
    heapify(a)
    print(a)
    while (len(a) > 1):
        x = heappop(a)
        y = heappop(a)
        cost += x + y
        heappush(a, x + y)
        print(a, end=" ")
        print(f"Cost: {cost}")
    return cost

def fuse_materials_without_heap(a: list[int]) -> int:
    cost = 0
    while (len(a) > 1):
        x = a[0]
        y = a[1]
        cost += x + y
        a.remove(x)
        a[0] = x + y
    return cost

if __name__ == "__main__":
    a = [7, 6, 5, 4, 3, 2, 1]
    cost = fuse_materials(a)
    print(cost)
    a = [7, 6, 5, 4, 3, 2, 1]
    larger_cost = fuse_materials_without_heap(a)
    print(larger_cost)
    