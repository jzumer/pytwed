from unittest import TestCase
import numpy as np
from numpy.testing import assert_almost_equal
from pytwed import twed



class PyTwedTest(TestCase):

    def test_pytwed(self):
        s1 = np.array([[0.], [0.], [1.], [1.], [2.], [3.], [5.], [2.], [0.], [1.], [-0.1]]).astype('float64')
        s2 = np.array([[0.], [1.], [2.], [2.5], [3.], [3.5], [4.], [4.5], [5.5], [2.], [0.], [0.], [.25], [.05], [0.]]).astype('float64')
        s3 = np.array([[4.], [4.], [3.], [3.], [3.], [3.], [2.], [5.], [2.], [.5], [.5], [.5]]).astype('float64')

        ts1 = np.arange(len(s1))
        ts2 = np.arange(len(s2))
        ts3 = np.arange(len(s3))

        nu = .1
        lmbda = .2
        p = 2

        # those should be the same
        assert_almost_equal(twed(s1, s2, ts1, ts2, nu=nu, lmbda=lmbda, p=p), 22.21)  # was 6.250000000000001 in TWED
        assert_almost_equal(twed(s1, s3, ts1, ts3, nu=nu, lmbda=lmbda, p=p), 22.21)  # was 25.159999999999993 in TWED
        assert_almost_equal(twed(s2, s3, ts2, ts3, nu=nu, lmbda=lmbda, p=p), 20.605) # was 28.529999999999994 in TWED

        assert_almost_equal(twed(s1, s2, ts1, ts2, nu=nu, lmbda=lmbda, p=p, fast=False), 11.899999999999999)
        assert_almost_equal(twed(s1, s3, ts1, ts3, nu=nu, lmbda=lmbda, p=p, fast=False), 16.299999999999994)
        assert_almost_equal(twed(s2, s3, ts2, ts3, nu=nu, lmbda=lmbda, p=p, fast=False), 19.9)

        nu = .001
        lmbda = 1.0
        p = 2

        s4 = np.array([[1.0], [2.1], [3.2], [4.3], [5.4]]).astype('float64')
        s5 = np.array([[9.8], [7.6], [6.5], [4.3], [2.1]]).astype('float64')
        ts4 = np.array([1., 2., 3., 4., 5.]).astype('float64')
        ts5 = np.array([1., 2., 3., 4., 5.]).astype('float64')

         # those should be the same
        assert_almost_equal(twed(s4, s5, ts4, ts5, nu=nu, lmbda=lmbda, p=p), 5.84)
        assert_almost_equal(twed(s4, s5, ts4, ts5, nu=nu, lmbda=lmbda, p=p, fast=False), 24.904)


    # def test_2d(self):
        # test some 2d cases
