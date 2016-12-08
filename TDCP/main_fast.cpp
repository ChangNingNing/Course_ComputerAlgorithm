#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#define MAXN 300000
#define bound INT_MAX

typedef struct {
	int x, y, index;
} Point;

int ans[MAXN/2][2], nAns = 0;
int min = bound;

int indexCompare(const void *a, const void *b){
	if(*(int *)a != *(int *)b) return *(int *)a-*(int *)b;
	return *((int *)a+1) - *((int *)b+1);
}

bool xCompare(Point a, Point b){
	if(a.x != b.x) return a.x < b.x;
	return a.y < b.y;
}

bool yCompare(Point a, Point b){
	if(a.y != b.y) return a.y < b.y;
	return a.x < b.x;
}

inline int Dist(Point s, Point t){
	int distX = (s.x - t.x)*(s.x - t.x);
	int distY = (s.y - t.y)*(s.y - t.y);
	int dist = distX + distY;
	if(dist <= min){
		if(dist < min){
			min = dist;
			nAns = 0;
		}
		ans[nAns][0] = (s.index > t.index)? t.index: s.index;
		ans[nAns][1] = (s.index > t.index)? s.index: t.index;
		nAns++;
	}
	return dist;
}

int D_C(Point *B, int n){
	if(n <= 1) return bound;

	// Split into 2 parts
	int Xm = B[n/2-1].x, nL = n/2, nR = n - n/2;
	Point *L = B, *R = B + nL;
	int DL = D_C(L, nL);
	int DR = D_C(R, nR);

	// Merge
	int minD = (DL > DR)? DR: DL;
	int l = 0, r = 0;
	int bsL = 0, bsR = nL-1;
	while(bsL <= bsR){
		int mid = (bsL + bsR) / 2;
		if(Xm - L[mid].x > minD) bsL = mid + 1;
		else bsR = mid - 1;
	}
	l = bsL;
	bsL = 0, bsR = nR-1;
	while(bsL <= bsR){
		int mid = (bsL + bsR) / 2;
		if(R[mid].x - Xm <= minD) bsL = mid + 1;
		else bsR = mid - 1;
	}
	r = bsL;
	if(l >= nL || r == 0) return minD;

	static Point tmpL[MAXN/2];
	static Point tmpR[MAXN/2];
	memcpy(tmpL, L+l, sizeof(Point)*(nL-l));
	memcpy(tmpR, R, sizeof(Point)*r);
	std::sort(tmpL, tmpL + nL-l, yCompare);
	std::sort(tmpR, tmpR + r, yCompare);

	for(int i=0, j=0; i<nL-l && j<r;){
		int dist = Dist(tmpL[i], tmpR[j]);
		if(dist < minD) minD = dist;
		if(tmpL[i].y <= tmpR[j].y) i++;
		else j++;
	}
	return minD;
}

int main(){
	int N;
	scanf("%d", &N);
	Point *P = (Point *)malloc(sizeof(Point)*N);
	for(int i=0; i<N; i++){
		scanf("%d %d", &(P[i].x), &(P[i].y));
		P[i].index = i;
	}

	// D & C
	std::sort(P, P+N, xCompare);
	int minD = D_C(P, N);
	printf("%.2f %d\n", sqrt(minD), nAns);
	qsort(ans, nAns, sizeof(ans[0]), indexCompare);
	for(int i=0; i<nAns; i++) printf("%d %d\n", ans[i][0]+1, ans[i][1]+1);
	return 0;
}
