import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0, 30, 0.1);
m = c1 = c2 = c = k = 1;
c = 0.1
w = np.sqrt(4*m*k - c*c) / (2*m)
print w
y = np.e ** (-c * t / 2 / m) * (c1 * np.cos(w*t) + c2 * np.sin(w*t));
plt.plot(t, y)
plt.show()
