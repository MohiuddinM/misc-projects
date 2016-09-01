import math
import sys

try:
    Qgn = float(sys.argv[1])
    Vg = float(sys.argv[2])
    Ig = float(sys.argv[3])
except Exception, e:
    print("Invalid Arguments\n\nExample Usage:")
    print(sys.argv[0].split('\\')[-1] + " [Qg in nC] [Vg in V] [Ig in A]")
    print(sys.argv[0].split('\\')[-1] + " 140 10 2")
    exit()
    


Qg = Qgn * 1e-9
C = Qg / Vg
R = Vg / Ig
Ton = 3 * R * C

F = 1 / (2 * math.pi * R * C)
aF = F / 3

if aF > 1e6:
    aF = aF / 1000000
    pf = " MHz"
    
elif aF > 1e3:
    aF = aF / 1000
    pf = " kHz"

if Ton < 1e-8:
    Ton = Ton * 1e9
    pt = " nS"
    
elif Ton < 1e-5:
    Ton = Ton * 1e6
    pt = " uS"

elif Ton < 1e-2:
    Ton = Ton * 1e3
    pt = " mS"

print("Switch ON time = " + str(Ton) + pt)
print("Max switching frequency = " + str(aF) + pf)
