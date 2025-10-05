
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

#define ROWS 14
#define COLS 12
#define MAX_WORDS 21
#define MAXLEN 20

char grid[ROWS][COLS + 1] = {
    "MACPIUPSMNRD",
    "EENNEEHOHMEI",
    "LESCALMRAALT",
    "LFGCHEIRRLAN",
    "EIHNNOLCIEHT",
    "NNKKAIRGALSA",
    "SDGGNGMLONIO",
    "KIUENAKULLFL",
    "RNRDEBTNHMNB",
    "AGGNYRODACWR",
    "HLLDENTISTOU",
    "SGEBUBBLESLC",
    "BARRACUDAUCE",
    "CORALREEFOKR"};

char words[MAX_WORDS][MAXLEN] = {
    "FINDING", "NEMO", "ELLEN", "MARLIN", "CLOWNFISH",
    "CORALREEF", "PELICAN", "BARRACUDA", "DORY", "SHARKS",
    "BRUCE", "ANCHOR", "CHUM", "DENTIST", "TANKGANG",
    "GILL", "BLOAT", "BUBBLES", "PEACH", "GURGLE", "DEB"};

int dx[8] = {-1,-1,-1,0,0,1,1,1};
int dy[8] = {-1,0,1,-1,1,-1,0,1};
char *dirNames[8] = {"NW","N","NE","W","E","SW","S","SE"};

typedef struct {
    bool found;
    int row, col, dir;
} WordLoc;

WordLoc result[MAX_WORDS];

bool checkWord(char *word, int r, int c, int d) {
    int len = strlen(word);
    for(int i=0;i<len;i++) {
        int nr = r + i*dx[d];
        int nc = c + i*dy[d];
        if(nr<0 || nr>=ROWS || nc<0 || nc>=COLS) return false;
        if(grid[nr][nc] != word[i]) return false;
    }
    return true;
}

int main() {
    double t1,t2;
    int i,r,c,d;

    // Sequential
    for(i=0;i<MAX_WORDS;i++) result[i].found=false;
    t1=omp_get_wtime();
    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            for(d=0;d<8;d++){
                for(i=0;i<MAX_WORDS;i++){
                    if(result[i].found) continue;
                    if(checkWord(words[i],r,c,d)){
                        result[i].found=true;
                        result[i].row=r; result[i].col=c; result[i].dir=d;
                    }
                }
            }
        }
    }
    t2=omp_get_wtime();
    printf("=== Sequential ===\nTime: %f s\n\n", t2-t1);

    // Parallel
    for(i=0;i<MAX_WORDS;i++) result[i].found=false;
    t1=omp_get_wtime();
    #pragma omp parallel for private(r,c,d,i) schedule(dynamic)
    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            for(d=0;d<8;d++){
                for(i=0;i<MAX_WORDS;i++){
                    if(checkWord(words[i],r,c,d)){
                        #pragma omp critical
                        if(!result[i].found){
                            result[i].found=true;
                            result[i].row=r; result[i].col=c; result[i].dir=d;
                        }
                    }
                }
            }
        }
    }
    t2=omp_get_wtime();
    printf("=== Parallel ===\nTime: %f s\n\n", t2-t1);

    // Results
    printf("=== Results ===\n");
    for(i=0;i<MAX_WORDS;i++){
        if(result[i].found){
            printf("%s FOUND at (%d,%d) dir %s\n",words[i],
                   result[i].row,result[i].col,dirNames[result[i].dir]);
        } else {
            printf("%s NOT FOUND\n",words[i]);
        }
    }

    return 0;
}