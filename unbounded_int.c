#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "unbounded_int.h"

char *long2char(long long b){
    char *c = malloc(sizeof(char));
    assert(c != NULL);
    
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
    size_t length = sizeof(b)/sizeof(long long);
    if((a.signe == '-' && b >= 0) || (a.len < length)){
        return -1;
    }
    if((a.signe == '+' && b < 0) || (a.len > length)){
        return 1;
    }
    long long tmp = b/pow(10,length-1);
    while(a.premier != NULL){
        if(a.premier->c-'0' < tmp){
            return -1;
        }
        if(a.premier->c-'0' > tmp){
            return 1;
        }
        a.premier = a.premier->suivant;
        tmp = tmp/pow(10,length-1);
    }
    return 0;
}
