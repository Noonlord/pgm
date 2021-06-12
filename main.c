#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

int getPGMColumns(char *fileName);
int getPGMRows(char *fileName);
int getPGMGrayValue(char *fileName);
int getElementFromFile(char *fileName, int pos);
int *getPGMData(char *fileName);
FILE *readFile(char *fileName);
FILE *writeFile(char *fileName);

int main()
{
    FILE *fp, *outFp;
    char ch, fileName[25], outName[25];
    printf("Enter filename: ");
    gets(fileName);

    fp = readFile(fileName);

    int rows = getPGMRows(fileName);
    int columns = getPGMColumns(fileName);
    int *imgArrPtr = getPGMData(fileName);
    int maxGray = getPGMGrayValue(fileName);
    int length = rows * columns;
    printf("Enter output compressed filename: ");
    gets(outName);
    outFp = writeFile(outName);
    fprintf(outFp, "%d\n%d\n%d\n", rows, columns, maxGray);
    int i;
    for (i = 0; i < length; i++)
    {
        int count = 1;
        while (i < length - 1 && *(imgArrPtr + i) == *(imgArrPtr + i + 1))
        {
            count++;
            i++;
        }
        fprintf(outFp, "%d %d ", count, *(imgArrPtr + i));
    }
    return 0;
}
int getElementFromFile(char *fileName, int pos)
{
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    int counter = 0;
    pos++;
    int data;
    char ch;
    while ((ch = fgetc(fp)) != EOF && counter < pos)
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
            if (counter == pos)
            {
                sscanf(buffer, "%d", &data);
                fclose(fp);
                return data;
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    return -1;
}

int getPGMColumns(char *fileName)
{
    return getElementFromFile(fileName, 1);
}

int getPGMRows(char *fileName)
{
    return getElementFromFile(fileName, 2);
}

int getPGMGrayValue(char *fileName)
{
    return getElementFromFile(fileName, 3);
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
FILE *writeFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
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
    fclose(fp);
    return imgArrPtr;
}