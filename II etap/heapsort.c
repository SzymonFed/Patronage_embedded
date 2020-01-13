#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define IPARENT(i) (int)((i-1) / 2)
#define PARENT(index,base) base - ((index - IPARENT(index))*size)

typedef int (*cmp_t)(const void *a, const void *b);

void swap(void* a, void* b,size_t size)
{
    void *temp = malloc(size);
    memcpy(temp, a,size); 
    memcpy(a, b,size); 
    memcpy(b, temp,size); 
    free(temp); 
}


void heapify(void *base,size_t nmemb,size_t size,cmp_t cmp)
{
    int index = 0;
    while(index<nmemb)
    {
        if (cmp(base, PARENT(index,base)) > 0 )
        {
            swap (base,PARENT(index,base),size);
            base = PARENT(index,base);
            index = IPARENT(index);
        }
        else
        {
            index++;
            base+=size;
        }
    }
}

void heapsort(void *base, size_t nmemb, size_t size, cmp_t cmp)
{
    while(nmemb>0)
    {
        heapify(base,nmemb,size,cmp);
        nmemb-=1;
        swap(base,base+nmemb*size,size);
    }
}


int cmp_int(const void *a, const void *b)
{
    if (*(int *)a < *(int *)b)
        return -1;

    else if (*(int *)a > *(int *)b)
        return 1;

    else
        return 0;
}

int cmp_str(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(void)
{
    int A[] = {5, 8, 2, 11, 3, 7, 9, 12};
    char *B[] = { "Jacek", "Maria", "Szymon", "Zosia",
        "Adam", "Gosia", "Bonawentura", "Kunegunda" };

    int i;


    heapsort(A, 8, sizeof(*A), cmp_int);
    heapsort(B, 8, sizeof(*B), cmp_str);

    for (i = 0; i < 8; i++)
        printf("%d%c", A[i], (i == 7) ? '\n' : ' ');

    for (i = 0; i < 8; i++)
        printf("%s%c", B[i], (i == 7) ? '\n' : ' ');

    return 0;
}
