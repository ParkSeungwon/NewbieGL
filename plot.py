import sys, posix_ipc, os, struct
import matplotlib.pyplot as plt

sz = int(sys.argv[1])
f = posix_ipc.SharedMemory("plot1")
x = [0] * sz
y = [0] * sz
for i in range(sz):
    x[i], y[i] = struct.unpack('ff', os.read(f.fd, 8))
os.close(f.fd)
plt.plot(x, y)
plt.show()

