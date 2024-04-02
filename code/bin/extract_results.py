import sys
import os
import csv
import pandas as pd
import re

# first argv ist the input directory
output_dirs = ["C:\\Users\\Lis\\Uni\\sem12\\Hiwi\\masterarbeit\\mastersthesis\\code\\output\\E1\\",
              "C:\\Users\\Lis\\Uni\\sem12\\Hiwi\\masterarbeit\\mastersthesis\\code\\output\\E2\\"]

n=[]
m=[]
lb=[]
ub=[]
makespans=[]
total_t=[] 
prep_t=[] 
iter_t=[] 
d=[]
tau=[]
iters=[]
box_size=[]
box_bound=[]
cols=[]
pmax=[]
pmin=[]
for output_dir in output_dirs:
    print (output_dir)
    fileList = os.listdir(output_dir)

    for filename in fileList:
        nums = [int(s) for s in re.findall(r'\d+', filename)]
        m.append(nums[0])
        n.append(nums[1])
        lb.append(nums[2])
        ub.append(nums[3])
        with open(output_dir + filename) as f:
            #if filename.startswith(group):
                lines = f.readlines()

                # get makespan and processing times
                line = lines[-1]
                if (line[23:39] == "The makespan is "):
                    makespans.append(float(line[39:]))
                    line = lines[-2]
                    l = len(line)
                    iter_t.append(float(line[56:l-3]))
                    line = lines[-3]
                    l = len(line)
                    prep_t.append(float(line[59:l-3]))
                    line = lines[-4]
                    l = len(line)
                    total_t.append(float(line[38:l-3]))
                    
                else:
                    makespans.append(float(0))
                    total_t.append(float(0))
                    prep_t.append(float(0))
                    iter_t.append(float(0))

                # get dimensions and pmax
                line = lines[6]
                numbers = [int(s) for s in re.findall(r'\b\d+\b', line)]
                d.append(len(numbers))
                pmax.append(max(numbers))
                pmin.append(min(numbers))
                line = lines[7]
                numbers = [int(s) for s in re.findall(r'\b\d+\b', line)]
                tau.append(len(numbers))


                # get num iterations
                num_iters = len([i for i in lines if "guessed" in i])
                iters.append(num_iters)

                # get mean columns of config matrix, box size D and actual box bound
                ls = [i for i in lines if "configuration matrix" in i]
                sum = 0
                for l in ls:
                    sum += int(l[70:])
                cols.append(float(sum/num_iters))

                ls = [i for i in lines if "box size" in i]
                sum = 0
                for l in ls:
                    sum += int(l[41:])
                box_size.append(float(sum/num_iters))

                ls = [i for i in lines if "bound" in i]
                sum = 0
                for l in ls:
                    sum += int(l[48:])
                box_bound.append(float(sum/num_iters))

for i in range(len(total_t)):
    total_t[i] = total_t[i]/(iters[i] * 1000)
    prep_t[i] = prep_t[i]/(iters[i] * 1000)
    iter_t[i] =iter_t[i]/(iters[i] * 1000)

d = {'M': m, 'N': n, 'lb': lb, 'ub': ub, 
        'makespan': makespans, 'iters':iters,
        'total_t': total_t, 'prep_t':prep_t, 'iter_t':iter_t,
        'd':d, 'tau':tau, 'pmax':pmax, 'pmin':pmin,
        'cols':cols, 'box_size':box_size, 'box_bound':box_bound}
df = pd.DataFrame(data=d)
#print(df)

df.to_csv('C:\\Users\\Lis\\Uni\\sem12\\Hiwi\\masterarbeit\\mastersthesis\\code\\output\\' + 'all.csv')#index=False)
