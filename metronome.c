#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void metronome(int top, int bottom, int bpm, int subdiv);

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "-h") == 0)
    {
    }

    else if (argc == 4)
    {
        int top = atoi(strtok(argv[1], "/"));
        int bottom = atoi(strtok(NULL, "/"));
        int bpm = atoi(argv[2]);
        int subdiv = atoi(argv[3]);

        if (bpm > 400)
        {
            printf("BPM too high\n");
            return -1;
        }

        metronome(top, bottom, bpm, subdiv);
    }
    
    return 0;
}

void metronome(int top, int bottom, int bpm, int subdiv)
{
    printf("%d/%d, BPM: %d\n", top, bottom, bpm);

    while (1)
    {
        for (int i = 1; i <= top; i++)
        {
            for (int j = 1; j <= (subdiv / 4); j++)
            {
                printf("%d ", i);
                fflush(stdout);
                usleep((60000 / bpm / (subdiv / 4)) * 1000);
            }
        }
    }
}
