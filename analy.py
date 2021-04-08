import numpy
import os
outcpy = out = os.popen('LC_ALL=C lscpu').read()
idx = out.find('Model name:')
if idx > 0:
    out = " ".join(out[idx:].split('\n')[0].split()[2:])
beg = outcpy.find('Vulnerability Meltdown:')
end = outcpy.find('Vulnerability Spec store bypass:')

if beg > 0 and end > 0:
    outcpy = " ".join(outcpy[beg:end].split('\n')[0].split()[2:])
else:
    outcpy = "Unknown"

print('\nModel name: %s' % (out))
print('Vulnerability Meltdown: %s\n' % (outcpy))

elements = numpy.loadtxt('tmp.out')
arr = elements.tolist()

mean = numpy.mean(elements, axis=0)
sd = numpy.std(elements, axis=0)

# remove outlier
final_list = [x for x in arr if (x > mean - 2 * sd)]
elements = numpy.array([x for x in final_list if (x < mean + 2 * sd)])

mean = numpy.mean(elements, axis=0)
sd = numpy.std(elements, axis=0)
print('Avg of mode switch takes %.2f cycles and standard deviation is %.2f' % (mean, sd))
