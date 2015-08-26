/*
!==========================================================================
pure function gsw_util_interp1q_int (x, iy, x_i) result(y_i)
!==========================================================================
! Returns the value of the 1-D function iy (integer) at the points of column
! vector x_i using linear interpolation. The vector x specifies the
! coordinates of the underlying interval.
!==========================================================================
*/
double *
gsw_util_interp1q_int(int nx, double *x, int *iy, int nxi, double *x_i,
	double *y_i)
{
	char	*in_rng;
	int	*j, *k, *r, *jrev, *ki, imax_x, imin_x, i, n, m, ii;
	double	*xi, *xxi, u, max_x, min_x;

	if (nx <= 0 || nxi <= 0)
	    return (NULL);

	min_x = max_x = x[0];
	imin_x = imax_x = 0;
	for (i=0; i<nx; i++) {
	    if (x[i] < min_x) {
	        min_x = x[i];
	        imin_x = i;
	    } else if (x[i] > max_x) {
	        max_x = x[i];
	        imax_x = i;
	    }
	}
        in_rng = malloc(nxi*sizeof (char));
	memset(in_rng, 0, nxi*sizeof (char));

	for (i=n=0; i<nxi; i++) {
	    if (x_i[i] <= min_x) {
	        y_i[i] = iy[imin_x];
	    } else if (x_i[i] >= max_x) {
	        y_i[i] = iy[imax_x];
	    } else {
	        in_rng[i] = 1;
	        n++;
	    }
	}

	xi = malloc((n+1)*sizeof (double));
	k  = malloc(3*(n+1)*sizeof (int)); ki = k+n+1; r = ki+n+1;
	m  = nx + n;
	xxi = malloc(m*sizeof (double));
	j = malloc(2*m*sizeof (int)); jrev = j+m;

	ii = 0;
	for (i = 0; i<nxi; i++) {
	    if (in_rng[i]) {
	        xi[ii] = x_i[i];
	        ki[ii] = i;
	        ii++;
	    }
	}
	free(in_rng);
    /*
    **  Note that the following operations on the index
    **  vectors jrev and r depend on the sort utility
    **  gsw_util_sort_real() having the special property
    **  of sorting indecies in reverse sequence for
    **  equal real values in the vector being sorted.
    */
	gsw_util_sort_real(xi, n, k);
	for (i = 0; i<nx; i++)
	    xxi[i] = x[i];
	for (i = 0; i<n; i++)
	    xxi[nx+i] = xi[k[i]];
	gsw_util_sort_real(xxi, nx+n, j);

	for (i = 0; i<nx+n; i++)
	    jrev[j[i]] = i;
	for (i = 0; i<n; i++)
	    r[k[i]] = jrev[nx+i] - i-1;

	for (i = 0; i<n; i++) {
	    u = (xi[i]-x[r[i]])/(x[r[i]+1]-x[r[i]]);
	    y_i[ki[i]] = iy[r[i]] + (iy[r[i]+1]-iy[r[i]])*u;
	}
	free(j); free(xxi); free(k); free(xi);
	return (y_i);
}
