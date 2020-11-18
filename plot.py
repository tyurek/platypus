import matplotlib.pyplot as plt
strlist = ['1x1', '1x0', '1xn1', '0x1', '0x0', '0xn1', 'n1x1', 'n1x0', 'n1xn1', '654321x123456']
scale=20
cutoff=30
windowsize=100

def smoothe(arr, windowsize):
	a,b = 0, windowsize
	avg = sum(arr[:windowsize]+[arr[0],-arr[windowsize]])/windowsize
	out = []
	for i in range(len(arr)-windowsize):
		avg += (arr[b]- arr[a])/windowsize
		out.append(avg)
		a+=1
		b+=1
	return out
	
for name in strlist:
	f = open(name + ".txt", 'r')
	arr = [int(line, 16) for line in f]
	arrnorm = [item -arr[0] for item in arr]
	arrnormdedup = [arrnorm[i] for i in range(1,len(arrnorm)//scale) if arrnorm[i] != arrnorm[i-1]]
	arrderiv = [arrnormdedup[i] - arrnormdedup[i-1] for i in range(1,len(arrnormdedup))]
	arrderivfilt = [arrderiv[i] for i in range(len(arrderiv)) if arrderiv[i] < cutoff]
	arrderivfiltsmooth = smoothe(arrderivfilt,windowsize)
	#plt.plot(arrnorm, label=name)
	plt.plot(arrderivfiltsmooth, label=name)
'''
fbase = open('base.txt', 'r')
f1x1 = open('1x1.txt', 'r')
f1x0 = open('1x0.txt', 'r')
base, m1x1, m1x0 = [], [], []
for line in fbase:
	base.append(int(line, 16))
for line in f1x1:
	m1x1.append(int(line, 16))
for line in f1x0:
	m1x0.append(int(line, 16))
basenorm = [item -base[0] for item in base]
m1x1norm = [item -m1x1[0] for item in m1x1]
m1x0norm = [item -m1x0[0] for item in m1x0]
#plt.plot(basenorm)
plt.plot(m1x1norm)
plt.plot(m1x0norm)
'''
plt.ylabel("dNormalized MSR_PP0_ENERGY_STATUS/dt")
plt.xlabel("sample number")
plt.legend()
plt.show()
