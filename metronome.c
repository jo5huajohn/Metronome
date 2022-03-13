#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_BPM             60
#define DEFAULT_SUB_DIV         4
#define DEFAULT_TIME_SIGNATURE  "4/4"

#define MAX_BPM                 400
#define MAX_NO_OF_BEATS         16
#define MAX_SUB_DIV             32
#define MAX_VALUE_OF_BEATS      8

#define MIN_BPM                 20
#define MIN_NO_OF_BEATS         1
#define MIN_SUB_DIV             4
#define MIN_VALUE_OF_BEATS      1

#define ONE_MIN_IN_NANOSECS     6e10
#define ONE_SEC_IN_NANOSECS     1e9
#define SUB_DIV_DIVIDER         4

int8_t metronome(char *time_signature, uint16_t bpm, uint8_t sub_div);

int main(int argc, char *argv[])
{
    char default_time_signature[] = "4/4";
    char *time_signature = default_time_signature;
    int8_t res = 0;
    uint8_t sub_div = DEFAULT_SUB_DIV;
    uint16_t bpm = DEFAULT_BPM;
    
    if (argc != 1)
    {
        if (argc == 4)
        {
            sub_div = (uint8_t)atof(argv[3]);
        }

        bpm = (uint16_t)atof(argv[2]);
        time_signature = argv[1];
    }

    res = metronome(time_signature, bpm, sub_div);

    return res;
}

int8_t metronome(char *time_signature, uint16_t bpm, uint8_t sub_div)
{
    if (bpm < MIN_BPM)
    {
        printf("BPM too low\n");

        return -1;
    }

    else if (bpm > MAX_BPM)
    {
        printf("BPM too high\n");

        return -1;
    }

    if (sub_div < MIN_SUB_DIV)
    {
        printf("Not enough sub-divisions\n");

        return -1;
    }
    
    else if (sub_div > MAX_SUB_DIV)
    {
        printf("Too many sub-divisions\n");

        return -1;
    }

    uint8_t top = (uint8_t)atof(strtok(time_signature, "/"));
    uint8_t bottom = (uint8_t)atof(strtok(NULL, "/"));

    if ((top < MIN_NO_OF_BEATS) || (bottom < MIN_VALUE_OF_BEATS))
    {
        printf("Time signature is too high\n");

        return -1;
    }

    else if ((top > MAX_NO_OF_BEATS) || (bottom > MAX_VALUE_OF_BEATS))
    {
        printf("Time signature is too high\n");

        return -1;
    }

    uint32_t tick = ((ONE_MIN_IN_NANOSECS / bpm / (sub_div / SUB_DIV_DIVIDER)));

    struct timespec tim, tim_rem;

    memset(&tim, 0, sizeof(tim));

    if (tick >= ONE_SEC_IN_NANOSECS)
    {
        tim.tv_sec = 1;
        tim.tv_nsec = tick - ONE_SEC_IN_NANOSECS;
    }

    else
    {
        tim.tv_nsec = tick;
    }

    printf("Time Signature: %d/%d, BPM: %d Sub-division: %d\n", top, bottom, bpm, sub_div);

    while (1)
    {
        /* To count the number of beats in each measure */
        for (uint8_t i = 1; i <= top; i++)
        {
            /* To handle the sub-divisions as inputted by the user */
            for (uint8_t j = 1; j <= (sub_div / 4); j++)
            {
                printf("%d ", i);
                fflush(stdout);
                nanosleep(&tim, &tim_rem);
            }
        }
    }

    return 0;
}
