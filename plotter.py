import numpy as np
import pylab as py
import math

from matplotlib import rc
rc('text', usetex=True)


py.close('all')

from matplotlib.colors import LogNorm

data = np.loadtxt("HistsOut.dat")

Abins = 90
bins = 180/Abins

Ebins = 700/(350/2)

angles = [i*bins for i in range(Abins)]

colors = py.cm.gist_rainbow(np.linspace(0,1,len(data)))

cArr = [0,20,40,60,80,100,120,140,160,180,200]

markerArr = ["x","*","v","^","<",">","1","2","3","4"]
miter = 0

py.figure(1,figsize=(5,4))
py.clf()
for i in range(len(data)-1):
    if i*Ebins >= 600:
        continue
    if i % 8 != 0:
        continue
    if i*Ebins >= 470:
        py.plot(angles,data[i,:],color=colors[i],ls="--",lw=1,marker=markerArr[miter],ms=2,label=str(i*Ebins))
        miter += 1
    else:
        py.plot(angles,data[i,:],color=colors[i],ls="--",lw=1,marker='o',ms=1.5)

    #py.axvline(i*2,color=colors[i],ls=':')
py.tick_params(axis='both', which='major', labelsize=15)
py.tick_params(axis='both', which='minor', labelsize=10)
py.xlabel("$\\theta$ in deg",fontsize=20)
py.ylabel("Probability Density",fontsize = 20)
py.yscale('log')
py.legend(loc=1,fontsize=8)
py.ylim([1e-6,10])
py.savefig("sampled.pdf",bbox_inches="tight")

"""
data2 = np.loadtxt("Processed.dat")


Eg = 661.7
mc2 = 511

def angle(E):
    return math.acos(1 - mc2/(Eg-E) + mc2/Eg)*180/np.pi


Edge = Eg*(1 - 1/(1 + 2*Eg/mc2))

Edeps = np.linspace(0,Edge,1000)

angles = [angle(E) for E in Edeps]

py.figure(2,figsize=(5,4))
py.clf()
py.hist2d(data2[:,0],data2[:,1],range=[[0,700],[0,180]],bins=(350,180),cmap = py.cm.jet,cmin = 1)
py.tick_params(axis='both', which='major', labelsize=15)
py.tick_params(axis='both', which='minor', labelsize=10)
py.ylabel("$\\theta$ in deg",fontsize=20)
py.xlabel("$E_{dep}$ in keV",fontsize = 20)
#py.plot(Edeps,angles,color="k",lw=1)
py.savefig("sampled2.pdf",bbox_inches="tight")


iter = 0
anglesSave = np.zeros(len(data2))

for i in range(len(data2)):
    if data2[i,0] >= Edge:
        anglesSave[iter] = data2[i,1]
        iter += 1

py.figure(2,figsize=(5,4))
py.clf()
py.hist(anglesSave[0:iter],bins=180,range=[0,180],color="dodgerblue",histtype="step")

"""