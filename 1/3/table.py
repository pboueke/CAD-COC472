import os

root = os.path.dirname(os.path.abspath(__file__))
files = []
outdata = ""

def get_data(filename):
    string = ""
    if (filename.find('IJ') > -1):
        prefix ="IJ "
        prefix += filename.replace("IJ_", "").replace(".dat", "")
    elif (filename.find('JI') > -1):
        prefix = "JI "
        prefix += filename.replace("JI_", "").replace(".dat","")
    with open("time_data/"+filename, "r") as rd_input:
        for line in rd_input:
            if line.find('Initializing') > -1:
                string += prefix + line.replace("Initializing for N = ", " ").replace("\n", "")
            elif line.find('Run') > -1:
                string += line.replace("Run Time = ", " ")
        return string

for file in os.listdir(root+"/time_data"):
    #print file
    if file.startswith("IJ_"):
        files.append(file)
    elif file.startswith("JI_"):
        files.append(file)
        
for file in files:
    outdata += get_data(file)
    

with open('table.txt','w') as t:
    t.write(outdata)

