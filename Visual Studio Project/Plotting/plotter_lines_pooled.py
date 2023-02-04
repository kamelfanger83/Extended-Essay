import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats

def savitzky_golay(y, window_size, order, deriv=0, rate=1):
    from math import factorial
    window_size = abs(int(window_size))
    order = abs(int(order))
    if window_size % 2 != 1 or window_size < 1:
        raise TypeError("window_size size must be a positive odd number")
    if window_size < order + 2:
        raise TypeError("window_size is too small for the polynomials order")
    order_range = range(order + 1)
    half_window = (window_size - 1) // 2
    # precompute coefficients
    b = np.mat([[k ** i for i in order_range] for k in range(-half_window, half_window + 1)])
    m = np.linalg.pinv(b).A[deriv] * rate ** deriv * factorial(deriv)
    # pad the signal at the extremes with
    # values taken from the signal itself
    firstvals = y[0] - np.abs(y[1:half_window + 1][::-1] - y[0])
    lastvals = y[-1] + np.abs(y[-half_window - 1:-1][::-1] - y[-1])
    y = np.concatenate((firstvals, y, lastvals))
    return np.convolve(m[::-1], y, mode='valid')

path = ".\\plots"

plots = [ #dependent, axis name, split, infile, title, outfile
    ["predation rate", "Predation rate", "predator scared frames", "scared_frames.csv", "Comparison of duration territorial conflicts", "scared_frames_experiment_PR.png"],
    ["predator population size", "Numerical response", "predator scared frames", "scared_frames.csv", "Comparison of duration territorial conflicts", "scared_frames_experiment_NR.png"],
    ["average prey eaten per predator", "Functional response", "predator scared frames", "scared_frames.csv", "Comparison of duration territorial conflicts", "scared_frames_experiment_FR.png"],

    ["predation rate", "Predation rate", "predator territorial range", "predator_territorial.csv", "Comparison of predator territorial ranges", "territorial_ranges_experiment_PR.png"],
    ["predator population size", "Numerical response", "predator territorial range", "predator_territorial.csv", "Comparison of predator territorial ranges", "territorial_ranges_experiment_NR.png"],
    ["average prey eaten per predator", "Functional response", "predator territorial range", "predator_territorial.csv", "Comparison of predator territorial ranges", "territorial_ranges_experiment_FR.png"]
]
"""
    ["predation rate", "Predation rate", "energy for moving", "energy_move.csv", "Comparison of energy costs for movement", "move_energy_experiment_PR.png"],
    ["predator population size", "Predator population size", "energy for moving", "energy_move.csv", "Comparison of energy costs for movement", "move_energy_experiment_NR.png"],
    ["average prey eaten per predator", "Average prey eaten per predator", "energy for moving", "energy_move.csv", "Comparison of energy costs for movement", "move_energy_experiment_FR.png"],

    ["predation rate", "Predation rate", "energy equivalence", "energy_equivalence.csv", "Comparison of energy costs for reproduction", "reproduction_energy_experiment_PR.png"],
    ["predator population size", "Predator population size", "energy equivalence", "energy_equivalence.csv", "Comparison of energy costs for reproduction", "reproduction_energy_experiment_NR.png"],
    ["average prey eaten per predator", "Average prey eaten per predator", "energy equivalence", "energy_equivalence.csv", "Comparison of energy costs for reproduction", "reproduction_energy_experiment_FR.png"],

    ["predation rate", "Predation rate", "prey speed", "prey_speed.csv", "Comparison of prey speeds", "prey_speed_experiment_PR.png"],
    ["predator population size", "Predator population size", "prey speed", "prey_speed.csv", "Comparison of prey speeds", "prey_speed_experiment_NR.png"],
    ["average prey eaten per predator", "Average prey eaten per predator", "prey speed", "prey_speed.csv", "Comparison of prey speeds", "prey_speed_experiment_FR.png"],

    ["predation rate", "Predation rate", "prey vision distance", "prey_vision.csv", "Comparison of prey vision ranges", "prey_vision_experiment_PR.png"],
    ["predator population size", "Predator population size", "prey vision distance", "prey_vision.csv", "Comparison of prey vision ranges", "prey_vision_experiment_NR.png"],
    ["average prey eaten per predator", "Average prey eaten per predator", "prey vision distance", "prey_vision.csv", "Comparison of prey vision ranges", "prey_vision_experiment_FR.png"],
]"""

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
    l_low_errors_ls = []
    l_high_errors_ls = []
    labels = []

    for values_i in splits:
        values = splits[values_i]
        l_xpoints_ls.append([])
        l_ypoints_ls.append([])
        l_low_errors_ls.append([])
        l_high_errors_ls.append([])
        labels.append(values_i)
        for creater in values:
            l_xpoints_ls[-1].append(creater)
            # put average in ypoints and 95% confidence interval in errors
            l_ypoints_ls[-1].append(np.mean(values[creater]))
            l_low_errors_ls[-1].append(np.mean(values[creater]) - 1.96*np.std(values[creater])/np.sqrt(len(values[creater])))
            l_high_errors_ls[-1].append(np.mean(values[creater]) + 1.96*np.std(values[creater])/np.sqrt(len(values[creater])))
            print(np.sqrt(len(values[creater])))


    minx = 10**10
    maxx = 0

    for xpoints_ls, ypoints_ls, tlabel, low_error, high_error in zip(l_xpoints_ls, l_ypoints_ls, labels, l_low_errors_ls, l_high_errors_ls):

        minx = min(minx, int(min(xpoints_ls)))
        maxx = max(maxx, int(max(xpoints_ls)))

        x = np.array(xpoints_ls)
        y = np.array(ypoints_ls)
        low_error = np.array(low_error)
        high_error = np.array(high_error)

        plt.plot(x, y, "-", label=tlabel, linewidth=1)

        #plot error around with fill_between
        plt.fill_between(x, low_error, high_error, alpha=0.4)

        """yhat = savitzky_golay(y, 51, 3) # window size 51, polynomial order 3
        yhat = np.maximum(yhat, 0)
        plt.plot(x,yhat, color='red')"""

    plt.xlabel("Prey density")
    plt.ylabel(plot[1])
    plt.legend()

    plt.xticks(np.arange(minx-1, maxx+2, 50))

    plt.tight_layout()

    plt.savefig(path + "\\" + plot[5])
    plt.show()