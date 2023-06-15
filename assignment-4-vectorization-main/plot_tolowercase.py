import matplotlib.pyplot as plt



# line 1 points
x11=[1,2,3,4] 
x1 = [100, 1000, 10000, 500000]
y1 = [0.00253, 0.0158, 0.15785, 2.96859]
# plotting the line 1 points
plt.plot(x11, y1, label = "scalar version")
_ = plt.xticks(x11,x1)

# line 2 points
x22=[1,2,3,4] 
x2 = [100, 1000, 10000, 500000]
y2 = [0.00028, 0.00114, 0.01166, 0.56678]
# plotting the line 2 points
plt.plot(x22, y2, label = "scalar no branch version")
_ = plt.xticks(x22,x2)

# line 3 points
x33=[1,2,3,4] 
x3 = [100, 1000, 10000, 500000]
y3 = [0.00073, 0.00471, 0.0356, 1.76983]
# plotting the line 3 points
plt.plot(x33, y3, label = "vector version")
_ = plt.xticks(x33,x3)



# naming the x axis
plt.xlabel('number of characters')
# naming the y axis
plt.ylabel('runtime (ms)')
# giving a title to my graph
# plt.title('Hash Join baseline run time')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

