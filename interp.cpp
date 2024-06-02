#include "interp.h"

Matrix solve_LU(Matrix& A, Matrix& b){
	ASSERT(A.getRows() == A.getCols(), "Matrix A must be square\n");
	ASSERT(A.getRows() == b.getRows(), "Matrix A and b must have the same number of rows\n");
	ASSERT(b.getCols() == 1, "Matrix b must have only one column\n");
	int N = A.getRows();
	LU lu = A.decomposeLU();

	Matrix y(N);
	Matrix x(N);

	for(int i = 0; i<N; i++){
		double sum = 0;
		for(int j = 0; j<i; j++){
			sum += lu.L->getElement(i,j)*y.getElement(j);
		}
		y.setElement(i,0,(b.getElement(i)-sum)/lu.L->getElement(i,i));
	}

	for(int i = N-1; i>=0; i--){
		double sum = 0;
		for(int j = i+1; j<N; j++){
			sum += lu.U->getElement(i,j)*x.getElement(j);
		}
		x.setElement(i,0,(y.getElement(i)-sum)/lu.U->getElement(i,i));
	}

	Matrix error_vec = (A*x) - b;
	double error = 0;
	for (int i = 0; i < N; i++){
		error += error_vec.getElement(i) * error_vec.getElement(i);
	}
	error = sqrt(error);
	delete lu.L;
	delete lu.U;
	return x;
}

vector<long double> interp_lagrange(vector<point> selected_points, vector<point> x_interp)
{
	vector<long double> y_interp;
	int N = x_interp.size();
	int n = selected_points.size();
	for (int i = 0; i < N; i++)
	{
		long double x = x_interp[i].x;
		long double y = 0;
		for (int j = 0; j < n; j++)
		{
			long double L = 1;
			for (int k = 0; k < n; k++)
			{
				if (k != j)
				{
					L *= (x - selected_points[k].x) / (selected_points[j].x - selected_points[k].x);
				}
			}
			y += selected_points[j].y * L;
		}
		y_interp.push_back(y);
	}
	return y_interp;
}

vector<long double> interp_cubic_spline(vector<point> selected_points, vector<point> x_interp)
{
	int subintervals = selected_points.size() - 1;
	int n = 4 * (subintervals);
	Matrix A(n, n);
	Matrix b(n);
	int row = 0;
	for (int i = 0; i<subintervals; i++){
		A.setElement(row, 4*i, 1);
		b.setElement(row, 0, selected_points[i].y);
		row++;
		int h = selected_points[i+1].x - selected_points[i].x;
		A.setElement(row, 4*i, 1);
		A.setElement(row, 4*i+1, h);
		A.setElement(row, 4*i+2, pow(h,2));
		A.setElement(row, 4*i+3, pow(h,3));
		b.setElement(row, 0, selected_points[i+1].y);
		row++;

		if(i<subintervals-1){
			A.setElement(row, 4*i+1, 1);
			A.setElement(row, 4*i+2, 2*h);
			A.setElement(row, 4*i+3, 3*pow(h,2));
			A.setElement(row,(i+1)*4+1, -1);
			row++;

			A.setElement(row, 4*i+2, 2);
			A.setElement(row, 4*i+3, 6*h);
			A.setElement(row,(i+1)*4+2, -2);
			row++;
		}

	}
	A.setElement(row, 2, 1);
	row++;
	A.setElement(row, n-2, 2);
	A.setElement(row, n-1, 6*(selected_points[subintervals].x-selected_points[subintervals-1].x));
	row++;
	Matrix c = solve_LU(A, b);
	vector<long double> y_interp;
	for(int i = 0; i<x_interp.size(); i++){
		printf("%Lf\n", x_interp[i].x);
		bool found = false;
		for(int j = 0; j<selected_points.size()-1; j++){
			if(x_interp[i].x>=selected_points[j].x && x_interp[i].x<=selected_points[j+1].x){
				long double h = x_interp[i].x - selected_points[j].x;
				long double y = c.getElement(4*j) + c.getElement(4*j+1)*h + c.getElement(4*j+2)*pow(h,2) + c.getElement(4*j+3)*pow(h,3);
				y_interp.push_back(y);
				printf("%Lf %Lf\n", x_interp[i].x, y);
				found = true;
				break;
			}
		}
		if(!found){
			printf("%Lf not found\n", x_interp[i].x);
			y_interp.push_back(y_interp[y_interp.size()-1]);
		}
	}
	return y_interp;
}

vector<point> get_points_linspace(vector<point> x, int count){
	vector<point> x_interp;
	long double x_min = x[0].x;
	long double x_max = x[x.size()-1].x;
	long double dx = (x_max-x_min)/(count-1);
	for(int i = 0; i<count; i++){
		long double x_interp_i = x_min + i*dx;
		int j = 0;
		while(j<x.size()-1 && x_interp_i>x[j+1].x){
			j++;
		}
		x_interp_i = x[j].x;
		long double y_interp_i = x[j].y;
		x_interp.push_back({x_interp_i, y_interp_i});
	}
	return x_interp;
}

vector<point> get_points_chebyshev(vector<point> x, int count)
{
	vector<point> x_interp;
	long double x_min = x[0].x;
	long double x_max = x[x.size() - 1].x;
	for (int i = 0; i < count; i++)
	{
		long double x_interp_i = (x_min + x_max) / 2 + (x_max - x_min) / 2 * cos((2 * i + 1) * M_PI / (2 * count));
		int j = 0;
		while (j < x.size() - 1 && x_interp_i > x[j + 1].x)
		{
			j++;
		}
		x_interp_i = x[j].x;
		long double y_interp_i = x[j].y;
		x_interp.push_back({x_interp_i, y_interp_i});
	}
	return x_interp;
}
