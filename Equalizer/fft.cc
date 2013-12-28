#include <math.h>

static void swap( double* x, int j1, int j2 )
{
  double tmp = x[j1];
  x[j1] = x[j2];
  x[j2] = tmp;
}

// двоичная инверсия массива
static void binrevers( double* x, int n )
{
  int i, j, k;

  for( j = i = 0; i < n - 1; i++ ){
    if( i < j )
      swap(x, i, j);
    for( k = n / 2; k <= j; k /= 2 )
      j -= k;
    j += k;
  }
}

static int intlog2( int x )
{
  int res;
  for( res = 0; x != 1; res++ )
    x /= 2;
  return res;
}

static void base_fft( double* x, double* y, int n )
{
  double ur, ui, wr, wi, tr, ti, ur2;
  int i, j, l, le1, le2, ip;

  int r = intlog2(n);

  binrevers(x, n);
  binrevers(y, n);
  for( le2 = l = 1; l <= r; l++ ){
    le1 = le2;
    le2 *= 2;
    ur = 1.0;
    ui = 0.0;
    wr = cos(M_PI / le1);
    wi = -sin(M_PI / le1);
    for( j = 0; j < le1; j++ ){
      for( i = j; i < n; i += le2 ){
        ip = i + le1;
        tr = x[ip] * ur - y[ip] * ui;
        ti = x[ip] * ui + y[ip] * ur;
        x[ip] = x[i];
        y[ip] = y[i];
        x[i] += tr;
        y[i] += ti;
        x[ip] -= tr;
        y[ip] -= ti;
      }
      ur2 = ur * wr - ui * wi;
      ui = ur * wi + ui * wr;
      ur = ur2;
    }
  }
}

static void scale( double* a, int n )
{
  int i;

  for( i = 0; i < n; i++ )
    a[i] /= n;
}

static void resort( double* a, int n )
{
  int i;

  for( i = 1; i < n / 2; i++ )
    swap(a, i, n-i);
}

// export

void cfft( double* a, double* b, int n )
{
  scale(a, n);
  scale(b, n);
  base_fft(a, b, n);
}

void cifft( double* a, double* b, int n )
{
  resort(a, n);
  resort(b, n);
  base_fft(a, b, n);
}
