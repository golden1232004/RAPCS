#ifndef _HMM_H
#define _HMM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI    
#define PI 3.14159265358979323846264338327950288
#endif 

typedef struct _model_t {
	int N;			/* number of states */
	double* p0;		/* initial probs */
	double** a;		/* transition probs */
	int L;			/* dimensionality of data */
	double** mu;	/* state means */
	double** cov;	/* covariance, tied between all states */
} model_t;

void hmm_train(double** x, int T, model_t* model);							/* with scaling */
void forward_backwards(double*** xi, double** gamma, double* loglik, double* loglik1, double* loglik2, int iter, 
					   int N, int T, double* p0, double** a, double** b);
void baum_welch(double* p0, double** a, double** mu, double** cov, int N, int T, int L, double** x, double*** xi, double** gamma);
void viterbi_decode(double** x, int T, model_t* model, int* q, double &score);				/* using logs */
model_t* hmm_init(double** x, int T, int L, int N);
void hmm_close(model_t* model);
void invert(double** cov, int L, double** icov, double* detcov);			/* uses LAPACK (included with Mac OSX) */
double gauss(double* x, int L, double* mu, double** icov, double detcov, double* y, double* z);
double loggauss(double* x, int L, double* mu, double** icov, double detcov, double* y, double* z);
void hmm_print(model_t* model);

#ifdef __cplusplus
}
#endif

#endif

