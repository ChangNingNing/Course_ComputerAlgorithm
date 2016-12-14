#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1001
#define MAXW 5000001

int M, N;
int C[MAXW] = {0}, obj[MAXN][2], accuW[MAXN];

int MAX(int x, int y){ return (x > y)? x: y; }
int MIN(int x, int y){ return (x < y)? x: y; }

int comByW(const void *a, const void *b){
	return *((int *)a+1) - *((int *)b+1);
}

int main(){
	scanf("%d %d", &M, &N);
	for(int i=1; i<=N; i++)
		scanf("%d %d", &obj[i][0], &obj[i][1]);

	qsort(obj[1], N, sizeof(obj[1]), comByW);
	accuW[0] = 0;
	for(int i=1; i<=N; i++) accuW[i] = accuW[i-1] + obj[i][1];

	for(int i=1; i<=N; i++){
		int min = MIN(M, accuW[i]);
		for(int j=min; j>=0; j--){
			int min2 = MIN(j, accuW[i-1]);
			int tmp = j - obj[i][1];
			C[j] = (tmp < 0)? C[min2]: MAX(C[min2], C[tmp]+obj[i][0]);
		}
	}
	
	printf("%d\n", C[MIN(M, accuW[N])]);
	return 0;
}
