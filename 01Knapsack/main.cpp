#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXN 1024

typedef struct {
	int v, w;
	float uV;
} Object;

int W, N;
int bound = INT_MIN;
Object obj[MAXN];

int compare(const void *a, const void *b){
	Object *A = (Object *)a, *B = (Object *)b;
	if(A->uV > B->uV) return -1;
	else if(A->uV < B->uV) return 1;
	if(A->w > B->w) return -1;
	return 1;
}

float UGreedy(int index, int w){
	float ub = 0;
	for(int i=index; i<N && w < W; i++){
		float tmp = W - w;
		float k = tmp / obj[i].w;
		k = (k <= 1)? k: 1;
		w += k * obj[i].w;
		ub += k * obj[i].v;
	}
	return ub;
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

void DFS(int index, int w, int v, float ub){
	if(w > W) return;
	if(ub <= bound) return;
	if(index >= N){
		if(v > bound) bound = v;
		return;
	}

	int tmp = W - w;
	float ub_tmp[2];
	ub_tmp[0] = ub_tmp[1] = -1;
	if(tmp >= obj[index].w)
		ub_tmp[0] = UGreedy(index + 1, w + obj[index].w);
	ub_tmp[1] = UGreedy(index + 1, w);
	//int lb_tmp = v + LGreedy(index + 1, w);
	//bound = (lb_tmp > bound)? lb_tmp: bound;

	if(ub_tmp[0] < 0){
		DFS(index + 1, w, v, v + ub_tmp[1]);
		return;
	}
	if(ub_tmp[0] > ub_tmp[1]){	
		DFS(index + 1, w + obj[index].w, v + obj[index].v, v+obj[index].v+ub_tmp[0]);
		DFS(index + 1, w, v, v + ub_tmp[1]);
	}
	else{
		DFS(index + 1, w, v, v + ub_tmp[1]);
		DFS(index + 1, w + obj[index].w, v + obj[index].v, v+obj[index].v+ub_tmp[0]);
	}
	return;
}

int main(){
	scanf("%d %d", &W, &N);
	for(int i=0; i<N; i++){
		int w, v;
		scanf("%d %d", &v, &w);
		obj[i].v = v;
		obj[i].w = w;
		obj[i].uV = (float)v/w;
	}
	qsort(obj, N, sizeof(obj[0]), compare);
	float ub = UGreedy(0, 0);
	bound = LGreedy(0, 0);
	DFS(0, 0, 0, ub);
	printf("%d\n", bound);
	return 0;
}
