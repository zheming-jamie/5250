#include <math.h>
#include "quad.h"

static double dist(point a, point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

double quad_perimeter(const quad *q) {
    double per = 0.0;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        per += dist(q->p[i], q->p[j]);
    }
    return per;
}
