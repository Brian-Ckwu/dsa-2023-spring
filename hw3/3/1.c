#include <stdio.h>
#include <stdlib.h>

int djs[16];
int cnt = 0;

void djs_make_set(int x)
{
    djs[x] = x;
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
    if (rand() % 2 == 0)
        djs[x] = y;
    else
        djs[y] = x;
}

int main()
{
    srand(1126);
    for(int i = 0; i < 16; i++) djs_make_set(i);

    // merge sequence of seed 1126
    // y -> x
    // y -> x
    // x -> y
    // y -> x
    // y -> x
    // x -> y
    // x -> y
    // x -> y
    // x -> y
    // x -> y
    // x -> y
    // x -> y
    // x -> y
    // y -> x
    // x -> y

    // Modify the following sample test case
    djs_union(1, 0); // 0 is tail, 1 is leader
    djs_union(2, 0); // 0 is tail, 2 is leader
    djs_union(0, 3); // 0 is tail, 3 is leader
    djs_union(4, 0); // 0 is tail, 4 is leader
    djs_union(5, 0); // 0 is tail, 5 is leader
    djs_union(0, 6); // 0 is tail, 6 is leader
    djs_union(0, 7); // 0 is tail, 7 is leader
    djs_union(0, 8); // 0 is tail, 8 is leader
    djs_union(0, 9); // 0 is tail, 9 is leader
    djs_union(0, 10); // 0 is tail, 10 is leader
    djs_union(0, 11); // 0 is tail, 11 is leader
    djs_union(0, 12); // 0 is tail, 12 is leader
    djs_union(0, 13); // 0 is tail, 13 is leader
    djs_union(14, 0); // 0 is tail, 14 is leader
    djs_union(0, 15); // 0 is tail, 15 is leader
    // ---------------------------

    printf("cnt = %d\n", cnt);
}