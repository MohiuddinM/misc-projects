import math

freq = int(input("Enter the frequency of sine wave [50]: ") or "50")
numOfSamples = int(input("Enter the number of samples [32]: ") or "32")
PWMresolution = float(input("Resolution of PWM [8]: ") or "8")
maxONtime = float(input("Enter PWM maximum ON time [90]: ") or "90") / 100

T = 1 / freq
delayBetweenSamples = T / 2 / numOfSamples
minPWMfreq = 1 / delayBetweenSamples
DutyCycleValues = pow(2, PWMresolution)
maxDutyCycleValue = int(maxONtime * DutyCycleValues)

out = ""
inc = 180 / numOfSamples
for x in range(0, numOfSamples):
    val = int(math.sin(math.radians(inc * x) ) * maxDutyCycleValue)
    if x != numOfSamples - 1:
        out += str(val) + ","
    else:
        out += str(val)


print("Duty Cycle update interval: " + str(delayBetweenSamples) + "(" + str(delayBetweenSamples * 1000000) + "us)")
print("Minimum PWM frequency: " + str(minPWMfreq))
print("[" + str(out) + "]")
