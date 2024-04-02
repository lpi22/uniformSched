import sys
import os
from datetime import datetime, timedelta
from os import path

folder = "E2"
group = "M4_N20_"

# first argv ist the input directory
output_dir = "C:\\Users\\Lis\\Uni\\sem12\\Hiwi\\masterarbeit\\mastersthesis\\code\\output\\E2\\"
print (output_dir)
fileList = os.listdir(output_dir)
counter = 0
counter2 = 0
for filename in fileList:
    #if filename.startswith(group):
    #with open(output_dir + filename) as f:
    #    last_line = f.readlines()[-1]
    #    if (last_line[23:39] == "The makespan is "):
    #        f.close()
            #print(last_line[39:])

    
    fileList2 = os.listdir(output_dir)
    for filename2 in fileList2:
        if filename2.startswith(filename[:23]) and (filename != filename2):
            filetime = datetime.fromtimestamp(path.getctime(output_dir + filename))
            print(filename)
            print(filetime)
            print(filename2)
            filetime2 = datetime.fromtimestamp(path.getctime(output_dir + filename2))
            print(filetime2)
            if (filetime < filetime2):
                os.remove(output_dir + filename)
                fileList.remove(filename)                          
                print("removed: " + filename)
            else:
                os.remove(output_dir + filename2) 
                fileList.remove(filename2)                          
                print("removed: " + filename2)

        
    counter += 1

print(counter)
print(counter2)