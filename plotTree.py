import json
import re
import matplotlib.pyplot as plt

with open('cmake-build-debug/pretty.json', 'r') as file:
    data = file.read().replace('\n', '')
y = json.loads(data)

for key in y:
    parent = list(map(int,key[1:-1].split(',')))
    if y[key] == "":
        continue
    res = re.findall("{.+?}",y[key])
    for item in res:
        child = list(map(int,item[1:-1].split(',')))
        plt.plot([parent[0], child[0]], [parent[1], child[1]], 'ro-')
obstacle1 = plt.Circle((40,25), 15, color='b')
obstacle2 = plt.Circle((17,40), 15, color='g')
obstacle3 = plt.Circle((35,15), 10, color='y')
obstacle4 = plt.Circle((5,5), 3, color='r')
obstacle5 = plt.Circle((60,80), 15, color='r')
obstacle6 = plt.Circle((50,10), 15, color='r')
plt.gca().add_patch(obstacle1)
plt.gca().add_patch(obstacle2)
plt.gca().add_patch(obstacle3)
plt.gca().add_patch(obstacle4)
plt.show()
