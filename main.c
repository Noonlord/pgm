#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

int getPGMColumns(char *fileName);
int getPGMRows(char *fileName);
int getPGMGrayValue(char *fileName);
int getElementFromFile(char *fileName, int pos);
int *getPGMData(char *fileName);
int *getPGMDataFromCMP(char *fileName);
void compress(char *inFile, char *outFile);
void decompress(char *inFile, char *outFile);
FILE *readFile(char *fileName);
FILE *writeFile(char *fileName);

int main()
{
    char ch, fileName[25], outName[25], uncompName[25];
    printf("Enter filename: ");
    gets(fileName);
    printf("Enter output compressed filename: ");
    gets(outName);
    compress(fileName, outName);
    printf("Tamamlandi. \nDecompress: ");
    gets(uncompName);
    decompress(uncompName, "test_decoded.pgm");
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

int *getPGMDataFromCMP(char *fileName){
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    char ch;
    int counter = 0;
    int pixel = 0;
    int *imgArrPtr = (int *)malloc(getElementFromFile(fileName, 0) * getElementFromFile(fileName, 1) * sizeof(int));
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
            if (counter > 3)
            {
                if (!isdigit(buffer[0])){
                    counter--;
                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    rewind(fp);
    int rleLength = counter - 3;
    int *rleArrayPtr = (int *)malloc(rleLength * sizeof(int));
    counter = 0; reading = 0;
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
            if (counter > 3)
            {
                if (!isdigit(buffer[0])){
                    counter--;
                }
                else{
                    sscanf(buffer, "%d", rleArrayPtr + counter - 4);
                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    int i, j;
    for (i = 0; i < rleLength; i = i + 2)
    {
        for(j = 0; j < *(rleArrayPtr + i); j++){
            *(imgArrPtr + pixel) = *(rleArrayPtr + i + 1);
            pixel++;
        }
    }
    fclose(fp);
    return imgArrPtr;
}

void decompress(char *inFile, char *outFile){
    int rows = getElementFromFile(inFile, 0);
    int columns = getElementFromFile(inFile, 1);
    int gray = getElementFromFile(inFile, 2);
    int *data = getPGMDataFromCMP(inFile);

    FILE *fp = writeFile(outFile);
    fprintf(fp, "P2 %d %d %d\n", columns, rows, gray);
    int i;
    for (i = 0; i < rows * columns; i++)
    {
        fprintf(fp, "%d ", *(data + i));
    }
    fclose(fp);
}

void compress(char *inFile, char *outFile){
    int rows = getPGMRows(inFile);
    int columns = getPGMColumns(inFile);
    int *imgArrPtr = getPGMData(inFile);
    int maxGray = getPGMGrayValue(inFile);
    int length = rows * columns;
    FILE *fp = writeFile(outFile);
    fprintf(fp, "%d\n%d\n%d\n", rows, columns, maxGray);
    int i;
    for (i = 0; i < length; i++)
    {
        int count = 1;
        while (i < length - 1 && *(imgArrPtr + i) == *(imgArrPtr + i + 1))
        {
            count++;
            i++;
        }
        fprintf(fp, "%d %d ", count, *(imgArrPtr + i));
    }
    fclose(fp);
}
