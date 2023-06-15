import matplotlib.pyplot as plt



# line 1 points
x11=[1,2,3,4,5] 
x1 = [4, 8, 16, 32, 64]
y1 = [1.46, 1.46, 1.46, 1.46, 1.46]
# plotting the line 1 points
plt.plot(x11, y1, label = "scalar version")
_ = plt.xticks(x11,x1)

# line 2 points
x22=[1,2,3,4,5] 
x2 = [4, 8, 16, 32, 64]
y2 = [0.64, 0.76, 0.82, 1.04, 1.45]
# plotting the line 2 points
plt.plot(x22, y2, label = "vector version")
_ = plt.xticks(x22,x2)



# naming the x axis
plt.xlabel('number of float in one vector (vector version)')
# naming the y axis
plt.ylabel('runtime (s)')
# giving a title to my graph
plt.title('Mandelbrot')

# show a legend on the plot
plt.legend()

# function to show the plot
plt.show()

