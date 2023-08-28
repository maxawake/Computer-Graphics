#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 15 15:43:39 2017

@author: ksdeo
"""

import numpy as np
import matplotlib.pyplot as plt

plt.close("all")

def rotMat(alpha):
  return np.array([[np.cos(alpha), -np.sin(alpha)],
                  [np.sin(alpha), np.cos(alpha)]])

font = {'family' : 'normal',
            'weight' : 'normal',
            'size'   : 22}
  
plt.rc('font', **font)

# ---
# subtask a)

pos = np.array([[0, 0, 0.5, 1, 1],
                [0, 1, 2, 0, 1]])

posActive = np.zeros(pos.shape)
posPassive = np.zeros(pos.shape)

alphaDeg = 30
alpha = alphaDeg / 360.0 * 2 * np.pi

R = rotMat(alpha)

for i in range(np.size(pos, 1)):
  posActive[:, i] = np.matmul(R, pos[:, i])

# ---
# Subtask b) + c)

S_BprimeB = rotMat(-alpha)
transformedBasisVectors = 2 * np.identity(2)
transformedBasisVectors = np.matmul(S_BprimeB, transformedBasisVectors)

# ---
# Subtask d)

for i in range(np.size(pos, 1)):
  posPassive[:, i] = np.matmul(np.linalg.inv(S_BprimeB), pos[:, i])

# ---
# Subtask e)
   
diff = np.sum(np.abs(posActive - posPassive)[:])
if (diff < 1e-6):
  print("Coordinates of basis B' and active rotated coordinates are the same.")
    
# ---
# Plots:

#fig, axes = plt.subplots(1, 3, sharey=True)
#ax = axes[0]
fig, ax = plt.subplots(1, 1, sharey=True)
ax.set_aspect(1)
#axes = ax.gca()

#plt.axis((-2.5, 2.5, -2.5, 2.5))
ax.arrow(0, 0, 2, 0, head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.arrow(0, 0, 0, 2, head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.scatter(pos[0, :], pos[1, :], s=300, marker=".")
#ax.ylim((25,250))
#ax.set_xlim([-2.5, 2.5])
#ax.set_ylim([-2.5, 2.5])
ax.set_aspect('equal')
ax.set_xlim([-1, 2.5])
ax.set_ylim([-1.5, 2.5])
#ax.xaxis.get_major_ticks(1)
ax.set_xticks(np.arange(-2, 4))
#ax.grid(alpha=0.3)
ax.set_ylabel('y')
ax.set_xlabel('x')

#plt.axis('equal')

#ax = axes[1]
fig, ax = plt.subplots(1, 1, sharey=True)
ax.arrow(0, 0, 2, 0, head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.arrow(0, 0, 0, 2, head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.scatter(posActive[0, :], posActive[1, :], s=300, marker=".")
ax.set_aspect('equal')
ax.set_xlim([-1, 2.5])
ax.set_ylim([-1.5, 2.5])
ax.set_xticks(np.arange(-2, 4))
ax.set_ylabel('y')
ax.set_xlabel('x')

#ax.grid(alpha=0.3)

#ax = axes[2]
fTmp, ax = plt.subplots(1, 1, sharey=True)
ax.arrow(0, 0, transformedBasisVectors[0, 0], transformedBasisVectors[1, 0], head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.arrow(0, 0, transformedBasisVectors[0, 1], transformedBasisVectors[1, 1], head_width=0.15, head_length=0.3, fc='k', ec='k', width=0.02)
ax.scatter(pos[0, :], pos[1, :], s=300, marker=".")
ax.set_aspect('equal')
ax.set_xlim([-1, 2.5])
ax.set_ylim([-1.5, 2.5])
ax.set_xticks(np.arange(-2, 4))
#ax.grid(alpha=0.3)
ax.set_ylabel('y')
ax.set_xlabel('x')

#ax.set_ylabel('\$y\$')
#ax.set_xlabel('\$x\$')

# equivalent:
# plt.scatter(x, y, s=80, c=z, marker=None, verts=verts)

fig.tight_layout()
plt.show()
