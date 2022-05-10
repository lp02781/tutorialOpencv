import numpy as np
rx = np.array([9, -6, 5, 7, -5, 5])
ry = np.array([7, 8, -6.5, 8, 4.5, 5.5])
rz = np.array([30, 30, 30, 50, 50, 50])
u = np.array([1443, 685, 1263, 1327, 847, 1199])
v = np.array([75, 27, 775, 219, 185, 639])
A = np.zeros([2, 11])
B = np.zeros([2, 1])
haha = np.empty([2, 11])
hehe = np.empty([2, 1])
for i in range(6):
    haha = [[rx[i], ry[i], rz[i], 1, 0, 0, 0, 0, -u[i]*rx[i], -u[i]*ry[i], -u[i]*rz[i]],
            [0, 0, 0, 0, rx[i], ry[i], rz[i], 1, -v[i]*rx[i], -v[i]*ry[i], -v[i]*rz[i]]]
    hehe = [[u[i]],
            [v[i]]]        
    A = np.append(A, haha, axis=0)
    B = np.append(B, hehe, axis=0)
A = A[2:,:]
B = B[2:,:] 
H = np.matmul(np.linalg.pinv(A),B)
index = 5
H = [[-5.35133580e+01, -5.56877529e+01, -2.83123311e+00, 8.55339387e+02],
     [-4.24953360e+00, -3.80283119e+01, 5.26015589e-01, 2.86280355e+02],
     [-6.95180688e-02, -3.48627984e-02, -6.56773775e-03, 1.0]]
test =  [[rx[index]],
        [ry[index]],
        [rz[index]],
        [1]]        
proj = np.matmul(H,test)
proj[0,0] = proj[0,0]/proj[2,0]
proj[1,0] = proj[1,0]/proj[2,0]
proj[2,0] = proj[2,0]/proj[2,0]
print(proj)
error_u = 100*abs(u[index] - proj[0,0])/u[index]
error_v = 100*abs(v[index] - proj[1,0])/v[index]
print(error_u)
print(error_v)