#include <stdio.h>

/* 1 -> 3
 * 1 -> 2
 * 3 -> 2
 * 1 -> 3
 * 2 -> 1
 * 2 -> 3
 * 1 -> 3
 */

void hanoiTower(int dishs, int peg1, int peg2, int peg3)
{
    if (dishs == 1) {
        printf("dish %d move to %d\n", peg1, peg3);
        return;
    } else {
        hanoiTower(dishs - 1, peg1, peg3, peg2);
        printf("dish %d move to %d\n", peg1, peg3);
        hanoiTower(dishs - 1, peg2, peg1, peg3);
    }
}

int main(void)
{
    hanoiTower(3, 1, 2, 3);
    return 0;
}
