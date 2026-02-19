#ifndef __QUAD_H__
#define __QUAD_H__

typedef struct point {
    double x;
    double y;
} point;

typedef struct quad {
    point p[4];   /* vertices in order */
} quad;

double quad_perimeter(const quad *q);
double quad_area(const quad *q);
void   quad_angles(const quad *q, double angles_deg[4]);

#endif
