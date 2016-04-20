#import math


Qg = 1000e-9
Vg = 10

C = Qg / Vg
R = Vg / 1.5
t = 3 * R * C

print("t = " + str(t))
print("f = " + str(1 / (t)))

F = 1 / (6.2832 * R * C)
aF = F / 3

if aF > 1000000:
    aF = aF / 1000000
    p = " MHz"
    
elif aF > 1000:
    aF = aF / 1000
    p = " kHz"
    
print(str(aF) + p)
