import re
import subprocess
import os
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.lines import Line2D

def iCounter(fileName):
    counter = {}
    f = open(fileName, "r")
    for line in f:
        matchObj = re.match("(.*)\t(.*)\t(.*?) (.*)", line)
        if matchObj != None:
            instruction = matchObj.group(2)
            if instruction in counter:
                counter[instruction] += 1
            else:
                counter[instruction] = 1
    return counter

def _addUnusedInstructions(counter1, counter2):
    data = counter2.items()
    for field in data:
        if not field[0] in counter1:
            counter1[field[0]] = 0

def addUnusedInstruction(counterList):
    for counter1 in counterList:
        for counter2 in counterList:
            if counter1 != counter2:
                _addUnusedInstructions(counter1, counter2)

def orderByInstruction(counter):
    data = sorted(counter.items(), key=lambda tup: tup[0])
    return data

def orderByUse(counter):
    data = sorted(counter.items(), key=lambda tup: tup[1], reverse=True)
    return data

def exec(fileName1, fileName2):
    counter1 = iCounter(fileName1)
    counter2 = iCounter(fileName2)
    addUnusedInstruction((counter1, counter2))
    counter1 = orderByInstruction(counter1)
    counter2 = orderByInstruction(counter2)
    return (counter1, counter2)


def get_labels(e1, e2):
    labels = []
    for e in e1:
        labels.append(e[0])
    for e in e2:
        labels.append(e[0])
    labels.sort()
    labels = list(dict.fromkeys(labels))

    return labels

def get_data(e1, e2):
    data1 = []
    data2 = []
    for e in e1:
        data1.append(e[1])
    for e in e2:
        data2.append(e[1])

    return (data1, data2)

def plot(e, title=''):
    labels = get_labels(e[0], e[1])
    data = get_data(e[0], e[1])

    ax = plt.subplot(111)
    width=0.3
    bins = np.array(range(len(data[0])))
    ax.bar(bins, data[0], width=width, color='b', align='center')
    ax.bar(bins+0.3, data[1], width=width, color='g', align='center')

    ax.set_xticks(list(map(lambda x: x, range(0,len(data[0])))))
    ax.set_xticklabels(labels,rotation=45, rotation_mode="anchor", ha="right")
    ax.autoscale(tight=True)

    custom_lines = [Line2D([0], [0], color='b', lw=4),
                Line2D([0], [0], color='g', lw=4)]

    ax.legend(custom_lines, ['Non-Optimized', 'Optimized'])
    ax.set_title(title)

    plt.show()

subprocess.run(['gcc', '-O0', 'example1.c', '-o', 'example1O0'])
p = subprocess.Popen('Objdump -D example1O0 > example1O0.dump', shell=True)
os.waitpid(p.pid, 0)

subprocess.run(['gcc', '-O3', 'example1.c', '-o', 'example1O3'])
p = subprocess.Popen('Objdump -D example1O3 > example1O3.dump', shell=True)
os.waitpid(p.pid, 0)

e1 = exec('example1O0.dump', 'example1O3.dump')

plot(e1, 'example1')

subprocess.run(['gcc', '-O0', 'example2.c', '-o', 'example2O0'])
p = subprocess.Popen('Objdump -D example2O0 > example2O0.dump', shell=True)
os.waitpid(p.pid, 0)

subprocess.run(['gcc', '-O3', 'example2.c', '-o', 'example2O3'])
p = subprocess.Popen('Objdump -D example2O3 > example2O3.dump', shell=True)
os.waitpid(p.pid, 0)

e2 = exec('example2O0.dump', 'example2O3.dump')

plot(e2, 'example2')

subprocess.run(['gcc', '-O0', 'example3.c', '-o', 'example3O0'])
p = subprocess.Popen('Objdump -D example3O0 > example3O0.dump', shell=True)
os.waitpid(p.pid, 0)

subprocess.run(['gcc', '-O3', 'example3.c', '-o', 'example3O3'])
p = subprocess.Popen('Objdump -D example3O3 > example3O3.dump', shell=True)
os.waitpid(p.pid, 0)

e3 = exec('example3O0.dump', 'example3O3.dump')

plot(e3, 'example3')


subprocess.run(['gcc', '-O0', 'example4.c', '-o', 'example4O0'])
p = subprocess.Popen('Objdump -D example4O0 > example4O0.dump', shell=True)
os.waitpid(p.pid, 0)

subprocess.run(['gcc', '-O3', 'example4.c', '-o', 'example4O3'])
p = subprocess.Popen('Objdump -D example4O3 > example4O3.dump', shell=True)
os.waitpid(p.pid, 0)

e4 = exec('example4O0.dump', 'example4O3.dump')
plot(e4, 'example4')