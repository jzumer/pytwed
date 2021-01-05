#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 28 13:16:54 2020
@author: pfm
"""
import numpy as np


def Dlp(A, B, p=2):
    cost = np.sum(np.power(np.abs(A - B), p))
    return np.power(cost, 1.0 / p)


def twed(A, timeSA, B, timeSB, nu, lmbda, degree):
    # [distance, DP] = TWED( A, timeSA, B, timeSB, lambda, nu )
    # Compute Time Warp Edit Distance (TWED) for given time series A and B
    #
    # A      := Time series A (e.g. [ 10 2 30 4])
    # timeSA := Time stamp of time series A (e.g. 1:4)
    # B      := Time series B
    # timeSB := Time stamp of time series B
    # lambda := Penalty for deletion operation
    # nu     := Elasticity parameter - nu >=0 needed for distance measure
    # degree := Degree of the p norm for local cost.
    # Reference :
    #    Marteau, P.; F. (2009). "Time Warp Edit Distance with Stiffness Adjustment for Time Series Matching".
    #    IEEE Transactions on Pattern Analysis and Machine Intelligence. 31 (2): 306–318. arXiv:cs/0703033
    #    http://people.irisa.fr/Pierre-Francois.Marteau/

    n = A.shape[0]
    m = B.shape[0]
    # Dynamical programming
    DP = np.zeros((n, m))

    # Initialize DP Matrix and set first row and column to infinity
    DP[0, 1:] = np.inf
    DP[1:, 0] = np.inf

    # Compute minimal cost
    for i in range(1, n):
        for j in range(1, m):

            # Deletion in A
            del_a = (
                DP[i - 1, j]
                + Dlp(A[i - 1], A[i], p=degree)
                + nu * (timeSA[i] - timeSA[i - 1])
                + lmbda
            )

            # Deletion in B
            del_b = (
                DP[i, j - 1]
                + Dlp(B[j - 1], B[j], p=degree)
                + nu * (timeSB[j] - timeSB[j - 1])
                + lmbda
            )

            # Keep data points in both time series
            match = (
                DP[i - 1, j - 1]
                + Dlp(A[i], B[j], p=degree)
                + Dlp(A[i - 1], B[j - 1], p=degree)
                + nu * (abs(timeSA[i] - timeSB[j]) + abs(timeSA[i - 1] - timeSB[j - 1]))
            )

            # Choose the operation with the minimal cost and update DP Matrix
            DP[i, j] = min(del_a, del_b, match)

    distance = DP[n-1, m-1]
    return distance, DP

def backtracking(DP):
    # [ best_path ] = BACKTRACKING ( DP )
    # Compute the most cost efficient path
    # DP := DP matrix of the TWED function

    x = np.shape(DP)
    i = x[0] - 1
    j = x[1] - 1

    # The indices of the paths are save in opposite direction
    # path = np.ones((i + j, 2 )) * np.inf;
    best_path = []

    steps = 0
    while i != 0 or j != 0:
        best_path.append((i - 1, j - 1))

        C = np.ones((3, 1)) * np.inf

        # Keep data points in both time series
        C[0] = DP[i - 1, j - 1]
        # Deletion in A
        C[1] = DP[i - 1, j]
        # Deletion in B
        C[2] = DP[i, j - 1]

        # Find the index for the lowest cost
        idx = np.argmin(C)

        if idx == 0:
            # Keep data points in both time series
            i = i - 1
            j = j - 1
        elif idx == 1:
            # Deletion in A
            i = i - 1
            j = j
        else:
            # Deletion in B
            i = i
            j = j - 1
        steps = steps + 1

    best_path.append((i - 1, j - 1))

    best_path.reverse()
    return best_path[1:]
