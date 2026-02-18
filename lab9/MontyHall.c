/*
 * MontyHall.c
 * Monty Hall Simulator (interactive, single trial)
 *
 * Logic matches Lecture 9 example:
 *  - Prize randomly placed behind one of 3 doors (0,1,2)
 *  - Contestant picks a door
 *  - Host reveals one non-prize door not chosen by contestant
 *  - Contestant decides to stay or switch
 *  - Report WINNER/LOSER and reveal prize door
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    /* Where is the grand prize? */
    srand((unsigned)time(NULL));
    int prize = rand() % 3;          /* 0, 1, 2 */
    int notprize1 = (prize + 1) % 3;
    int notprize2 = (prize + 2) % 3;

    /* Header */
    printf("\n");
    printf(" =-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf(" ** Monty Hall Simulator **\n");
    printf(" =-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("\n");

    /* Ask contestant to pick a door */
    int pick = -1;
    while (pick < 0 || pick > 2)
    {
        printf(" Pick a door (0, 1, or 2): ");
        if (scanf("%d", &pick) != 1)
        {
            /* Clear invalid input */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            pick = -1;
        }
    }

    printf("\n You entered: %d\n", pick);

    /* Tell contestant about another door */
    int other;        /* door Monty opens (guaranteed not prize) */
    int other_other;  /* the remaining unopened door */

    printf("\n Interesting choice ...\n");

    if (pick == prize)
    {
        /* If contestant picked the prize, Monty can open either non-prize door */
        int ss = rand() % 2;
        if (ss == 0)
        {
            other = notprize1;
            other_other = notprize2;
        }
        else
        {
            other = notprize2;
            other_other = notprize1;
        }
    }
    else
    {
        /* If contestant did not pick prize, Monty must open the other non-prize door */
        other_other = prize;
        if (pick == notprize1)
            other = notprize2;
        else
            other = notprize1;
    }

    printf("\n I can tell you for sure that the prize is not behind door: %i\n", other);

    /* Ask if they want to change */
    int change = -1;
    while (change != 0 && change != 1)
    {
        printf("\n Stay with Door %i (press 0) or switch to Door %i (press 1): ",
               pick, other_other);

        if (scanf("%d", &change) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            change = -1;
        }
    }

    /* Final decision */
    int final_pick;
    if (change == 0)
    {
        final_pick = pick;
        printf("\n You stayed with Door %i\n", final_pick);
    }
    else
    {
        final_pick = other_other;
        printf("\n You switched to Door %i\n", final_pick);
    }

    /* Check answer */
    if (final_pick == prize)
        printf("\n *** WINNER ***\n\n");
    else
        printf("\n --- LOSER ---\n\n");

    printf(" The prize was behind Door %i\n\n", prize);

    return 0;
}

/*

cd lab9
bash running code
gcc MontyHall.c -o MontyHall
./MontyHall
*/