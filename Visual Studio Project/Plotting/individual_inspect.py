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

vals = [0]*22
i = 0

for sample in data: #is name here?
    sample = str(sample).split(",")

    if int(sample[prey_i]) != 140: continue

    tstp = int(sample[indp_i])
    if tstp < lstp:
        i += 1
        if len(vals) == i:
            break
    lstp = tstp

    vals[i] = float(sample[dp_i])

i = 0

for num in vals:
    #print 4 decimal places
    print("{:.1f}".format(num), end="   ")
    i += 1
    if i == 11:
        print()
        i = 0

# box plot

data = [vals[:11], vals[11:]]

fig = plt.figure(1, figsize=(9, 6))
ax = fig.add_subplot(111)
bp = ax.boxplot(data)

ax.set_xticklabels(['0.05', '0.1'])

plt.xlabel("Predator territorial range")
plt.ylabel("Predator population size")

print(stats.ttest_ind(data[0], data[1]))
print(stats.tstd(data[0]))
print(stats.tstd(data[1]))

plt.show()