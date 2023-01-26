import matplotlib.pyplot as plt
import numpy as np
from scipy import stats

independent = "sample step"
dependent = "predator population size"

data = open("predator_territorial.csv", "r")

names = data.readline().split(",")
names = [trimmer[1:-1] for trimmer in names]

names[-1] = names[-1][:-1] #this is neccesary as there is an extra caracter in the end

indp_i = names.index(independent)
dp_i = names.index(dependent)
pr_i = names.index("predation rate")
prey_i = names.index("prey population size")

xpoints = []
y1points = []
y2points = []

lstp = -1

for sample in data:
    sample = str(sample).split(",")
    if int(sample[prey_i]) > 139:
        break

for sample in data: #is name here?
    sample = str(sample).split(",")
    tstp = int(sample[indp_i])
    if tstp < lstp:
        break
    lstp = tstp
    xpoints.append(tstp)
    y1points.append(int(sample[dp_i]))
    y2points.append(float(sample[pr_i]))

x = np.array(xpoints)
y1 = np.array(y1points)
y2 = np.array(y2points)

fig, ax1 = plt.subplots()
#plot y1 on ax1, y2 on ax2, share x-axis

ax1.set_xlabel("Measurement")
ax1.set_ylabel("Numerical response", color="red")
plt.plot(x, y1, color='red')
ax1.tick_params(axis="y", labelcolor="red")

ax2 = ax1.twinx()
ax2.set_ylabel("Predation rate", color="blue")
plt.plot(x, y2, color='blue')
ax2.tick_params(axis="y", labelcolor="blue")

plt.title("Numerical response and predation rate over course of one run")

fig.tight_layout()
plt.savefig("./plots/individual.png")
plt.show()

# compute skewness of y1 and y2

print("Skewness of y1: ", stats.skew(y1))
print("Skewness of y2: ", stats.skew(y2))

# show histogram of y1 and y2

fig, (ax1, ax2) = plt.subplots(1, 2)
ax1.hist(y1, bins=20)
ax1.set_title("Histogram of y1")
ax2.hist(y2, bins=20)
ax2.set_title("Histogram of y2")
plt.savefig("./plots/individual_hist.png")
plt.show()