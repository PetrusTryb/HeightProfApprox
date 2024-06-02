#include "matrix.h"
#include <vector>
#include <math.h>
using namespace std;

struct point{
	long double x;
	long double y;
};

typedef point point;

Matrix solve_LU(Matrix& A, Matrix& b);
vector<long double> interp_lagrange(vector<point> selected_points, vector<point> x_interp);
vector<long double> interp_cubic_spline(vector<point> selected_points, vector<point> x_interp);
vector<point> get_points_linspace(vector<point> x, int count);
vector<point> get_points_chebyshev(vector<point> x, int count);