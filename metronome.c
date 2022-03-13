#include <stdbool.h>
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

typedef struct metronome {
    char *time_signature;
    uint8_t sub_div;
    uint16_t bpm;
} metronome_config_t;

int8_t metronome_init(metronome_config_t *metronome_config);
int8_t metronome(metronome_config_t *metronome_config);

int main(int argc, char *argv[])
{
    int8_t res = 0;
    
    metronome_config_t metronome_config;

    memset(&metronome_config, 0, sizeof(metronome_config));

    if (argc != 1)
    {
        if (argc == 4)
        {
            metronome_config.sub_div = (uint8_t)atof(argv[3]);
        }

        metronome_config.bpm = (uint16_t)atof(argv[2]);
        metronome_config.time_signature = argv[1];
    }

    if ((res = metronome_init(&metronome_config)) != 0)
    {
        return res;
    }

    res = metronome(&metronome_config);

    return res;
}

int8_t metronome_init(metronome_config_t *metronome_config)
{
    bool previous_config_found = false;
    bool new_config_provided = false;
    char default_time_signature[] = DEFAULT_TIME_SIGNATURE;
    FILE *fp;

    new_config_provided = ((metronome_config->bpm != 0) && (metronome_config->time_signature != NULL)) ? true : false;

    fp = fopen("data/config", "r");
    previous_config_found = (fp != NULL) ? true : false;

    if (!new_config_provided && !previous_config_found)
    {
        system("mkdir data");
        metronome_config->time_signature = default_time_signature;
        metronome_config->bpm = DEFAULT_BPM;
        metronome_config->sub_div = DEFAULT_SUB_DIV;
    }

    else if (!new_config_provided && previous_config_found)
    {
        char *buff = NULL;
        size_t len = 0;

        getline(&buff, &len, fp);
        buff[strcspn(buff, "\n")] = '\0';
        metronome_config->time_signature = strdup(buff);
        getline(&buff, &len, fp);
        buff[strcspn(buff, "\n")] = '\0';
        metronome_config->bpm = atoi(strdup(buff));
        getline(&buff, &len, fp);
        buff[strcspn(buff, "\n")] = '\0';
        metronome_config->sub_div = atoi(buff);

        free(buff);
        fclose(fp);
    }

    else if ((new_config_provided && !previous_config_found) || (new_config_provided && previous_config_found))
    {
        if (!previous_config_found)
        {
            system("mkdir data");
        }

        if (metronome_config->sub_div == 0)
        {
            metronome_config->sub_div = DEFAULT_SUB_DIV;
        }

        if (previous_config_found)
        {
            fclose(fp);
        }

        fp = fopen("data/config", "w+");

        if (fp == NULL)
        {
            return -1;
        }

        fprintf(fp, "%s\n", metronome_config->time_signature);
        fprintf(fp, "%d\n", metronome_config->bpm);
        fprintf(fp, "%d", metronome_config->sub_div);

        fclose(fp);
    }

    return 0;
}

int8_t metronome(metronome_config_t *metronome_config)
{
    if (metronome_config->bpm < MIN_BPM)
    {
        printf("BPM too low\n");

        return -1;
    }

    else if (metronome_config->bpm > MAX_BPM)
    {
        printf("BPM too high\n");

        return -1;
    }

    if (metronome_config->sub_div < MIN_SUB_DIV)
    {
        printf("Not enough sub-divisions\n");

        return -1;
    }
    
    else if (metronome_config->sub_div > MAX_SUB_DIV)
    {
        printf("Too many sub-divisions\n");

        return -1;
    }

    uint8_t top = (uint8_t)atof(strtok(metronome_config->time_signature, "/"));
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

    uint32_t tick = ((ONE_MIN_IN_NANOSECS / metronome_config->bpm / (metronome_config->sub_div / SUB_DIV_DIVIDER)));

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

    printf("Time Signature: %d/%d, BPM: %d Sub-division: %d\n", top, bottom, metronome_config->bpm, metronome_config->sub_div);

    while (1)
    {
        /* To count the number of beats in each measure */
        for (uint8_t i = 1; i <= top; i++)
        {
            /* To handle the sub-divisions as inputted by the user */
            for (uint8_t j = 1; j <= (metronome_config->sub_div / 4); j++)
            {
                printf("%d ", i);
                fflush(stdout);
                nanosleep(&tim, &tim_rem);
            }
        }
    }

    return 0;
}
