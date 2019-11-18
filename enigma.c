#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct Wheel {
    int  letters;
    int* wires;
};

struct Cartridge {
    int    w;
    struct Wheel* wheels;
};

struct Enigma_key {
    int* wheel_positions;
    struct Cartridge c;
    //plugboard
};

int print_cartridge(struct Cartridge c) {
    int w=c.w;
    int letters=c.wheels[0].letters;
    printf("  ");
    for (int wheel=0; wheel<w; wheel++) {
        printf("  W%d", wheel);
    }
    for (int letter=0; letter<letters; letter++) {
        printf("\n%2d: ", letter);
        for (int wheel=0; wheel<w; wheel++) {
            printf("%2d  ", c.wheels[wheel].wires[letter]);
        }
    }
    printf("\n");
}

int print_key(struct Enigma_key key) {
    int* wheel_positions=key.wheel_positions;
    int  w=key.c.w;
    printf("  ");
    for (int wheel=0; wheel<w; wheel++) {
        printf("   %d", wheel_positions[wheel]);
    }
    printf("\n");
    print_cartridge(key.c);
    printf("\n");
}

/*
int enigma_readkey(String filename) {

}
*/

struct Enigma_key enigma_makekey(int wheels, int letters) {
    struct Cartridge c;
    struct Enigma_key key;
    int* wires;
    int w1, w2, v1, v2;
    srand(time(NULL));
    //init wheels
    c.w     =wheels;
    c.wheels=malloc(wheels*sizeof *c.wheels);
    for (int i=0; i<wheels; i++) {
        c.wheels[i].letters=letters;
        c.wheels[i].wires=malloc(letters*sizeof*c.wheels[i].wires);
        wires=c.wheels[i].wires;
        //make a legal wiring, note that wires[wires[j]]=j
        for(int j=0; j<letters; j++) {
            wires[j]=j+1-2*(j%2);
        }
        //randomize the wiring with legal operations
        for(int k=0; k<letters*letters; k++) {
            w1=rand()%letters;
            w2=rand()%letters;
            //cross the wires
            if (w1!=w2 && wires[w1]!=w2) {
                v2       =wires[w2];
                v1       =wires[w1];
                wires[w2]=v1; wires[v2]=w1;
                wires[w1]=v2; wires[v1]=w2;
            }
            else {
                k-=1;
            }
        }
    }
    print_cartridge(c);

    //make key, make random init?
    int* zeros=malloc(wheels*sizeof*zeros);
    for (int i=0; i<wheels; i++) {
        zeros[i]=0;
    }
    key.c=c;
    key.wheel_positions=zeros;

    return key;
}


/*
int enigma_writekey() {

}
*/

//encrypt a single letter
int encrypt(int letter, struct Enigma_key key, int carry) {
    //detach wheels
    struct Wheel* wheels=key.c.wheels;
    struct Wheel  wheel;
    int*   wheel_positions=key.wheel_positions;
    int    letters=wheels[0].letters, w=key.c.w;
    //check if letter is encryptable

    //encrypt the letter
    for (int wi=0; wi<w; wi++) {
        wheel=wheels[wi];
        letter=wheel.wires[(letter+wheel_positions[wi])%letters];
        //printf("%d->", letter);
    }
    for (int wi=w-2; wi>=0; wi--) {
        wheel=wheels[wi];
        letter=wheel.wires[(letter+wheel_positions[wi])%letters];
        //printf("%d->", letter);
    }
    //update the wheel positions in key
    for (int i=0; carry>0&&i<w; i++) {
        wheel_positions[i]=(wheel_positions[i]+carry)%letters;
        carry=(wheel_positions[i]==0);
    }
    return letter;
}

//encrypts in place
int encrypt_word(int* word, int length, struct Enigma_key key) {
    for(int i=0; i<length; i++) {
        //print_key(key);
        printf("E: %d->",word[i]);
        word[i]=encrypt(word[i], key, 1);
        printf("  %d\n",word[i]);
    }
}





int main() {
    int message[6*4]={0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3,4,5};
    struct Enigma_key key=enigma_makekey(6, 6);
    printf("made key\n");
    encrypt_word(message, 6*4, key);
}
