import serial
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy

# write all data input to a txt file
write_to_file = 1
filename = "./data_in.txt"

sample = 1000
x=[]
y=[]
i = 0


if(write_to_file == 1):
    f = open(filename, 'w')
    


#ser = serial.Serial('COM9', 9600, timeout=1)
ser = serial.Serial('COM9', 115200, timeout=1)

print("gathering data...")

## Gather data from ADC
#
while(i<sample):
    line2 = -1
    line = ser.readline()
    lsplit = line.split()
    if(len(lsplit)  >= 1 ):
        if(lsplit[0].isdigit()):
            line2 = int(lsplit[0])
            #print(int(line2))
            if(write_to_file == 1):
                f.write(line)
    x.append(i)
    y.append(int(line2))
    i = i + 1
ser.close()

if(write_to_file == 1):
    f.close()

## Plotting data / data analysis
#
plt.subplot(211)
plt.plot(x[1:len(x)],y[1:len(y)])
plt.ylabel('ADC Value')
plt.xlabel('sample ID')
plt.ylim(-100,1200)
plt.grid(True)
#plt.show()

plt.subplot(212)
#plt.plot(x[1:len(x)],y[1:len(y)])
#mu = 100 # mean of distribution
#sigma = 15 # standard deviation of distribution
n, bins, patches = plt.hist(y[1:len(y)], 32, normed=1, facecolor='g', alpha=0.75)
#z = mlab.normpdf(bins, mu, sigma)
#plt.plot(bins, z, 'r--')
#print(z)
#plt.axis([40, 160, 0, 0.03])
#plt.xlim(0,1024)
plt.grid(True)
plt.ylabel('density')
plt.xlabel('ADC Value (bin)')
#plt.ylim(0,1024)
plt.grid(True)

plt.show()

