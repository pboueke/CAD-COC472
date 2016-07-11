import os

root = os.path.dirname(os.path.abspath(__file__))
files = []
dic = {}

for file in os.listdir(root + "/data"):
    if file.endswith(".time.dat"):
        files.append("data/" + file)
        
for file in files:
    dic[file] = sum([float(s.strip()) for s in open(file).readlines()])/(sum(1.0 for line in open(file)))
    

f = open('time_results.csv','w')
f.write("threads,n,time\n")

for key in dic.keys():
    f.write(key.replace("data/mcpi_t", "").replace("_s", ",").replace(".time.dat", ",") + str(round(dic[key], 3)) + "\n")
f.close()
