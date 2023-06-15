import matplotlib.pyplot as plt



# line 1 points
x11=[1,2,3,4,5] 
x1 = [1, 8, 64, 1024, 8192]
y1 = [16019.9, 4957.1, 3102.2, 3025.7, 3006.8]
# plotting the line 1 points
plt.plot(x11, y1, label = "Q0")
_ = plt.xticks(x11,x1)

# line 2 points
x22=[1,2,3,4,5] 
x2 = [1, 8, 64, 1024, 8192]
y2 = [4935.4, 1579.2, 891.7, 764.4, 803.1]
# plotting the line 2 points
plt.plot(x22, y2, label = "Q1")
_ = plt.xticks(x22,x2)

# line 2 points
x33=[1,2,3,4,5] 
x3 = [1, 8, 64, 1024, 8192]
y3 = [4129.9,1389.1,907.0, 922.0, 905.3]
# plotting the line 2 points
plt.plot(x33, y3, label = "Q2")
_ = plt.xticks(x33,x3)

# naming the x axis
plt.xlabel('vector size (tuples)')
# naming the y axis
plt.ylabel('runtime (ms)')
# giving a title to my graph
plt.title('run time')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

plt.savefig('runtime.eps', format='eps')