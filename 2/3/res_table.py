import os
import math

root = os.path.dirname(os.path.abspath(__file__))
files = []
dic = {}

for file in os.listdir(root + "/data"):
    if file.endswith(".res.dat"):
        files.append("data/" + file)
        
for file in files:
    dic[file] = sum([float(s.strip()) for s in open(file).readlines()])/(sum(1.0 for line in open(file)))
    

f = open('pi_results.csv','w')
f.write("threads,n,fpi,fpibympi\n")

for key in dic.keys():
    f.write(key.replace("data/mcpi_t", "").replace("_s", ",").replace(".res.dat", ",") + str(dic[key]) + "," + str(abs(dic[key]-math.pi)/math.pi) +  "\n")
f.close()
