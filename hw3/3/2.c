#include <stdio.h>
#include <stdlib.h>

int djs[16], ind[16];
int cnt = 0;

void djs_make_set(int x)
{
    djs[x] = x;
    ind[x] = rand();
}

int djs_find_set(int x)
{
    cnt++;
    if (djs[x] == x) return x;
    return djs_find_set(djs[x]);
}

void djs_union(int x, int y)
{
    x = djs_find_set(x), y = djs_find_set(y);
    if (x == y) return;
    if (ind[x] < ind[y])
        djs[x] = y;
    else
        djs[y] = x;
}

int main()
{
    srand(1126);
    for(int i = 0; i < 16; i++) djs_make_set(i);
    for(int i = 0; i < 16; i++) printf("%d: %d\n", i, ind[i]);
    // sequence (small to large) of seed 1126: 15 0 4 11 9 12 6 14 1 5 10 7 2 13 3 8

    // Modify the following sample test case
    djs_union(15, 0); // 15 is tail, 0 is leader
    djs_union(15, 4); // 15 is tail, 4 is leader
    djs_union(15, 11);
    djs_union(15, 9);
    djs_union(15, 12);
    djs_union(15, 6);
    djs_union(15, 14);
    djs_union(15, 1);
    djs_union(15, 5);
    djs_union(15, 10);
    djs_union(15, 7);
    djs_union(15, 2);
    djs_union(15, 13);
    djs_union(15, 3);
    djs_union(15, 8);
    // ---------------------------

    printf("cnt = %d\n", cnt);
}