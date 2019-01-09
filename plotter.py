import numpy as np
import pylab as py
import math

py.close('all')

from matplotlib.colors import LogNorm

data = np.loadtxt("HistsOut.dat")

angles = [i for i in range(180)]

colors = py.cm.jet(np.linspace(0,1,len(data)))

py.figure(1,figsize=(5,4))
py.clf()
for i in range(len(data)-1):
    if i % 10 != 0 or i*2 > 500:
        continue
    py.plot(angles,data[i,:],color=colors[i],ls=":",marker='o',lw=2,ms=4,label=str(i*2))
    #py.axvline(i*2,color=colors[i],ls=':')
py.legend(loc=2,fontsize=8)


"""


Eg = 661.7
mc2 = 511

def angle(E):
    return math.acos(1 - mc2/(Eg-E) + mc2/Eg)*180/np.pi


Edge = Eg*(1 - 1/(1 + 2*Eg/mc2))

Edeps = np.linspace(0,Edge,1000)

angles = [angle(E) for E in Edeps]

py.figure(1,figsize=(5,4))
py.clf()
py.hist2d(data[:,0],data[:,1],range=[[0,700],[0,180]],bins=(350,180),cmap = py.cm.rainbow,cmin = 1)
py.colorbar()
py.plot(Edeps,angles,color="k",lw=1)

iter = 0
anglesSave = np.zeros(len(data))

for i in range(len(data)):
    if data[i,0] >= Edge:
        anglesSave[iter] = data[i,1]
        iter += 1

py.figure(2,figsize=(5,4))
py.clf()
py.hist(anglesSave[0:iter],bins=180,range=[0,180],color="dodgerblue",histtype="step")

"""