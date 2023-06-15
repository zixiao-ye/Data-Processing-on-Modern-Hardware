import matplotlib.pyplot as plt


# line 1 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y1 = [300550976, 340325828, 390639196, 599515505, 379667826]
# plotting the line 1 points
plt.plot(x11, y1, label = "coarse-grained locking")
_ = plt.xticks(x11,x1)

# line 2 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y2 = [319289301, 380090052, 434262697, 559592139, 469252422]
# plotting the line 1 points
plt.plot(x11, y2, label = "coarse-grained locking with RW")
_ = plt.xticks(x11,x1)

# line 3 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y3 = [1277215957, 1250193769, 991045056, 1016928067, 940218637]
# plotting the line 1 points
plt.plot(x11, y3, label = "lock coupling")
_ = plt.xticks(x11,x1)

# line 4 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y4 = [1092957357, 1602362721, 1420410716, 1159240653, 1186571857]
# plotting the line 1 points
plt.plot(x11, y4, label = "lock coupling with RW")
_ = plt.xticks(x11,x1)

# line 5 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y5 = [446481898, 400161684, 282841422, 262431233, 139222333]
# plotting the line 1 points
plt.plot(x11, y5, label = "optimistic")
_ = plt.xticks(x11,x1)




# naming the x axis
plt.xlabel('number of threads')
# naming the y axis
plt.ylabel('clock-cycles')
# giving a title to my graph
plt.title('synchronization')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

