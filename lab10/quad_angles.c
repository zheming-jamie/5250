/* quad_angles.c */
#include <math.h>
#include "quad.h"

static double signed_area(const quad *q)
{
    double s = 0.0;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        s += q->p[i].x * q->p[j].y - q->p[i].y * q->p[j].x;
    }
    return 0.5 * s;
}

void quad_angles(const quad *q, double angles_deg[4])
{
    /* Standard C way to get pi (portable): */
    const double PI = acos(-1.0);

    double A = signed_area(q);
    double orient = (A >= 0.0) ? 1.0 : -1.0; /* +1 CCW, -1 CW */

    for (int i = 0; i < 4; i++) {
        point prev = q->p[(i + 3) % 4];
        point cur  = q->p[i];
        point next = q->p[(i + 1) % 4];

        /* incoming edge (prev -> cur), outgoing edge (cur -> next) */
        double e1x = cur.x - prev.x;
        double e1y = cur.y - prev.y;
        double e2x = next.x - cur.x;
        double e2y = next.y - cur.y;

        double dot   = e1x*e2x + e1y*e2y;
        double cross = e1x*e2y - e1y*e2x;

        /* signed turning angle theta in (-PI, PI] */
        double theta = atan2(cross, dot);

        /* internal angle alpha (can exceed 180° for concave vertices) */
        double alpha = PI - orient * theta;

        angles_deg[i] = alpha * (180.0 / PI);
    }
}
