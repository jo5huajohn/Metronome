#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void metronome(char *time_signature, int bpm, int sub_div);

int main(int argc, char *argv[])
{
    int sub_div = 4;

    if (argc == 4)
    {
        sub_div = atoi(argv[3]);
    }

    int bpm = atoi(argv[2]);
    
    if (bpm > 400)
    {
        printf("BPM too high\n");
        return -1;
    }

    metronome(argv[1], bpm, sub_div);

    return 0;
}

void metronome(char *time_signature, int bpm, int sub_div)
{
    int top = atoi(strtok(time_signature, "/"));
    int bottom = atoi(strtok(NULL, "/"));
    long tick = ((60000 / bpm / (sub_div / 4)) * 1000000);

    struct timespec tim, tim_rem;

    memset(&tim, 0, sizeof(tim));

    if (tick == 1000000000)
    {
        tim.tv_sec = 1;
    }

    else
    {
        tim.tv_nsec = tick;
    }

    printf("%d/%d, BPM: %d\n", top, bottom, bpm);

    while (1)
    {
        /* To count the number of beats in each measure */
        for (int i = 1; i <= top; i++)
        {
            /* To handle the sub-divisions as inputted by the user */
            for (int j = 1; j <= (sub_div / 4); j++)
            {
                printf("%d ", i);
                fflush(stdout);
                nanosleep(&tim, &tim_rem);
            }
        }
    }
}
