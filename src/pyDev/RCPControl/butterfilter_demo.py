import numpy as np
import matplotlib.pyplot as plt
from scipy import signal



def loadCSVfile2():
    tmp = np.loadtxt("hapticFeedback.csv", dtype=np.str, delimiter=",")
    data = tmp[:,2].astype(np.float)
    shift = np.sum(data[0:99])/100
    data = (data - shift)/100
    label = tmp[:,1].astype(np.float)
    return data, label 

fs = 26  # Sampling frequency
fc = 3  # Cut-off frequency of the filter

my_data, my_label= loadCSVfile2()
w = 2 * fc / fs # Normalize the frequency
b, a = signal.butter(2, w, 'low')
output = signal.filtfilt(b, a, my_data)
# std = output - my_data
# print(np.std(std))
# plt.plot(my_label, output, label='cutoff{}'.format(i))
# plt.legend()
# plt.subplot(121)
# plt.plot(my_label, my_data, label='raw')
# for i in range(1,5):
#     w = 2 * i / fs # Normalize the frequency
#     b, a = signal.butter(2, w, 'low')
#     output = signal.filtfilt(b, a, my_data)
#     # std = output - my_data
#     # print(np.std(std))
#     plt.plot(my_label, output, label='cutoff{}'.format(i))
#     plt.legend()

# plt.subplot(122)
# plt.plot(my_label, my_data, label='raw')
# for j in range(1,5):
#     w = 2 * fc / fs # Normalize the frequency
#     b, a = signal.butter(j, w, 'low')
#     output = signal.filtfilt(b, a, my_data)
#     std = output - my_data
#     print(np.std(std))
#     plt.plot(my_label, output, label='order{}'.format(j))
#     plt.legend()
    
# plt.show()

