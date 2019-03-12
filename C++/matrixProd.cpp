//#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>

using namespace std;

#define SYSTEMTIME clock_t
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void initMatrixes(double * &pha, double * &phb, double * &phc, int n) {
	int i,j;

	pha = (double *)malloc((n * n) * sizeof(double));
	phb = (double *)malloc((n * n) * sizeof(double));
	phc = (double *)malloc((n * n) * sizeof(double));

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			pha[i*n + j] = (double)1.0;

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			phb[i*n + j] = (double)(i+1);

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			phc[i*n + j] = (double)0.0;
}

float OnMult(int n)
{
	SYSTEMTIME Time1, Time2;
	char st[100];
	double temp;
	int i, j, k;
	double *pha, *phb, *phc;

	initMatrixes(pha, phb, phc, n);

  Time1 = clock();

	for(i=0; i<n; i++)
	{	for( j=0; j<n; j++)
		{	
			temp = 0;
			for( k=0; k<n; k++)
			{
				temp += pha[i*n+k] * phb[k*n+j];
			}
			phc[i*n+j]=temp;
		}
	}

  Time2 = clock();

	/*
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	
		for(j=0; j<min(10,n); j++)
			cout << phc[j] << " ";
	}
	cout << endl;
	*/

	free(pha);
	free(phb);
	free(phc);

	return (double)(Time2 - Time1) / CLOCKS_PER_SEC;
}


float OnMultLine(int n)
{
	SYSTEMTIME Time1, Time2;
	char st[100];
	double temp;
	int i, j, k;
	double *pha, *phb, *phc;

	initMatrixes(pha, phb, phc, n);

  Time1 = clock();

	for(i=0; i<n; i++)
	{	
		for( k=0; k<n; k++)
		{
			for( j=0; j<n; j++)
			{
				phc[i*n+j] += pha[i*n+k] * phb[k*n+j];
			}
		}
	}

  Time2 = clock();

	/*
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	
		for(j=0; j<min(10,n); j++)
			cout << phc[j] << " ";
	}
	cout << endl;
	*/

  free(pha);
  free(phb);
  free(phc);

	return (double)(Time2 - Time1) / CLOCKS_PER_SEC;
}

float OnMultBlock(int n, int block)
{
	SYSTEMTIME Time1, Time2;
	char st[100];
	double temp;
	int i, j, k;
	int i0, j0, k0;
	int istep, jstep;
	double *pha, *phb, *phc;

  initMatrixes(pha, phb, phc, n);

  Time1 = clock();

	for (i0 = 0; i0 < n; i0 += block) {
		for (j0 = 0; j0 < n; j0 += block) {
			for (k0 = 0; k0 < n; k0 += block) {
				
				for(i=i0; i<MIN(i0+block, n); i++) {	
					for(k=k0; k<MIN(k0+block, n); k++) {	
						for(j=j0; j<MIN(j0+block,n); j++) {
							phc[i*n+j] += pha[i*n+k] * phb[k*n+j];
						}
					}
				}
				
			}
		}
	}

  Time2 = clock();

	/*
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	
		for(j=0; j<min(10,n); j++)
			cout << phc[j] << " ";
	}
	cout << endl;
	*/

  free(pha);
  free(phb);
  free(phc);

	return (double)(Time2 - Time1) / CLOCKS_PER_SEC;
}

float produtoInterno(float *v1, float *v2, int col)
{
	int i;
	float soma=0.0;

	for(i=0; i<col; i++)
		soma += v1[i]*v2[i];

	return(soma);
}

/*
void handle_error (int retval)
{
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}
*/

int initPAPI(int *EventSet) {
	int ret;

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cerr << "FAIL" << endl;

	ret = PAPI_create_eventset(EventSet);
		if (ret != PAPI_OK) cerr << "ERRO: create eventset" << endl;


	ret = PAPI_add_event(*EventSet, PAPI_L1_DCM);
	if (ret != PAPI_OK) cerr << "ERRO: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(*EventSet, PAPI_L2_DCM);
	if (ret != PAPI_OK) cerr << "ERRO: PAPI_L2_DCM" << endl;

	//ret = PAPI_add_event(*EventSet, PAPI_TLB_DM);
	//if (ret != PAPI_OK) cerr << "ERRO: PAPI_TLB_DM" << endl;

	return ret;
}

int stopPAPI(int *EventSet) {
	int ret;

	ret = PAPI_reset(*EventSet);
	if ( ret != PAPI_OK ) std::cerr << "FAIL reset" << endl;

	ret = PAPI_remove_event(*EventSet, PAPI_L1_DCM);
	if ( ret != PAPI_OK ) std::cerr << "FAIL remove event PAPI_L1_DCM" << endl;

	ret = PAPI_remove_event(*EventSet, PAPI_L2_DCM);
	if ( ret != PAPI_OK ) std::cerr << "FAIL remove event PAPI_L2_DCM" << endl;

	//ret = PAPI_remove_event(*EventSet, PAPI_TLB_DM);
	//if ( ret != PAPI_OK ) std::cerr << "FAIL remove event PAPI_TLB_DM" << endl;

	ret = PAPI_destroy_eventset(EventSet);
	if ( ret != PAPI_OK ) std::cerr << "FAIL destroy" << endl;

	return ret;
}


int main (int argc, char *argv[])
{
	char c;
	int i, iterations, n, block;
	int op;

	int EventSet = PAPI_NULL;
  long long values[2];
  int ret;

	float time;

	if (argc < 4) {
		std::cerr << "./matrix <num iterations> <algorithm> <line/col size> [<block size>]" << endl;
		std::cerr << "(<block size> only needed for algorithm 3)" << endl;
		return 0;
	}

	iterations = atoi(argv[1]);
	op = atoi(argv[2]);
	n = atoi(argv[3]);

	if (op == 3) {
		if (argc < 5) {
			std:cerr << "<block size> needed" << endl;;
			return 0;
		} else {
			block = atoi(argv[4]);
		}
	}

	initPAPI(&EventSet);

	printf("Time, L1_DCM, L2_DCM\n");

	for (i = 0; i < iterations; i++) {
		// Start counting
		ret = PAPI_start(EventSet);
		if (ret != PAPI_OK) cerr << "ERRO: Start PAPI" << endl;
		
		switch (op){
			case 1:
				time = OnMult(n);
				break;
			case 2:
				time = OnMultLine(n);
				break;
			case 3:
				time = OnMultBlock(n, block);
				break;
			default:
				cerr << "Invalid op" << endl;
				return 0;
		}

		ret = PAPI_stop(EventSet, values);
		if (ret != PAPI_OK) cerr << "ERRO: Stop PAPI" << endl;
		
		printf("%3.3f, %lld, %lld\n", time, values[0], values[1]);
	}

	ret = stopPAPI(&EventSet);

}
