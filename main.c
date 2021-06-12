#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
int main(){
    printf("Program starting\n");
    FILE *fp;
    char ch, fileName[25];
    printf("Enter filename: ");
    gets(fileName);

    fp = fopen(fileName, "r");

    if(fp == NULL){
        printf("File couldn't be opened");
        exit(EXIT_FAILURE);
    }

    printf("Reading...\n");
    int reading = 0;
    int counter = 0;
    int rows = 0;
    int columns = 0;
    int * imgArrPtr;
    while((ch=fgetc(fp)) != EOF){
        char buffer[4];
        if(ch != ' ' & ch != '\n'){
            buffer[reading] = ch;
            reading++;
        }
        else {
            printf("%s\n", buffer);
            reading = 0;
            counter++;
            if (counter == 1){
                if(!strcmp(buffer, "P2")){
                    printf("File is ASCII format");
                }
            }
            else if (counter == 2)
                sscanf(buffer, "%d", &columns);
            else if (counter == 3){
                sscanf(buffer, "%d", &rows);
                imgArrPtr = (int*) malloc(columns * rows * sizeof(int));
            }
            else if (counter != 4) {
                if(isdigit(buffer[0]))
                    sscanf(buffer, "%d", imgArrPtr + counter - 5);
                else{
                    counter--;
                    printf("asdsd");
                }
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    printf("Rows: %d\n", rows);
    printf("Cols: %d\n", columns);
    printf("66th: %d\n", *(imgArrPtr + 64));
    printf("Last element: %d\n", *(imgArrPtr + columns * rows - 1));
    int length = rows * columns;
    int i;
    for (i = 0; i < length; i++)
    {
        int count = 1;
        while (i < length -1 && *(imgArrPtr + i) == *(imgArrPtr + i + 1))
        {
            count++;
            i++;
        }
        printf("%d %d ", count, *(imgArrPtr + i));
    }
    
    fclose(fp);
    return 0;

}
