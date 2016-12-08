#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#define MAXN 50

int Job[MAXN][MAXN] = {0};
int Cost[MAXN][MAXN] = {0};
int bound = INT_MAX;
int N, M;

void Assign(int index, int inDegree[N], int lb){
	if(index >= N){
		if(lb < bound) bound = lb;
		return;
	}
	if(lb >= bound) return;
	
	int tmpDegree[N];
	for(int i=0; i<N; i++){
		if(inDegree[i] != 0) continue;
		memcpy(tmpDegree, inDegree, sizeof(tmpDegree));
		tmpDegree[i] = -1;
		for(int j=0; j<N; j++)
			if(Job[i][j] && inDegree[j] > 0) tmpDegree[j]--;
		Assign(index+1, tmpDegree, lb+Cost[index][i]);
	}
	return;
}

int main(){
	assert(scanf("%d %d", &N, &M)==2);
	int inDegree[N];
	memset(inDegree, 0, sizeof(inDegree));
	for(int i=0; i<M; i++){
		int a, b;
		assert(scanf("%d %d", &a, &b)==2);
		Job[a-1][b-1] = 1;
		inDegree[b-1]++;
	}
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			assert(scanf("%d", &Cost[i][j]) == 1);
	Assign(0, inDegree, 0);
	printf("%d\n", bound);
	return 0;
}
