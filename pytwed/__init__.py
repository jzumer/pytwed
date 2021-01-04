from twed import twed as ctwed
from .slow_twed import twed as pytwed
import numpy as np


def twed(s1, s2, ts1=None, ts2=None, lmbda=1.0, nu=0.001, p=2, fast=True):
    """
    Time Warped Edit Distance (TWED)

    Parameters
    ----------
    s1 : np.ndarray
        First time series
    s2 : np.ndarray
        Second time series
    ts1 : np.ndarray, default: None
        Time stamps of first time series. If None, then equally spaced time stamps are assumed.
    ts2 : np.ndarray, default: None
        Time stamps of second time series. If None, then equally spaced time stamps are assumed.
    lmbda: float >= 0, default: 1.0
        A constant penalty that punishes the editing efforts
    nu: float > 0, default: 0.001
        A non-negative constant which characterizes the stiffness of the elastic TWED measure.
    p: int
        Order of the p-norm for local cost.
    fast: boolean, default: True
        If true, uses fast C implementation, if False uses Python reference implementation. Default "True" should usually be used, as it is typically magnitutes faster. 

    Returns
    -------
    float
        twe distance

    """
    # Check if input arguments
    if (ts1 is not None) and (len(s1) != len(ts1)):
        raise ValueError("The length of s1 is not equal length of ts1.")

    if (ts2 is not None) and (len(s2) != len(ts2)):
        raise ValueError("The length of s2 is not equal length of ts2.")

    if ts1 is None:
        ts1 = np.arange(len(s1))

    if ts2 is None:
        ts2 = np.arange(len(s2))

    if nu <= 0.:
        raise ValueError("nu must be > 0.")

    if lmbda < 0:
        raise ValueError("lmbda must be >= 0.")

    s1 = np.vstack(([[0] * s1.shape[1]], s1))
    ts1 = np.hstack(([0], ts1))
    s2 = np.vstack(([[0] * s2.shape[1]], s2))
    ts2 = np.hstack(([0], ts2))

    if fast:
        return ctwed(arr1=s1, arr2=s2, arr1_spec=ts1, arr2_spec=ts2, nu=float(nu), lmbda=float(lmbda), degree=p)
    else:
        return pytwed(A=s1, timeSA=ts1, B=s2, timeSB=ts2, nu=nu, lmbda=lmbda, degree=p)[0]
