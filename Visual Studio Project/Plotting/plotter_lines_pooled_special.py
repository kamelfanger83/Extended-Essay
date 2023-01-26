import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats

path = ".plots/exp"

plots = [ #dependent, axis name, split, infile, title, outfile
    ["predation rate", "Predation rate normalized", "predator territorial range", "predator_territorial.csv", "Comparison of inverse density dependence", "territorial_ranges_experiment_PR_normal.png"],
    ["predation rate", "Predation rate normalized", "predator scared frames", "scared_frames.csv", "Comparison of duration of territoriality", "scared_frames_experiment_PR_normal.png"],
]

for plot in plots:

    independent = "prey population size"
    dependent = plot[0]
    split = plot[2]

    data = open(plot[3], "r")

    plt.title(plot[4])

    names = data.readline().split(",")
    names = [trimmer[1:-1] for trimmer in names]

    names[-1] = names[-1][:-1] #this is neccesary as there is an extra caracter in the end

    indp_i = names.index(independent)
    dp_i = names.index(dependent)
    s_i = names.index(split)

    splits = {}

    tgroup = []
    for sample in data: #is name here?
        tl = str(sample).split(",")
        tsplit = float(tl[s_i])
        if(tsplit not in splits): splits[tsplit] = {}
        key = float(tl[indp_i])
        if(key not in splits[tsplit]): splits[tsplit][key] = []
        tgroup.append(float(tl[dp_i]))
        if(len(tgroup) == 99):
            tgroup = np.array(tgroup)
            splits[tsplit][key].append(np.mean(tgroup))
            tgroup = []

    l_xpoints_ls = []
    l_ypoints_ls = []
    l_error_ls = []
    labels = []
    l_highest = []

    for values_i in splits:
        values = splits[values_i]
        l_xpoints_ls.append([])
        l_ypoints_ls.append([])
        l_error_ls.append([])
        l_highest.append(0)
        labels.append(values_i)
        for creater in values:
            l_xpoints_ls[-1].append(creater)
            # put average in ypoints and standard error in error
            l_ypoints_ls[-1].append(np.mean(values[creater]))
            if l_ypoints_ls[-1][-1] > l_highest[-1]: l_highest[-1] = l_ypoints_ls[-1][-1]
            l_error_ls[-1].append(stats.sem(values[creater]))

    minx = 10**10
    maxx = 0

    for xpoints_ls, ypoints_ls, tlabel, error, highest in zip(l_xpoints_ls, l_ypoints_ls, labels, l_error_ls, l_highest):

        minx = min(minx, int(min(xpoints_ls)))
        maxx = max(maxx, int(max(xpoints_ls)))

        x = np.array(xpoints_ls)
        y = np.array(ypoints_ls)
        error = np.array(error)

        # divide y and error by highest
        y = y / highest
        error = error / highest


        # plot x/y with using error as error
        plt.errorbar(x, y, yerr=error, label=tlabel)



    plt.xlabel("Prey population size")
    plt.ylabel(plot[1])
    plt.legend()

    plt.xticks(np.arange(minx-1, maxx+2, 50))

    plt.savefig(path + "\\" + plot[5])
    plt.show()