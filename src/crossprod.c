#include "spm.h"
#include "matmult.h"

void ssyrk_(const char *uplo, const char *trans, const int *n, const int *k,
  const float *restrict alpha, const float *restrict a, const int *lda,
  const float *restrict beta, float *restrict c, const int *ldc);



// // lower triangle of t(x) %*% x
static inline void crossprod(const int m, const int n, const float alpha, const float * const restrict x, float *restrict c)
{
  ssyrk_(&(char){'L'}, &(char){'T'}, &n, &m, &alpha, x, &m, &(float){0.0}, c, &n);
}

static inline void tcrossprod(const int m, const int n, const float alpha, const float * const restrict x, float *restrict c)
{
  ssyrk_(&(char){'L'}, &(char){'N'}, &m, &n, &alpha, x, &m, &(float){0.0}, c, &m);
}

static inline void symmetrize(const int n, float *restrict x)
{
  const int blocksize = 8; // TODO check cache line explicitly
  
  // #pragma omp parallel for default(none) shared(x) schedule(dynamic, 1) if(n>OMP_MIN_SIZE)
  for (int j=0; j<n; j+=blocksize)
  {
    for (int i=j+1; i<n; i+=blocksize)
    {
      for (int col=j; col<j+blocksize && col<n; ++col)
      {
        for (int row=i; row<i+blocksize && row<n; ++row)
          x[col + n*row] = x[row + n*col];
      }
    }
  }
}



SEXP R_crossprod_spm(SEXP x_ptr)
{
  SEXP ret_ptr;
  matrix_t *x = (matrix_t*) getRptr(x_ptr);
  const int m = NROWS(x);
  const int n = NCOLS(x);
  
  matrix_t *ret = newmat(n, n);
  
  crossprod(m, n, 1.0f, DATA(x), DATA(ret));
  symmetrize(n, DATA(ret));
  
  newRptr(ret, ret_ptr, matfin);
  UNPROTECT(1);
  return ret_ptr;
}

SEXP R_crossprod_spmspm(SEXP x_ptr, SEXP y_ptr)
{
  SEXP ret_ptr;
  matrix_t *x = (matrix_t*) getRptr(x_ptr);
  matrix_t *y = (matrix_t*) getRptr(y_ptr);
  
  if (NROWS(x) != NROWS(y))
    error("non-conformable arguments");
  
  matrix_t *ret = newmat(NCOLS(x), NCOLS(y));
  
  matmult(true, false, 1.0f, NROWS(x), NCOLS(x), DATA(x), NROWS(y), NCOLS(y), DATA(y), DATA(ret));
  
  newRptr(ret, ret_ptr, matfin);
  UNPROTECT(1);
  return ret_ptr;
}



SEXP R_tcrossprod_spm(SEXP x_ptr)
{
  SEXP ret_ptr;
  matrix_t *x = (matrix_t*) getRptr(x_ptr);
  const int m = NROWS(x);
  const int n = NCOLS(x);
  
  matrix_t *ret = newmat(m, m);
  
  tcrossprod(m, n, 1.0f, DATA(x), DATA(ret));
  symmetrize(m, DATA(ret));
  
  newRptr(ret, ret_ptr, matfin);
  UNPROTECT(1);
  return ret_ptr;
}

SEXP R_tcrossprod_spmspm(SEXP x_ptr, SEXP y_ptr)
{
  SEXP ret_ptr;
  matrix_t *x = (matrix_t*) getRptr(x_ptr);
  matrix_t *y = (matrix_t*) getRptr(y_ptr);
  
  if (NCOLS(x) != NCOLS(y))
    error("non-conformable arguments");
  
  matrix_t *ret = newmat(NROWS(x), NROWS(y));
  
  matmult(false, true, 1.0f, NROWS(x), NCOLS(x), DATA(x), NROWS(y), NCOLS(y), DATA(y), DATA(ret));
  
  newRptr(ret, ret_ptr, matfin);
  UNPROTECT(1);
  return ret_ptr;
}
