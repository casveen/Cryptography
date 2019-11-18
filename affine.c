#include <stdio.h>
#include <stdlib.h>

int affine(int letter, int offset) {
    return (letter+offset)%26;
}

int encode_word(int* word, int n, int (*encoder)(int, int) ) {
    int  key=1;
    int* out= malloc(n*sizeof * word);
    for(int i=0; i<n; i++) {
        out[i]=encoder(word[i], key);
        printf("%d \n", out[i]);
    }
}

int main() {
    int word[6]={1,2,3,4,5,6};
    encode_word(word, 6, affine);
}
