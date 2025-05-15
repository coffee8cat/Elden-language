#include <stdio.h>

extern "C" {
    double elem_in() {
        double x = 0.0;
        scanf("%lf", &x);

        return x;
    }

    void elem_out(double x) {
        printf("%lf\n", x);
    }
}
