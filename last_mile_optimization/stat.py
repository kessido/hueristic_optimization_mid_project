import subprocess
import matplotlib.pyplot as plt

def run_with_parameters(bf=1,r=1,crp=0.7,gsize=500,mod='direct'):
    result = subprocess.run(['x64\Release\last_mile_optimization.exe', '-i', 'Input_midproject.txt', '-o', 'solution.txt', '-m', mod,
        '-b', str(bf), '-r', str(r), '--genetic_car_randomized_precentage', str(crp), '--genetic_generation_size', str(gsize)], stdout=subprocess.PIPE)
    res = str(result.stdout).split("|")[1].split(",")
    res = [float(r) for r in res]
    # duration best_score number_of_solution_tried number_of_generations
    print(res)
    return res

def generation_size_stats():
    x = []
    y = []
    i = 4
    while i < 15:
        cur = 0
        print(i)
        for j in range(10):
            duration,best_score,number_of_solution_tried,number_of_generations = run_with_parameters(bf=1,r=0.2,gsize=int(i))
            cur += best_score
        x.append(i)
        y.append(cur / 10)
        i*=1.1

    fig, ax = plt.subplots()
    ax.set_xscale('log')
    ax.plot(x, y)
    ax.set_xlabel('generation_size')
    ax.set_ylabel('best_solution_after_0.2_second')
    plt.show()

def run_time_effect():
    x = []
    y = []
    i = 0.05
    while i <= 1:
        cur = 0
        print(i)
        rep = 10
        for j in range(rep):
            duration,best_score,number_of_solution_tried,number_of_generations = run_with_parameters(bf=1,r=i,gsize=50,mod='single')
            cur += best_score
        x.append(i)
        y.append(cur / rep)
        i *= 1.5

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel('time')
    ax.set_ylabel('mean_best_solution')
    plt.show()

def crp_effect():
    x = []
    y = []
    i = 0.01
    while i < 1:
        cur = 0
        print(i)
        for j in range(10):
            duration,best_score,number_of_solution_tried,number_of_generations = run_with_parameters(bf=1,r=0.2,crp=1-i,gsize=30)
            cur += best_score
        x.append(1-i)
        y.append(cur / 10)
        i*=1.2

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_xlabel('car_randomize_precentage')
    ax.set_ylabel('best_solution_after_1_second')
    plt.show()


crp_effect()