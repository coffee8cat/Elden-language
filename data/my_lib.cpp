#include <stdio.h>

double elem_in() {
    double x = 0.0;
    scanf("%lf", &x);

    return x;
}

void elem_out(double x) {
    printf("%lf", x);
}
