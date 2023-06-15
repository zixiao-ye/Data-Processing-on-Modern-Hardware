import matplotlib.pyplot as plt


# line 1 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y1 = [337476459, 213142364, 152411611, 159740133, 111830189]
# plotting the line 1 points
plt.plot(x11, y1, label = "coarse-grained locking")
_ = plt.xticks(x11,x1)

# line 2 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y2 = [391148096, 207788886, 124113979, 122907749, 110185661]
# plotting the line 1 points
plt.plot(x11, y2, label = "coarse-grained locking with RW")
_ = plt.xticks(x11,x1)

# line 3 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y3 = [1735431540, 869798162, 445561071, 338800574, 311529069]
# plotting the line 1 points
plt.plot(x11, y3, label = "lock coupling")
_ = plt.xticks(x11,x1)

# line 4 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y4 = [1600141844, 790820272, 403655429, 229796790, 239199695]
# plotting the line 1 points
plt.plot(x11, y4, label = "lock coupling with RW")
_ = plt.xticks(x11,x1)

# line 5 points
x11=[1,2,3,4,5] 
x1 = [1, 2, 4, 8, 10]
y5 = [485254751, 241111899, 121204049, 85687364, 49568154]
# plotting the line 1 points
plt.plot(x11, y5, label = "optimistic")
_ = plt.xticks(x11,x1)




# naming the x axis
plt.xlabel('number of threads')
# naming the y axis
plt.ylabel('instructions')
# giving a title to my graph
plt.title('synchronization')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

