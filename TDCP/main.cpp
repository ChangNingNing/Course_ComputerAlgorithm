#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <assert.h>

#define MAXN 30
#define bound (80000.f*80000.f)

int ans[MAXN/2][2];
int nAns = 0;
float min = bound;

int a = 0;
int yCompare(const void *a, const void *b){
	int *A = *(int **)a, *B = *(int **)b;
	return A[1] - B[1];
}

int indexCompare(const void *a, const void *b){
	if(*(int *)a != *(int *)b) return *(int *)a-*(int *)b;
	return *((int *)a+1) - *((int *)b+1);
}

bool xCompare(int i, int j){ return (i < j);}

float Dist(int s[3], int t[3]){
	float distX = ((float)s[0]-t[0])*((float)s[0]-t[0]);
	float distY = ((float)s[1]-t[1])*((float)s[1]-t[1]);
	float dist = distX + distY;
	if(dist <= min){
		if(dist < min){
			min = dist;
			nAns = 0;
		}
		ans[nAns][0] = (s[2]>t[2])? t[2]: s[2];
		ans[nAns][1] = (s[2]>t[2])? s[2]: t[2];
		nAns++;
	}
	return dist;
}

float D_C(int **B, int n){
	if(n <= 1) return bound;
	// Find Xm
	int *tmp = (int *)malloc(sizeof(int)*n);
	for(int i=0; i<n; i++) tmp[i] = B[i][0];
	std::nth_element(tmp, tmp + (n-1)/2, tmp + n, xCompare);
	int Xm = tmp[(n-1)/2];
	int nXm = 0;
	for(int i=0; i<n; i++)
		if(Xm == B[i][0])
			tmp[nXm++] = B[i][1];
	std::nth_element(tmp, tmp + (nXm-1)/2, tmp + nXm, xCompare);
	int Ym = tmp[(nXm-1)/2];
	free(tmp);

	// Split into 2 parts
	int **L = (int **)malloc(sizeof(int *)*(n/2 + 20));
	int **R = (int **)malloc(sizeof(int *)*(n/2 + 20));
	for(int i=0; i<(n/2+20); i++){
		L[i] = (int *)malloc(sizeof(int)*3);
		R[i] = (int *)malloc(sizeof(int)*3);
	}
	int nL = 0, nR = 0;
	for(int i=0; i<n; i++){
		assert(nL < n/2+20 && nR < n/2+20);
		if(B[i][0] > Xm)
			R[nR][0] = B[i][0], R[nR][1] = B[i][1], R[nR][2] = B[i][2], nR++;
		else if(B[i][0] < Xm)
			L[nL][0] = B[i][0], L[nL][1] = B[i][1], L[nL][2] = B[i][2], nL++;
		else{
			if(B[i][1] <= Ym)
				L[nL][0] = B[i][0], L[nL][1] = B[i][1], L[nL][2] = B[i][2], nL++;
			else
				R[nR][0] = B[i][0], R[nR][1] = B[i][1], R[nR][2] = B[i][2], nR++;
		}
	}
	float DL = D_C(L, nL);
	float DR = D_C(R, nR);
	// Merge
	float minD = (DL > DR)? DR: DL;
	for(int i=0, j=0; i<nL && j<nR;){
		while(i<nL && Xm - L[i][0] > minD) i++;
		while(j<nR && R[j][0] - Xm > minD) j++;
		if(i>=nL || j>=nR) break;
		float dist = Dist(L[i], R[j]);
		if(dist < minD) minD = dist;

		if(L[i][1] < R[j][1]) i++;
		else j++;
	}
	free(L);
	free(R);
	return minD;
}

int main(){
	int N;
	scanf("%d", &N);
	int **A = (int **)malloc(sizeof(int *)*N);
	for(int i=0; i<N; i++) A[i] = (int *)malloc(sizeof(int)*3);
	for(int i=0; i<N; i++){
		int a, b;
		scanf("%d %d", &a, &b);
		A[i][0] = a, A[i][1] = b, A[i][2] = i;
	}

	// Sort by Y
	qsort(A, N, sizeof(int *), yCompare);

	// D & C
	float minD = D_C((int **)A, N);
	printf("%.2f %d\n", sqrt(minD), nAns);
	qsort(ans, nAns, sizeof(ans[0]), indexCompare);
	for(int i=0; i<nAns; i++) printf("%d %d\n", ans[i][0]+1, ans[i][1]+1);
	return 0;
}
