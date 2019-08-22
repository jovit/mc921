#include <stdio.h>
// remoção de variáveis desnecessárias
// h = ((a + 20) * b + 5) * 2 + 1
// 
int main(void)
{
    int a, b, c, d, e, f, g, h;

    scanf("%d", &a);
    scanf("%d", &b);
    c = 20;
    d = a + c;
    e = d * b;
    f = e + 5;
    g = f * 2;
    h = g + 1;
    printf("%d", h);

    return 0;
}
