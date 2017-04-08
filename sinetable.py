import math

sinewave_frequency = int(input("Enter the frequency of sine wave [50]: ") or "50")
num_samples = int(input("Enter the number of samples [32]: ") or "32")
min_resolution = float(input("Enter minimum resolution of PWM [8]: ") or "8")
max_on_time = float(input("Enter PWM maximum ON time [90]: ") or "90") / 100

sinewave_period = 1 / sinewave_frequency
delay_between_samples = sinewave_period / 2 / num_samples
min_pwm_frequency = 1 / delay_between_samples
duty_cycle_range = pow(2, min_resolution)
max_duty_cycle = int(max_on_time * duty_cycle_range)

out = ""
inc = 180 / num_samples
values = []
for x in range(0, num_samples):
    val = int(math.sin(math.radians(inc * x) ) * max_duty_cycle)
    values.append(val)
    if x != num_samples - 1:
        out += str(val) + ", "
    else:
        out += str(val)


print("Duty Cycle update interval: " + str(delay_between_samples) + "(" + str(delay_between_samples * 1000000) + "us)")
print("Minimum PWM frequency: " + str(min_pwm_frequency))
print("{" + str(out) + "}")

print("\n==================== Calculations for PIC MCU ============================\n")

pwm_frequency = int(input("Enter the frequency of PWM [57kHz]: ") or "57000")
mcu_frequency = int(input("Enter the frequency of MCU [32MHz]: ") or "32000000")
resolution = math.log10(mcu_frequency / pwm_frequency) / math.log10(2)

if resolution < min_resolution:
    print("PWM frequency is too high. Exiting...")
    exit()

print("Resolution: " + str(float(resolution)))
max_range = 2 ** resolution

out = ""
for x in range(0, num_samples):
    val = int((values[x] / 2 ** min_resolution * max_range) / 4)

    if x != num_samples - 1:
        out += str(val) + ", "
    else:
        out += str(val)
print("{" + str(out) + "}")
