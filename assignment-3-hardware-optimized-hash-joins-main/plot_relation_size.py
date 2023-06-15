import matplotlib.pyplot as plt



# line 1 points
x11=[1,2,3,4,5] 
x1 = [65536, 131072, 262144, 524288, 1048576]
y1 = [0.05, 0.09, 0.22, 0.48, 1.09]
# plotting the line 1 points
plt.plot(x11, y1, label = "hash join baseline")
_ = plt.xticks(x11,x1)

# line 2 points
x22=[1,2,3,4,5] 
x2 = [65536, 131072, 262144, 524288, 1048576]
y2 = [0.04, 0.10, 0.16, 0.27, 0.53]
# plotting the line 2 points
plt.plot(x22, y2, label = "radix join(naive partition)")
_ = plt.xticks(x22,x2)

# line 3 points
x33=[1,2,3,4,5] 
x3 = [65536, 131072, 262144, 524288, 1048576]
y3 = [0.05, 0.08, 0.14, 0.27, 0.54]
# plotting the line 3 points
plt.plot(x33, y3, label = "radix join(software managed partition)")
_ = plt.xticks(x33,x3)

# line 4 points
x44=[1,2,3,4,5] 
x4 = [65536, 131072, 262144, 524288, 1048576]
y4 = [0.05, 0.09, 0.15, 0.28, 0.57]
# plotting the line 4 points
plt.plot(x44, y4, label = "radix join(multi-pass partition)")
_ = plt.xticks(x44,x4)

# naming the x axis
plt.xlabel('number of tuples in relation')
# naming the y axis
plt.ylabel('runtime (s)')
# giving a title to my graph
# plt.title('Hash Join baseline run time')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

