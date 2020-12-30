# Time Warped Edit Distance

This is a python wrapper for Marteau's implementation of this 2009 paper introducing TWED [1] (slightly modified and provided here). TWED is probably distance metric for time series data that has been shown to be a promising contender the popular DTW distance when comparing time series sequences.

## Install

### Using pip

The module can be installed by running the following command in the directory with the In the path of `setup.py`

```bash
pip install .
```

### Using build script

The `example_build.sh` provides an exemplary command to build the sources without pip. 

## Usage

The module can then be loaded directly like so:

```python
import twed
import numpy
seq1 = numpy.array([[1.0], [2.1], [3.2], [4.3], [5.4]]).astype('float64')
seq2 = numpy.array([[9.8], [7.6], [6.5], [4.3], [2.1]]).astype('float64')
ts1 = numpy.array([1., 2., 3., 4., 5.]).astype('float64')
ts2 = ts1.copy()
twed.twed(seq1, seq2, ts1, ts2)
```

I'm uploading this in case it can be useful to someone, but this isn't exactly a well-structured project with maintenance goals.

### References

[1] Marteau, Pierre-François. "Time warp edit distance with stiffness adjustment for time series matching." IEEE transactions on pattern analysis and machine intelligence 31.2 (2008): 306-318. Url: [https://arxiv.org/ftp/cs/papers/0703/0703033.pdf](https://arxiv.org/ftp/cs/papers/0703/0703033.pdf).