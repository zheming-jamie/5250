#include <math.h>
#include "quad.h"

double quad_area(const quad *q)
{
    double s = 0.0;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        s += q->p[i].x * q->p[j].y - q->p[i].y * q->p[j].x;
    }
    return 0.5 * fabs(s);
}
