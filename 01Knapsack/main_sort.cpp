#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <algorithm>
#define MAXN 1024

typedef struct {
	int value, weight;
	float uValue;
} Object;

int W, N;
int bound = INT_MIN;
Object obj[MAXN];

bool compare(Object a, Object b){
	return a.uValue > b.uValue;
}

float Greedy(int index, int weight){
	float ub = 0;
	for(int i=index; i<N && weight < W; i++){
		float tmp = W - weight;
		if(obj[i].weight <= tmp){
			weight += obj[i].weight;
			ub += obj[i].value;
		}
		else{
			float scale = (float)tmp/obj[i].weight;
			weight = W;
			ub += (float)obj[i].value * scale;
		}
	}
	return ub;
}

void DFS(int index, int weight, int value, float ub){
	if(weight > W) return;
	if(ub <= bound) return;
	if(index >= N){
		if(value > bound) bound = value;
		return;
	}
	DFS(index + 1, weight+obj[index].weight, value+obj[index].value, ub);
	float tmp = Greedy(index+1, weight);
	DFS(index + 1, weight, value, value + tmp);
	return;
}

int main(){
	scanf("%d %d", &W, &N);
	for(int i=0; i<N; i++){
		int weight, value;
		scanf("%d %d", &value, &weight);
		obj[i].value = value;
		obj[i].weight = weight;
		obj[i].uValue = (float)value/weight;
	}
	std::sort(obj, obj + N, compare);
	float ub = Greedy(0, 0);
	DFS(0, 0, 0, ub);
	printf("%d\n", bound);
	return 0;
}
