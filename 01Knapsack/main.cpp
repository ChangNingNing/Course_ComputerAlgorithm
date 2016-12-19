#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAXN 1024

typedef struct {
	int v, w;
} Object;

int W, N;
int bound = INT_MIN;
Object obj[MAXN];

int compare(const void *a, const void *b){
	Object *A = (Object *)a, *B = (Object *)b;
	float cpA = (float)A->v/A->w, cpB = (float)B->v/B->w;
	if(cpA > cpB) return -1;
	else if(cpA < cpB) return 1;
	
	if(A->w > B->w) return -1;
	return 1;
}

int UGreedy(int index, int w){
	float ub = 0;
	for(int i=index; i<N && w < W; i++){
		float tmp = W - w;
		float k = tmp / obj[i].w;
		k = (k <= 1)? k: 1;
		w += k * obj[i].w;
		ub += k * obj[i].v;
	}
	return (int)ub;
}

int LGreedy(int index, int w){
	int lb = 0;
	for(int i=index; i<N && w < W; i++){
		int tmp = W - w;
		if(tmp >= obj[i].w){
			w += obj[i].w;
			lb += obj[i].v;
		}
	}
	return lb;
}

int a = 0;
void DFS(int index, int w, int v){
	int ub = v + UGreedy(index, w);
	
	if(a % 100000000 == 0){
		a = 0;
		if(clock() > CLOCKS_PER_SEC*1){
			bound = ub;
			return;
		}
	}
	
	if(w > W) return;
	if(ub <= bound) return;
	if(index >= N){
		if(v > bound) bound = v;
		return;
	}
a++;
	DFS(index + 1, w, v);
	DFS(index + 1, w + obj[index].w, v + obj[index].v);
	return;
}

int main(){
	scanf("%d %d", &W, &N);
	for(int i=0; i<N; i++){
		int w, v;
		scanf("%d %d", &v, &w);
		obj[i].v = v;
		obj[i].w = w;
	}
	qsort(obj, N, sizeof(obj[0]), compare);
	bound = LGreedy(0, 0);
	DFS(0, 0, 0);
	printf("%d\n", bound);
	return 0;
}
