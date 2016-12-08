#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#define MAXN 30

int Job[MAXN][MAXN] = {0};
int Cost[MAXN][MAXN] = {0};
int isAssign[MAXN] = {0};
int beAssigned[MAXN] = {0};
int bound = INT_MAX;
int N, M;

int myReduce(int X[N], int Y[N]){
	int reduce = 0, minX[N], minY[N];
	for(int i=0; i<N; i++) minX[i] = minY[i] = INT_MAX;
	for(int i=0; i<N; i++){
		if(isAssign[i]) continue;
		for(int j=0; j<N; j++){
			if(beAssigned[j]) continue;
			int tmp = Cost[i][j] - X[i] - Y[j];
			if(tmp >= 0 && tmp < minX[i]) minX[i] = tmp;
		}
	}
	for(int i=0; i<N; i++){
		if(minX[i] >= INT_MAX) continue;
		reduce += minX[i];
		X[i] += minX[i];
	}

	for(int i=0; i<N; i++){
		if(isAssign[i]) continue;
		for(int j=0; j<N; j++){
			if(beAssigned[j]) continue;
			int tmp = Cost[i][j] - X[i] - Y[j];
			if(tmp >= 0 && tmp < minY[j]) minY[j] = tmp;
		}
	}
	for(int i=0; i<N; i++){
		if(minY[i] >= INT_MAX) continue;
		reduce += minY[i];
		Y[i] += minY[i];
	}
	return reduce;
}

int compare(const void *a, const void *b){
	return *(int *)a - *(int *)b;
}

void Assign(int index, int inDegree[N], int X[N], int Y[N], int lb){
	if(index >= N){
		if(lb < bound) bound = lb;
		return;
	}
	if(lb >= bound) return;
	
	int lbSort[N][2], num = 0;
	int tmpDegree[N][N], tmpX[N][N], tmpY[N][N];
	for(int i=0; i<N; i++){
		if(beAssigned[i] || inDegree[i] > 0) continue;
		memcpy(tmpDegree[i], inDegree, sizeof(tmpDegree[i]));
		memcpy(tmpX[i], X, sizeof(tmpX[i]));
		memcpy(tmpY[i], Y, sizeof(tmpY[i]));

		int reduce = lb + Cost[index][i] - X[index] - Y[i];
		isAssign[index] = beAssigned[i] = 1;
		reduce += myReduce(tmpX[i], tmpY[i]);
		for(int j=0; j<N; j++)
			if(Job[i][j] && inDegree[j] > 0) tmpDegree[i][j]--;

		isAssign[index] = beAssigned[i] = 0;
		lbSort[num][0] = reduce;
		lbSort[num][1] = i;
		num++;
	}

	qsort(lbSort, num, sizeof(lbSort[0]), compare);
	for(int i=0; i<num; i++){
		int which = lbSort[i][1];
		isAssign[index] = beAssigned[which] = 1;
		Assign(index+1, tmpDegree[which], tmpX[which], tmpY[which], lbSort[i][0]);
		isAssign[index] = beAssigned[which] = 0;
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
	int X[N], Y[N];
	memset(X, 0, sizeof(X));
	memset(Y, 0, sizeof(Y));
	int reduce = myReduce(X, Y);

	Assign(0, inDegree, X, Y, reduce);
	printf("%d\n", bound);
	return 0;
}
