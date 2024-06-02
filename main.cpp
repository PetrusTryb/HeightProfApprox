#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string.h>
#include "interp.h"
#include <stdio.h>
using namespace std;

typedef vector<point> dataset;

dataset read_data(string filename)
{
	dataset data;
	ifstream file(filename);
	ASSERT(file.is_open(), "File not found\n");
	//data.name = filename.substr(0, filename.find("."));
	string line;
	while (getline(file, line))
	{
		long double x, y;
		if (line.find(" ") != string::npos)
		{
			sscanf(line.c_str(), "%Lf %Lf", &x, &y);
		}
		else
		{
			sscanf(line.c_str(), "%Lf,%Lf", &x, &y);
		}
		//printf("%Lf %Lf\n", x, y);
		data.push_back({x, y});
	}
	file.close();
	return data;
}

void plot_dataset(dataset data)
{
	ofstream tmp("orig.dat");
	for (int i = 0; i < data.size(); i++)
	{
		tmp << data[i].x << " " << data[i].y << "\n";
	}
	tmp.close();
	string command = "python plot_data.py orig.dat";
	system(command.c_str());
}

void plot_dataset_interp(dataset data, dataset data_interp, dataset data_interp_points, string title)
{
	ofstream tmp("orig.dat");
	for (int i = 0; i < data.size(); i++)
	{
		tmp << data[i].x << " " << data[i].y << "\n";
	}
	tmp.close();
	ofstream tmp2("interp.dat");
	for (int i = 0; i < data_interp.size(); i++)
	{
		tmp2 << data_interp[i].x << " " << data_interp[i].y << "\n";
	}
	tmp2.close();
	ofstream tmp3("interp_points.dat");
	for (int i = 0; i < data_interp_points.size(); i++)
	{
		tmp3 << data_interp_points[i].x << " " << data_interp_points[i].y << "\n";
	}
	tmp3.close();
	string command = "python plot_data.py orig.dat interp.dat interp_points.dat \""+title+"\"";
	system(command.c_str());
}

int main()
{
	const string datasets[] = {"2018_paths/chelm.txt", "2018_paths/rozne_wniesienia.txt"};
	const int n_interp_points[] = {6,11,16,32};
	for(string dataset_name: datasets){
		dataset data = read_data(dataset_name);
		plot_dataset(data);
		for(int n_points: n_interp_points){
			dataset data_interp_selected = get_points_linspace(data, n_points);
			vector<long double> data_interp = interp_lagrange(data_interp_selected, data);
			dataset data_interp_points;
			for (int i = 0; i < data.size(); i++)
			{
				data_interp_points.push_back({data[i].x, data_interp[i]});
			}
			string plot_title = dataset_name+" - Lagrange interpolation, n = " + to_string(n_points);
			plot_dataset_interp(data, data_interp_points, data_interp_selected, plot_title);

			data_interp_selected = get_points_chebyshev(data, n_points);
			data_interp = interp_lagrange(data_interp_selected, data);
			data_interp_points.clear();
			for (int i = 0; i < data.size(); i++)
			{
				data_interp_points.push_back({data[i].x, data_interp[i]});
			}
			plot_title = dataset_name+" - Lagrange interpolation (Chebyshev nodes), n = " + to_string(n_points);
			plot_dataset_interp(data, data_interp_points, data_interp_selected, plot_title);

			data_interp_selected = get_points_linspace(data, n_points);
			data_interp = interp_cubic_spline(data_interp_selected, data);
			data_interp_points.clear();
			for (int i = 0; i < data.size(); i++)
			{
				data_interp_points.push_back({data[i].x, data_interp[i]});
			}
			plot_title = dataset_name+" - Cubic spline interpolation, n = " + to_string(n_points);
			plot_dataset_interp(data, data_interp_points, data_interp_selected, plot_title);
		}
	}
	return 0;
}