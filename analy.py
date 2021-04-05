import numpy
import os
out = os.popen('lscpu').read()
idx = out.find('Model name:')
out = " ".join(out[idx:].split('\n')[0].split()[2:])
print('\nModel name: %s' % (out))

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
