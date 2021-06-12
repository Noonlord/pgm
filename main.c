#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

int getPGMColumns(char *fileName);
int getPGMRows(char *fileName);
int *getPGMData(char *fileName);
FILE *readFile(char *fileName);
int main()
{
    FILE *fp;
    char ch, fileName[25];
    printf("Enter filename: ");
    gets(fileName);

    fp = readFile(fileName);

    int rows = getPGMRows(fileName);
    int columns = getPGMColumns(fileName);
    int *imgArrPtr = getPGMData(fileName);
    int length = rows * columns;
    int i;
    for (i = 0; i < length; i++)
    {
        int count = 1;
        while (i < length - 1 && *(imgArrPtr + i) == *(imgArrPtr + i + 1))
        {
            count++;
            i++;
        }
        printf("%d %d ", count, *(imgArrPtr + i));
    }
    return 0;
}

int getPGMColumns(char *fileName)
{
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    int counter = 0;
    int columns;
    char ch;
    while ((ch = fgetc(fp)) != EOF && counter < 2)
    {
        char buffer[4];
        if (ch != ' ' & ch != '\n')
        {
            buffer[reading] = ch;
            reading++;
        }
        else
        {
            reading = 0;
            counter++;
            if (counter == 2)
            {
                sscanf(buffer, "%d", &columns);
                return columns;
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
}

int getPGMRows(char *fileName)
{
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    int counter = 0;
    int rows;
    char ch;
    while ((ch = fgetc(fp)) != EOF && counter < 3)
    {
        char buffer[4];
        if (ch != ' ' & ch != '\n')
        {
            buffer[reading] = ch;
            reading++;
        }
        else
        {
            reading = 0;
            counter++;
            if (counter == 3)
            {
                sscanf(buffer, "%d", &rows);
                return rows;
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
}

FILE *readFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("File couldn't be opened");
        exit(EXIT_FAILURE);
    }
    return fp;
}

int *getPGMData(char *fileName){
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    char ch;
    int counter = 0;
    int *imgArrPtr;
    imgArrPtr = (int *)malloc(getPGMColumns(fileName) * getPGMRows(fileName) * sizeof(int));
    while ((ch = fgetc(fp)) != EOF)
    {
        char buffer[4];
        if (ch != ' ' & ch != '\n')
        {
            buffer[reading] = ch;
            reading++;
        }
        else
        {
            reading = 0;
            counter++;
            if (counter > 4)
            {
                if (isdigit(buffer[0]))
                    sscanf(buffer, "%d", imgArrPtr + counter - 5);
                else
                {
                    counter--;
                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    return imgArrPtr;
}