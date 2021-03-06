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
int *getCMPData(char *fileName);
void compress(char *inFile, char *outFile);
void decompress(char *inFile, char *outFile);
void changeColor(char *inFile, int colorToChange, int targetColor);
void calculateHistogram(char *inFile);
FILE *readFile(char *fileName);
FILE *writeFile(char *fileName);

int main()
{
    char fileName[25];
    int select = -1;
    while(select != 5){
        printf("\n1)File compression\n2)File decompression\n3)Change all instances of a color\n4)Print Histogram\n5)Exit\n>:  ");
        scanf("%d", &select);
        switch(select){
            case 1:
                printf("Enter filename to compress: ");
                scanf("%s", fileName);
                compress(fileName, "test_encoded.txt");
                memset(fileName, 0, sizeof(fileName));
                break;
            case 2:
                printf("Enter filename to decompress: ");
                scanf("%s", fileName);
                decompress(fileName, "test_decoded.pgm");
                memset(fileName, 0, sizeof(fileName));
                break;
            case 3:
                printf("Enter compressed filename to change color: ");
                scanf("%s", fileName);
                int colorToChange, targetColor;
                printf("Enter which color you want to change: ");
                scanf("%d", &colorToChange);
                printf("Enter target color: ");
                scanf("%d", &targetColor);
                changeColor(fileName, colorToChange, targetColor);
                memset(fileName, 0, sizeof(fileName));
                break;
            case 4:
                printf("Enter compressed filename to print histogram: ");
                scanf("%s", fileName);
                calculateHistogram(fileName);
                memset(fileName, 0, sizeof(fileName));
                break;
            case 5:
                break;
        }
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
        char buffer[8];
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
        char buffer[8];
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
int *getCMPData(char *fileName){

    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    char ch;
    int counter = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        char buffer[8];
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
    int rleLength = counter - 2;
    int *rleArrayPtr = (int *)malloc(rleLength * sizeof(int));
    *(rleArrayPtr) = rleLength - 1;
    counter = 0; reading = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        char buffer[8];
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
                    sscanf(buffer, "%d", rleArrayPtr + counter - 3);

                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    return rleArrayPtr;
}

int *getPGMDataFromCMP(char *fileName){
    FILE *fp;
    fp = readFile(fileName);
    int reading = 0;
    char ch;
    int counter = 0;
    int pixel = 0;
    int pixelCount = 0;
    int lastColor = -1;
    int size = getElementFromFile(fileName, 0) * getElementFromFile(fileName, 1);
    int *imgArrPtr = (int *)malloc(size * sizeof(int));
    while ((ch = fgetc(fp)) != EOF)
    {
        char buffer[8];
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
                else {
                    if (!(counter%2)){
                        int tmp;
                        sscanf(buffer, "%d", &tmp);
                        pixelCount += tmp;
                    }
                    else{
                        int tmp;
                        sscanf(buffer, "%d", &tmp);
                        if (tmp<0 || tmp>255){
                            printf("Invalid colors!!\n");
                            exit(EXIT_FAILURE);
                        }
                        if (lastColor == tmp){
                            printf("Subsequent same colors!! \n");
                            exit(EXIT_FAILURE);
                        }
                        else{
                            lastColor = tmp;
                        }
                    }
                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    printf("No subsequent colors found!\n");
    printf("All colors valid!\n");
    if (pixelCount != size){
        printf("Pixel count doesn't match up with Rows x Columns!\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Pixel count matches!\n");
    }
    rewind(fp);
    int rleLength = counter - 3;
    int *rleArrayPtr = (int *)malloc(rleLength * sizeof(int));
    counter = 0; reading = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        char buffer[8];
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

void changeColor(char *inFile, int colorToChange, int targetColor){
    int *data = getCMPData(inFile);
    int i;
    FILE *fp = readFile(inFile);
    int rows = getElementFromFile(inFile, 0);
    int columns = getElementFromFile(inFile, 1);
    int gray = getElementFromFile(inFile, 2);
    fclose(fp);
    for(i = 1; i < (*(data) + 1); i++){
        if(!(i%2)){
            if (*(data + i) == colorToChange){
                *(data + i) = targetColor;
            } 
        }
    }
    fp = writeFile(inFile);
    fprintf(fp, "%d\n%d\n%d\n", rows, columns, gray);
    for(i = 1; i < (*(data) + 1); i++){
        fprintf(fp, "%d ", *(data + i));
    }
    fclose(fp);
}

void calculateHistogram(char *inFile){
    int *data = getCMPData(inFile);
    int i;
    int count[256] = {0};
    for(i = 2; i < (*(data) + 1); i+=2){
        count[*(data + i)] += *(data + i -1);
        }
    for(i=0; i < 256; i++){
        if(count[i] > 0){
            printf("%d times %d value\n",count[i], i);
        }
    }
}