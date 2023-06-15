import matplotlib.pyplot as plt


# line 1 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y1 = [1.12, 0.63, 0.39, 0.27, 0.29]
# plotting the line 1 points
plt.plot(x11, y1, label = "coarse-grained locking")
_ = plt.xticks(x11,x1)

# line 2 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y2 = [1.23, 0.55, 0.29, 0.22, 0.23]
# plotting the line 1 points
plt.plot(x11, y2, label = "coarse-grained locking with RW")
_ = plt.xticks(x11,x1)

# line 3 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y3 = [1.36, 0.70, 0.45, 0.33, 0.33]
# plotting the line 1 points
plt.plot(x11, y3, label = "lock coupling")
_ = plt.xticks(x11,x1)

# line 4 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y4 = [1.46, 0.49, 0.28, 0.20, 0.20]
# plotting the line 1 points
plt.plot(x11, y4, label = "lock coupling with RW")
_ = plt.xticks(x11,x1)

# line 5 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y5 = [1.09, 0.60, 0.43, 0.33, 0.36]
# plotting the line 1 points
plt.plot(x11, y5, label = "optimistic")
_ = plt.xticks(x11,x1)




# naming the x axis
plt.xlabel('number of threads')
# naming the y axis
plt.ylabel('IPC')
# giving a title to my graph
plt.title('synchronization')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

