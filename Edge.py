import numpy as np
import pylab as py

c = 3e8

mc2 = 511
mc = mc2/c

def EC(Ein,angle):
    cth = np.cos(angle/180.*np.pi)
    return Ein/(1 + Ein/mc2*(1-cth))

def EC_Dopp(Ein,angle,pzSc):
    cth = np.cos(angle/180.*np.pi)
    tau = 1/(1+Ein/mc2*(1-cth))
    pz = pzSc*mc
    t = (pz/mc)**2
    sign = pz/np.abs(pz)
    A = Ein*tau/(1-t*tau**2)
    B = 1-t*tau*cth
    C = sign*np.sqrt(B**2 - (1-t*tau**2)*(1-t))

    return A*(B + C)

MAX_len = 200

thetas = np.linspace(0,180,1000)

Ec = np.array([661.7-EC(661.7,t) for t in thetas])

EcD = np.zeros((MAX_len,1000))

pzTmp = np.linspace(-0.1,0.1,MAX_len)

for i in range(MAX_len):
    for j in range(1000):
        EcD[i,j] = 661.7-EC_Dopp(661.7,thetas[j],pzTmp[i])



colors = py.cm.gist_rainbow(np.linspace(0,1,MAX_len))

py.figure(1,figsize=(5,4))
py.clf()
for i in range(MAX_len):
    py.plot(thetas,EcD[i,:],color=colors[i])
py.plot(thetas,Ec,color="k")
