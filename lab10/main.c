#include <stdio.h>
#include "quad.h"

int main(void)
{
    printf("\nI am in the **main** function.\n");

    quad q;

    /* Define quadrilateral vertices in boundary order (CW or CCW) */
    q.p[0].x = 0.0; q.p[0].y = 0.0;
    q.p[1].x = 2.0; q.p[1].y = 0.0;
    q.p[2].x = 3.0; q.p[2].y = 0.0;
    q.p[3].x = 4.0; q.p[3].y = 4.0;

    double per  = quad_perimeter(&q);
    double area = quad_area(&q);

    double ang[4];
    quad_angles(&q, ang);

    printf("\nVertices:\n");
    for (int i = 0; i < 4; i++) {
        printf("  p[%d] = ( %12.5e, %12.5e )\n", i, q.p[i].x, q.p[i].y);
    }

    printf("\nPerimeter = %12.5e\n", per);
    printf("Area      = %12.5e\n", area);

    printf("\nInner angles (deg):\n");
    for (int i = 0; i < 4; i++) {
        printf("  angle[%d] = %12.5e\n", i, ang[i]);
    }
    printf("\n");

    return 0;
}


/*
cd lab10

gcc quad_area.c quad_perimeter.c quad_angles.c main.c -o main.exe -lm
./main.exe
*/

/*
gcc -c quad_area.c quad_perimeter.c quad_angles.c main.c
gcc quad_area.o quad_perimeter.o quad_angles.o main.o -o main.exe -lm
./main.exe > output.txt
*/