import os

root = os.path.dirname(os.path.abspath(__file__))
files = []
outdata = ""

def get_data(filename):
    string = ""
    with open("time_data/"+filename, "r") as rd_input:
        for line in rd_input:
            if line.find('Block') > -1:
                string += line.replace("Block size NB = ", " ").replace("\n", "")
            elif line.find('Run') > -1:
                string += line.replace("Run Time = ", " ")
        return string

for file in os.listdir(root+"/time_data"):
    #print file
    if (file.find('block') > -1):
        outdata = get_data(file)

with open('block_table.txt','w') as t:
    t.write(outdata)

