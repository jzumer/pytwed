# Time Warped Edit Distance (TWED)

This is a python wrapper for Marteau's implementation of this 2009 paper [1] introducing the Time Warped Edit Distance (TWED, also TWE Distance), slightly modified and provided here. TWED is an elastic distance metric for comparing time series sequences that has been shown to be a promising contender for the popular Dynamic Time Warping (DTW) distance and edit distances that have been adjusted to work on real numbers, such as the Edit Distance on Real Sequences (EDR) and Edit Distance with Real Penalty (ERP) [1, 2]. Like the edit distances, it comprises a penalty for mismatches, and, like DTW, it allows to control for _elasticity_. In contrast to DTW and the edit distance measures, it is a proper distance _metric_, as it fulfills the _triangle inequality_. Like DTW and the edit distances, TWED has a time complexity of O(mn), where m and n are the lengths of the time series sequences compared. Hence, it may not be appropriate for very long sequences and large time series databases. However, the fulfillment of the metric properties allows for using some established speed-up tricks in down-stream algorithms, like nearest neighbor retrieval, that can compensate for the complexity. 

## Install

### Using pip

The module can be installed by running the following command in the directory with the In the path of `setup.py`

```bash
pip install .
```

numpy needs to be installed as the only requirement.

### Using build script

The `example_build.sh` provides an example command to build the sources without pip. 

## Usage

The module can then be loaded and directly used as follows:

```python
import pytwed
import numpy as np

seq1 = np.array([[1.0], [2.1], [3.2], [4.3], [5.4]]).astype('float64')
seq2 = np.array([[9.8], [7.6], [6.5], [4.3], [2.1]]).astype('float64')
ts1 = np.array([1., 2., 3., 4., 5.]).astype('float64')
ts2 = ts1.copy()
pytwed.twed(seq1, seq2, ts1, ts2, nu = 0.001, lmbda = 1.0, degree = 2)
```

Sequences `seq1` and `seq2` should be of type `numpy.array` with shapes (m, d) and (n, d), where m and n are the lengths of the sequences and d is the dimensionality of the sequences. The TWED must be provided with time stamps `ts1` and `ts2` with type `numpy.array`. For equally spaced sequences, they may be set to `ts1=np.arange(len(seq1))` and `ts2=np.arange(len(seq2))`. 

The distance function has three parameters that can be chosen/tuned. Parameter `degree` refers to the L^p norm that is used for the distance calculation locally for individual elements. For `degree=2`, the Euclidean norm is used, and it is a useful default. Parameter `nu` (should be > 0, default: 0.001) is a non-negative constant which characterizes the _stiffness_ of the elastic TWED measure. Parameter `lmbda` (should be >= 0, default: 1.0) is a constant penalty that punishes the editing efforts. The parameters should be tuned for each individual problem and dataset using training data minimizing your task's objective on a hold-out set. Parameter grids to try are for instance `nu in [1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1]` and `lmbda in [0, .25, .5, .75, 1.0]` (see [1] for more details).

## Liability

We are providing the code "as is" without a warranty of any kind, with the hope that it can be useful to someone without maintenance goals.

## References

[1] Marteau, Pierre-François. "Time warp edit distance with stiffness adjustment for time series matching." IEEE transactions on pattern analysis and machine intelligence 31.2 (2008): 306-318. Url: [https://arxiv.org/ftp/cs/papers/0703/0703033.pdf](https://arxiv.org/ftp/cs/papers/0703/0703033.pdf).

[2] Serra, Joan, and Josep Ll Arcos. "An empirical evaluation of similarity measures for time series classification." Knowledge-Based Systems 67 (2014): 305-314.