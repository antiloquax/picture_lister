/* 
 * C picture file lister for Congleton Chronicle.
 * by Mark Tranter (tranter.m@sky.com).
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/* Constant for min. length of file name. */
#define LEN 9

/* Function prototypes. */
void exErr(char *msg);
void slice(char *dest, char *src, int l);

int main(int argc, char *argv[])
{
    /* Declarations. */
    FILE *f;
    char name[100], temp[100], num[10], end[5];
    char *start[] = {"Congleton ", "Biddulph ", "Sandbach ", "Alsager "};
    char *eds = "CBSA", *ext = ".jpg", *ptr;
    int i, k, y;

    /* Open the file for writing */
    f = fopen("plist.txt", "w");

    /* Get the year */
    struct tm *t;
    time_t now;
    time(&now);
    t = localtime(&now);
    y = (t -> tm_year) % 100;

    /* Open the directory and look for files */
    DIR *dhandle;
    struct dirent *drecord;
    dhandle = opendir(".");
    if (dhandle == NULL)
        exErr("Could not open directory.\n");

    while ((drecord = readdir(dhandle)) != NULL)
    {
        /* Make sure strings are empty. */
        temp[0] = '\0';
        num[0]  = '\0';
        strcpy(name, drecord -> d_name);
        
        /* Only look at the right type of file (check length and extension). */
        if((k = strlen(name) - 4) < LEN)
            continue;
        slice(end, &name[k], 4);
        if (strcasecmp(ext, end))
            continue;

         /* Do archive pics first. */
        if (name[0] >= '0' && name[0] <= '9')
        {
            slice(num, name, 9);
            sprintf(temp, "Chronicle Archive photo %s", num);
            fprintf(f, "%s\n", temp);
            continue;
        }

    /* Use the first letter of the filename to identify edition. */
    ptr = strchr(eds, name[0]);
    k = (int)(ptr - eds);
    if (k < 0 || k > 3)
        continue;

    slice(num, &name[4], 5);
    fprintf(f, "%sChronicle Photo. %s/%d\n", start[k], num, y);
    }

    /* Close file and directory. */
    fclose(f);
    closedir(dhandle);
}


/* Slice: returns a substring. Dest must be big enough.
 * Pass dest, start address in src and length. */
void slice(char *dest, char *src, int l)
{
    int i;
    for (i = 0; i < l; i++)
       *dest++ = *src++;
    *dest = '\0';

}


/* Simple error message and quit function. */
void exErr(char *msg)
{
    printf("ERROR: %s\n", msg);
    exit(1);
}


