import json
import re
import matplotlib.pyplot as plt

with open('cmake-build-debug/pretty.json', 'r') as file:
    data = file.read().replace('\n', '')
y = json.loads(data)

for key in y:
    parent = map(int,key[1:-1].split(','))
    if y[key] == "":
        continue
    res = re.findall("{.+?}",y[key])
    for item in res:
        child = map(int,item[1:-1].split(','))
        plt.plot([parent[0], child[0]], [parent[1], child[1]], 'ro-')
        print(child)
obstacle1 = plt.Circle((40,25), 5, color='b')
obstacle2 = plt.Circle((17,40), 6, color='g')
obstacle3 = plt.Circle((35,15), 4, color='y')
plt.gca().add_patch(obstacle1)
plt.gca().add_patch(obstacle2)
plt.gca().add_patch(obstacle3)
plt.show()
