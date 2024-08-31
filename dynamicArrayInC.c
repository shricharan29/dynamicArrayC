#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int sizeDouble = 64;
int entriesDouble = 0;
double *arrayDouble;

int sizeString = 128;
int entriesString = 0;
char *arrayString;

int sizeElements = 32;
int entriesElements = 0;
char *arrayElements;

void init();
int isNumber(char *);
void get(int);
void print();

void addDouble(double);
// void scaleUpDouble();
void scaleDouble(double);
double popDouble();

void addString(char *);
// void scaleUpString();
void scaleString(double);
char *popString();

void addElement(char);
void scaleElements(double);
char popElement();

int main()
{
    init();

    char input[255];

    while (1)
    {
        printf("\n\033[1;34mDynamic Array>\033[0m ");
        fgets(input, 255, stdin);

        input[strlen(input) - 1] = '\0';

        // printf("%s\n", input);

        if (!strcmp(input, "print"))
            print();
        else if (!strcmp(input, "pop"))
        {
            if (*(arrayElements + entriesElements - 1) == 'D')
                printf("Poped %lf\n", popDouble());
            else if (*(arrayElements + entriesElements - 1) == 'S')
                printf("Poped %s\n", popString());
        }
        else if (input[0] == 'G')
        {
            // printf("%d\n", atoi(&input[1]));
            int index = atoi(&input[1]);
            get(index);
        }
        // else if(isdigit(*input)){
        else if (isNumber(input))
        {
            double newElement;
            sscanf(input, "%lf", &newElement);
            addDouble(newElement);
            printf("Added : %lf\n", newElement);
        }
        else
        {
            addString(input);
            printf("Added : %s\n", input);
        }
    }
}

void init()
{
    arrayDouble = malloc(sizeDouble);
    arrayString = malloc(sizeString);
    arrayElements = malloc(sizeElements);
}

int isNumber(char *input)
{
    char *pInput;
    char *numbers = ".1234567890";
    int noChar = 0;
    int noPeriod = 0;

    if (*input == '-')
        input++;

    pInput = input;

    while (*pInput)
    {
        int i;
        if (*pInput == '.')
            noPeriod++;
        // if(*pInput == '-') return 0;
        for (i = 0; i < strlen(numbers); i++)
        {
            if (*pInput == *(numbers + i))
                noChar++;
            // printf("%d %d\n", noChar, noPeriod);
        }
        pInput++;
    }
    // printf("%d\n", strlen(input) == noChar && noPeriod <= 1);
    return strlen(input) == noChar && noPeriod < 1;
}

void get(int index)
{
    if (index >= entriesElements || index < 0)
    {
        printf("Array index out of bound.\n");
    }
    else
    {
        int strings = 0, doubles = 0, i = 0;
        while (i < index)
        {
            if (*(arrayElements + i) == 'S')
                strings++;
            else if (*(arrayElements + i) == 'D')
                doubles++;
            i++;
        }
        // printf("Doubles: %d Strings : %d\n", doubles, strings);

        if (*(arrayElements + index) == 'D')
            printf("%.2lf\n", *(arrayDouble + doubles));
        else if (*(arrayElements + index) == 'S')
        {
            int stringCounter = 0;
            char *charPointer = arrayString;
            while (stringCounter < strings)
            {
                if (!(*charPointer++))
                    stringCounter++;
                // printf("%s\n", charPointer);
            }
            printf("%s\n", charPointer);
        }
    }
}

void print()
{
    if (entriesElements == 0)
    {
        printf("Empty Set []\n");
        return;
    }

    char *element;
    char stringCounter = 0;
    int doubleCounter = 0;

    printf("[");
    for (element = arrayElements; element < arrayElements + entriesElements; element++)
    {
        if (*element == 'S')
        {
            while (*(arrayString + stringCounter))
            {
                printf("%c", *(arrayString + stringCounter));
                stringCounter++;
            }
            stringCounter++;
            // element == arrayElements + entriesElements - 1 ? printf("]\n") : printf(", ");
        }
        else if (*element == 'D')
        {
            printf("%lf", *(arrayDouble + doubleCounter));
            doubleCounter++;
            // element == arrayElements + entriesElements - 1 ? printf("]\n") : printf(", ");
        }
        // element == arrayElements + entriesElements - 1 ? printf("]\n") : printf(", ");
        printf(", ");
    }
    printf("\b\b]\n");
}

// void addDouble(char* input){
//     if(entriesDouble >= (int)((sizeDouble / sizeof(double)) * 0.75)) {
// 	    printf("Need to scale up\n");
// 	    scaleUpDouble();
//     }

//     sscanf(input, "%lf", arrayDouble + entriesDouble);
//     entriesDouble++;
// }

void addDouble(double element)
{
    if (entriesDouble >= (int)((sizeDouble / sizeof(double)) * 0.75))
    {
        printf("Need to scale up\n");
        scaleDouble(2);
    }

    *(arrayDouble + entriesDouble) = element;
    entriesDouble++;
    addElement('D');
}

double popDouble()
{
    if (entriesDouble <= 0)
        return -1;

    if (entriesDouble <= (int)(sizeDouble / sizeof(double)) * 0.5)
    {
        printf("Need to scale down\n");
        scaleDouble(0.65);
    }

    entriesDouble--;
    entriesElements--;
    return *(arrayDouble + entriesDouble);
}

// void scaleUpDouble(){
//     sizeDouble *= 2;

//     double* newArray = malloc(sizeDouble);

//     int i;
//     for(i = 0; i < entriesDouble; i++){
//     	*(newArray + i) = *(arrayDouble + i);
//     }

//     free(arrayDouble);
//     arrayDouble = newArray;
// }

void scaleDouble(double factor)
{
    sizeDouble = (int)(sizeDouble * factor);

    double *newArray = malloc(sizeDouble);

    int i;
    for (i = 0; i < entriesDouble; i++)
    {
        *(newArray + i) = *(arrayDouble + i);
    }

    free(arrayDouble);
    arrayDouble = newArray;
}

void addString(char *input)
{
    if (entriesString >= (int)(sizeString * 0.75) || strlen(input) >= (sizeString - entriesString + 1))
    {
        scaleString(2);
        printf("String scaled UP\n");

        addString(input);
    }
    else
    {
        for (; *input; input++)
        {
            *(arrayString + entriesString) = *input;
            entriesString++;
        }
        *(arrayString + entriesString) = '\0';

        entriesString++;
        addElement('S');
    }
}

// void scaleUpString(){
//     sizeString *= 2;
//     char* newArray = malloc(sizeString);

//     int i;
//     for(i = 0; i < entriesString; i++){
//         *(newArray + i) = *(arrayString + i);
//     }

//     free(arrayString);
//     arrayString = newArray;
// }

void scaleString(double factor)
{
    sizeString = (int)(sizeString * factor);
    char *newArray = malloc(sizeString);

    int i;
    for (i = 0; i < entriesString; i++)
    {
        *(newArray + i) = *(arrayString + i);
    }

    free(arrayString);
    arrayString = newArray;
}

char *popString()
{
    if (entriesString <= 0)
        return NULL;

    if (entriesString < (int)(sizeString * 0.5))
    {
        scaleString(0.65);
        printf("String scaled DOWN\n");
    }

    entriesString -= 2;
    while (entriesString > 0 && *(arrayString + entriesString))
        entriesString--;

    if (entriesString != 0)
        entriesString++;

    entriesElements--;
    return arrayString + entriesString;
}

void addElement(char type)
{
    if (entriesElements >= (int)(sizeElements * 0.75))
    {
        printf("Need to scale up array recorder.");
        scaleElements(2);
    }

    *(arrayElements + entriesElements) = type;
    entriesElements++;
}

void scaleElements(double factor)
{
    sizeElements = (int)(sizeElements * factor);
    char *newArray = malloc(sizeElements);

    int i;
    for (i = 0; i < entriesElements; i++)
    {
        *(newArray + i) = *(arrayElements + i);
    }

    free(arrayElements);
    arrayElements = newArray;
}

char popElement()
{
    if (entriesElements <= 0)
    {
        printf("There are not items to pop.");
        return '\0';
    }
    else if (entriesElements <= (int)(sizeElements * 0.5))
    {
        scaleElements(0.65);
        return popElement();
    }
    else
        return entriesElements--;
}