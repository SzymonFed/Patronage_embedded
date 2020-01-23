#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

uint64_t l_shift(uint64_t a, uint64_t *c, int i)
{
    *c = 0;
    while (i>0)
    {
        *c<<=1;
        if(a&0x8000000000000000UL)
            *c+=0x1UL;
        
        a<<=1;
        i--;
    }
    
    return a;
}

uint64_t sum(uint64_t a, uint64_t b, uint64_t *c)
{
    
    uint64_t result = a;
    *c = 0;
    while(b)
    {
        result = a^b;
        b = a&b;
        a = result;
        if(b)
        {
            *c <<=1;
            if(b&0x8000000000000000UL)
                *c+=1UL;
        }
        b<<=1;
    }
    printf("Result: %lu Rest: %lu\n",result,*c);
    return result;
}


void mul64(uint64_t *p, uint64_t a, uint64_t b)
{
    int count = 0;
    uint64_t shift_rest, sum_rest;
    shift_rest = 0;
    sum_rest = 0;
    p[0] = 0, p[1] = 0;
    while(b)
    {
        if (b%2 == 1)
        {
            uint64_t temp = l_shift(a,&shift_rest,count);
            p[0] = sum(p[0],temp,&sum_rest);
            p[1] +=(sum_rest + shift_rest);    
        }
        b/=2;
          
        count++;
    }
    
}   

void print_big(char *str, uint64_t *p, size_t n)
{
    uint64_t *p32 = malloc(n * 2 * sizeof(*p32)), m;
    char *tmp = malloc(20 * n), *ptr = tmp;
    ssize_t i, j;

    for (i = 0; i < n; i++) {
        p32[2*i] = p[i] & 0xffffffffUL;
        p32[2*i+1] = p[i] >> 32;
    }

    for (i = n*2-1; i >= 0; i--) {
        while (p32[i]) {
            m = 0UL;

            for (j = i; j >= 0; j--) {
                p32[j] += m << 32;
                m = p32[j] % 10;
                p32[j] /= 10;
            }

            *ptr++ = (char)m + '0';
        }
    }

    if (ptr == tmp)
        *str++ = '0';

    else
        while (ptr > tmp)
            *str++ = *--ptr;

    *str = '\0';
    free(p32);
    free(tmp);
}

int main(int argc, char *argv[])
{
    uint64_t a, b, p[2];
    char str[40], *end;

    if (argc != 3)
        return 1;

    a = strtoul(argv[1], &end, 0);

    if (*end)
        return 1;

    b = strtoul(argv[2], &end, 0);
    
    if (*end)
        return 1;

    mul64(p, a, b);
    print_big(str, p, 2);
    printf("0x%016lx * 0x%016lx = 0x%016lx%016lx\n",
            a, b, p[1], p[0]);
    printf("%lu * %lu = %s\n", a, b, str);
    return 0;
}
