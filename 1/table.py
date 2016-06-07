def getFirstNum(ln):
    lst = filter(None, ln.split(" "))
    return lst[1]
    
rd_input = open('output.txt', 'r')

string = ""

with open('table.dat', 'w') as t:
    for line in rd_input:
        if line.find('running') > -1:
            string += " " + line.split("\\n")[0].replace("running N=", "")
        elif line.find('Copy') > -1:
            string += " " + getFirstNum(line)
        elif line.find('Scale') > -1:
            string += " " + getFirstNum(line)
        elif line.find('Add') > -1:
            string += " " + getFirstNum(line)
        elif line.find('Triad') > -1:
            string += " " + getFirstNum(line)
            print string
            t.write(string)
            t.write("\n")
            string = ""
