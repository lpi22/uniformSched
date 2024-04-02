import pandas as pd
import numpy as np

df = pd.read_csv('C:\\Users\\Lis\\Uni\\sem12\\Hiwi\\masterarbeit\\mastersthesis\\code\\output\\all.csv')

"""

# mean of d
print("number of unfinished:")
#df_2 = df.loc[df['makespan'] == 0]
grouped_2 = df.groupby(['d'])
ds_2 = list(grouped_2.groups)
print(grouped_2['d'].count())

df = df.loc[df['makespan'] != 0]
grouped = df.groupby('d')
avg = grouped.mean()
ds = list(grouped.groups)
total_t = avg['total_t'].values.tolist()
prep_t = avg['prep_t'].values.tolist()
iter_t = avg['iter_t'].values.tolist()
iters = avg['iters'].values.tolist()

#print('total time')
#for i in range(len(ds)):
#    print('(' + str(ds[i]) + ',' + str(total_t[i]) + ')')
#print('prep time')
#for i in range(len(ds)):
#    print('(' + str(ds[i]) + ',' + str(prep_t[i]) + ')')
print('iter time')
for i in range(len(ds)):
    print('(' + str(ds[i]) + ',' + str(iter_t[i]) + ')')


"""

"""
# mean of tau
grouped = df.groupby('tau')
avg = grouped.mean()
taus = list(grouped.groups)
total_t = avg['total_t'].values.tolist()
prep_t = avg['prep_t'].values.tolist()
iter_t = avg['iter_t'].values.tolist()

iters = avg['iters'].values.tolist()
print('total time')
for i in range(5):
    print('(' + str(taus[i]) + ',' + str(total_t[i]/iters[i]) + ')')
print('prep time')
for i in range(5):
    print('(' + str(taus[i]) + ',' + str(prep_t[i]/iters[i]) + ')')
print('iter time')
for i in range(5):
    print('(' + str(taus[i]) + ',' + str(iter_t[i]/iters[i]) + ')')
"""


# get certain row
#print(df.loc[df['d'] == 7])


# get difference of box size and bound
#avg = df.mean()
#print(avg['box_size'], avg['box_bound'])

# get avg prep and iter time
#df = df.loc[df['makespan'] != 0]
#avg = df.mean()
#print(avg['total_t'],avg['iter_t'], avg['prep_t'])

"""
# preprocessing col - time
bins = np.arange(0,2400,300)
df = df.loc[df['makespan'] != 0]
grouped = df.groupby(pd.cut(df['cols'],bins))
avg = grouped.mean()
rows = len(avg)
prep_t = avg['prep_t'].values.tolist()
keys = avg['cols'].values.tolist()
for i in range(rows):
    print('(' + str(keys[i]) + ',' + str(prep_t[i]) + ')')
"""
"""
# preprocessing pmax - time - lb = 0
#bins = np.arange(0,2400,300)
df = df.loc[df['makespan'] != 0]
df = df.loc[df['lb'] == 0]
#grouped = df.groupby(pd.cut(df['cols'],bins))
grouped = df.groupby('pmax',as_index=False)
avg = grouped.mean()
rows = len(avg)
prep_t = avg['prep_t'].values.tolist()
keys = avg['pmax'].values.tolist()
for i in range(rows):
    print('(' + str(keys[i]) + ',' + str(prep_t[i]) + ')')

    
# preprocessing pmax - time - lb = 20
df = df.loc[df['makespan'] != 0]
#df = df.loc[df['lb'] == 20]
#grouped = df.groupby(pd.cut(df['cols'],bins))
grouped = df.groupby('pmax',as_index=False)
avg = grouped.mean()
rows = len(avg)
prep_t = avg['prep_t'].values.tolist()
keys = avg['pmax'].values.tolist()
for i in range(rows):
    print('(' + str(keys[i]) + ',' + str(prep_t[i]) + ')')

# preprocessing diff - time
df = df.loc[df['makespan'] != 0]
df['pdiff'] = df['pmax'] - df['pmin'] 
#grouped = df.groupby(pd.cut(df['cols'],bins))
grouped = df.groupby('pdiff',as_index=False)
avg = grouped.mean()
rows = len(avg)
prep_t = avg['prep_t'].values.tolist()
keys = avg['pdiff'].values.tolist()
for i in range(rows):
    print('(' + str(keys[i]) + ',' + str(prep_t[i]) + ')')


# time based on m/n
df = df.loc[df['makespan'] != 0]
df1 = df.loc[df['lb'] == 1]
grouped = df1.groupby(['M','N'], as_index=False)
avg = grouped.mean()
iter_t = avg['iter_t'].values.tolist()
rows = len(avg)
keys = avg['M'].values.tolist()
ns = avg['N'].values.tolist()

print(keys)
for i in range(rows):
    if keys[i] == 3:
        print('(' + str(ns[i]) + ',' + str(iter_t[i]) + ')')

# unfinished based on m/n
df = df.loc[df['makespan'] == 0]
#df1 = df.loc[df['lb'] == 20]
grouped = df.groupby(['M','N'])#, as_index=False)
#avg = grouped.mean()
#iter_t = avg['iter_t'].values.tolist()
#iters = avg['iters'].values.tolist()
#rows = len(avg)
#keys = avg['M'].values.tolist()
#ns = avg['N'].values.tolist()
mns = list(grouped.groups)
print(mns)
print(grouped['M','N'].count())

#for i in range(rows):
#    print('(' + str(keys[i]) + ',' + str(iter_t[i]/iters[i]) + ')')


# pmax
df = df.loc[df['makespan'] != 0]
df = df.loc[df['lb'] != 100]
grouped = df.groupby(['pmax'], as_index=False)
avg = grouped.mean()
iter_t = avg['iter_t'].values.tolist()
rows = len(avg)
keys = avg['d'].values.tolist()
ns = avg['pmax'].values.tolist()

for i in range(rows):
    #if(keys[i]==2):
        print('(' + str(ns[i]) + ',' + str(iter_t[i]) + ')')
"""
# diff
df = df.loc[df['makespan'] != 0]
df = df.loc[df['lb'] != 100]
df['pdiff'] = df['pmax'] - df['pmin'] 
#grouped = df.groupby(pd.cut(df['cols'],bins))
grouped = df.groupby('pdiff',as_index=False)
avg = grouped.mean()
rows = len(avg)
iter_t = avg['iter_t'].values.tolist()
keys = avg['pdiff'].values.tolist()
for i in range(rows):
    print('(' + str(keys[i]) + ',' + str(iter_t[i]) + ')')


"""

# time based on d/n
df = df.loc[df['makespan'] != 0]
df1 = df.loc[df['lb'] == 1]
grouped = df1.groupby(['N','d'], as_index=False)
avg = grouped.mean()
iter_t = avg['iter_t'].values.tolist()
rows = len(avg)
keys = avg['N'].values.tolist()
ds = avg['d'].values.tolist()

for i in range(rows):
    if ds[i] == 5:
        print('(' + str(keys[i]) + ',' + str(iter_t[i]) + ')')

        """
