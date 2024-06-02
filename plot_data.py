import matplotlib.pyplot as plt
import sys
import datetime

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("python plot_data.py <orig> <interp> <interp_points> [title]")
		sys.exit(1)
	input_file = sys.argv[1]
	with open(input_file, "r") as f:
		data = f.readlines()
	x = [float(line.split()[0]) for line in data]
	y = [float(line.split()[1]) for line in data]
	plt.xlabel("Distance [m]")
	plt.ylabel("Terrain Height [m]")
	if len(sys.argv) == 2:
		plt.plot(x, y, 'k')
	if len(sys.argv) >= 4:
		plt.semilogy(x, y, 'k')
		input_file = sys.argv[2]
		with open(input_file, "r") as f:
			data = f.readlines()
		x = [float(line.split()[0]) for line in data]
		y = [float(line.split()[1]) for line in data]
		plt.semilogy(x, y, 'b')
		plt.legend(["Original", "Interpolated"])
		input_file = sys.argv[3]
		with open(input_file, "r") as f:
			data = f.readlines()
		x = [float(line.split()[0]) for line in data]
		y = [float(line.split()[1]) for line in data]	
		plt.scatter(x, y, 10, 'r', 'o')
	if len(sys.argv) == 5:
		plt.title(sys.argv[4])
		plt.savefig(sys.argv[4] + ".png")
	else:
		plt.savefig(f"plot{datetime.datetime.now().strftime('%Y%m%d%H%M%S')}.png")