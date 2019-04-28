#include <stdio.h>

#define M	3	// resource 
#define N	5	// processes

void work_plus_alloc(int alloc[], int work[]) {
	int i;
	for (i = 0; i < M; i++) {
		work[i] += alloc[i];
	}
}

int is_request_ok(int req[], int work[]) {
	int i;
	for (i = 0; i < M; i++) {
		if (req[i] > work[i])
			return 0;
	}
	return 1;
}

int is_deadlock(int* fini)
{
	int i;
	int result = 0;

	for (i = 0; i < N; i++)
		result += fini[i];
	if (result == N)	// Finish[i]=true for all i, then no deadlock
		return 0;
	else			// if such Finish[i] == false, then Pi is deadlocked
		return 1;
}

int main()
{
	int allocation[N][M] = {
		{0, 1, 0},	// P0
		{2, 0, 0},	// P1
		{3, 0, 3}, 	// P2
		{2, 1, 1},	// P3
		{0, 0, 2}	// P4
	};
	int request[N][M] = {
		{0, 0, 0},	// P0
		{2, 0, 2},	// P1
		{0, 0, 0}, 	// P2
		{1, 0, 0},	// P3
		{0, 0, 2}	// P4
	};

	int available[M] = { 0, };
	int work[M] = { 0, };		
	int finish[N] = { 0, };
	int count = 0;
	int i, j;

	//	1. Let work and finish be array of length M and N,
	//	work = available
	for (i = 0; i < M; i++)
		work[i] = available[i];

	//	 For i = 1,2, ... , N, if allocation[i] != 0, then finish[i] = 0;
	//	 otherwise, finish[i] = 1
	for (i = 0; i < N; i++) {
		finish[i] = 1;
		for (j = 0; j < M; j++) {
			if (allocation[i][j] != 0) {
				finish[i] = 0;
				break;
			}
		}
	}

	/**
	//2. Find an index i such that both:
		(a) finish[i] == 0
		(b) request[i] <= work
		If no such i exists, call is_deadlock(finish)
	**/
	while (1) {
		count = 0;
		for (i = 0; i < N; i++) {
			if ( !finish[i] && is_request_ok(request[i], work) ) {		// finish[i] == 0 && request[i] <= work
				// 3. work = work + allocation[i], and finishi[i] = 1
				work_plus_alloc(allocation[i], work);
				finish[i] = 1;
			}
			else {
				count++;
			}
		}
		/**
		4. If Finish[i] == false, for some i, 1 <= i <= n,
		then the system is in deadlock state.
		Moreover, if finish[i] == 0, then Pi is deadlocked
		**/
		if (count == N) {
			if (is_deadlock(finish)) {
				printf("deadlock\n");
				return 0;
			}
			else {
				printf("no deadlock\n");
				return 0;
			}
		}
	}

}