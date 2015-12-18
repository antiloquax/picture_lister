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

int main(int argc, char *argv[])
{
    /* Declarations. */
    FILE *f;
    char name[100], end[5];
    char *start[] = {"Congleton ", "Biddulph ", "Sandbach ", "Alsager "};
    char *eds = "CBSA", *ext = ".jpg", *ptr;
    int k, y;

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
		/* Copy filename into name. */
        strcpy(name, drecord -> d_name);
        
        /* Only look at the right type of file (check length and extension). */
        if((k = strlen(name) - 4) < LEN)
            continue;

		/* Put final four characters of name in end.*/
		sprintf(end, "%.4s", &name[k]);
        /* Check extension is right. */
        if (strcasecmp(ext, end))
            continue;

         /* Do archive pics first. */
        if (name[0] >= '0' && name[0] <= '9')
        {
			/* Write new name, using fprintf to slice. */
            fprintf(f, "Chronicle Archive photo %.9s\n", name);
            continue;
        }

    /* Use the first letter of the filename to identify edition. */
	/* Get a pointer to matching character in eds. */
    ptr = strchr(eds, name[0]);
	/* Use pointer arithmetic to find the index. */
    k = (int)(ptr - eds);
    if (k < 0 || k > 3)
        continue;

	/* Write new name, using fprintf to slice. */
    fprintf(f, "%sChronicle Photo. %.5s/%d\n", start[k], &name[4], y);
    }

    /* Close file and directory. */
    fclose(f);
    closedir(dhandle);
	return 0;
}

/* Simple error message and quit function. */
void exErr(char *msg)
{
    printf("ERROR: %s\n", msg);
    exit(1);
}

