<a id="top"></a>

[![Build Status](https://travis-ci.com/vrunge/fpop.svg?branch=master)](https://travis-ci.com/vrunge/fpop)
[![](https://img.shields.io/badge/docs-vignettes-blue.svg)](https://github.com/vrunge/fpop)

<!-- 
%\VignetteEngine{knitr::rmarkdown} 
%\VignetteIndexEntry{An Introduction to gfpop}
--> 

# fpop Vignette
### Vincent Runge
#### LaMME, Evry University
### March 27, 2019

> [Introduction](#intro)

> [Examples](#ex)

<a id="intro"></a>

## Introduction

`fpop` is an `R` package developed in C++ performing parametric changepoint detection in univariate time series with many available cost functions. Changepoint detection uses the functional pruning optimal partitioning method (fpop) based on an exact dynamic programming algorithm.

The package makes available different cost functions to solve problems with change in mean, change in variance, change in a poisson distribution, in an exponential distribution or in a negative binomial distribution.

The package `fpop` is designed to segment univariate data <img src="/tex/b704a970e46e8418f3ef56718438b122.svg?invert_in_darkmode&sanitize=true" align=middle width=126.38913869999998pt height=24.65753399999998pt/>. The changepoint vector <img src="/tex/ed535ca37308856c5f0c6d85b4d4c676.svg?invert_in_darkmode&sanitize=true" align=middle width=209.11492305pt height=27.91243950000002pt/> defines the <img src="/tex/33359de825e43daa97171e27f6558ae9.svg?invert_in_darkmode&sanitize=true" align=middle width=37.38576269999999pt height=22.831056599999986pt/> segments <img src="/tex/97a268c17395aace06ce389334ba5322.svg?invert_in_darkmode&sanitize=true" align=middle width=115.02097364999997pt height=24.65753399999998pt/>, <img src="/tex/59680c09e0e2d723d0bcf2005047b028.svg?invert_in_darkmode&sanitize=true" align=middle width=73.18587374999998pt height=22.831056599999986pt/> with fixed bounds <img src="/tex/370a29c873e3d269a6111aa219085d0b.svg?invert_in_darkmode&sanitize=true" align=middle width=44.697406049999984pt height=21.18721440000001pt/> and  <img src="/tex/afdb85da7c3e7b7c3d226050994dbf5f.svg?invert_in_darkmode&sanitize=true" align=middle width=63.70246739999999pt height=14.15524440000002pt/>. We use the set <img src="/tex/36dd5900c84c0ddd4a48f4858bbd6e8f.svg?invert_in_darkmode&sanitize=true" align=middle width=264.26770754999995pt height=27.91243950000002pt/> to define the minimal global cost given by

<p align="center"><img src="/tex/55e63ede9d3bc1ac8c64fa74af425a24.svg?invert_in_darkmode&sanitize=true" align=middle width=277.1488236pt height=49.315569599999996pt/></p>

where <img src="/tex/99751e94989c68f9be0f6aa442bc80d5.svg?invert_in_darkmode&sanitize=true" align=middle width=40.302373649999986pt height=22.831056599999986pt/> is a penalty parameter and <img src="/tex/a44ff4154fc3bc708e9e752a14051324.svg?invert_in_darkmode&sanitize=true" align=middle width=49.762892849999986pt height=24.65753399999998pt/> is the minimal cost over the segment <img src="/tex/add1478513cabbadcd5004323f01b74c.svg?invert_in_darkmode&sanitize=true" align=middle width=62.71697189999998pt height=24.65753399999998pt/>. The penalty <img src="/tex/8217ed3c32a785f0b5aad4055f432ad8.svg?invert_in_darkmode&sanitize=true" align=middle width=10.16555099999999pt height=22.831056599999986pt/> is understood as an additional cost when introducing a new segment. The argminimum of this quantity gives a vector <img src="/tex/fa1931c6895840344a67aa4df9cf3f59.svg?invert_in_darkmode&sanitize=true" align=middle width=15.782028899999991pt height=22.63846199999998pt/> containing the last position of each segment (if we do not consider <img src="/tex/370a29c873e3d269a6111aa219085d0b.svg?invert_in_darkmode&sanitize=true" align=middle width=44.697406049999984pt height=21.18721440000001pt/>).

In our setting, the cost <img src="/tex/db5f7b3e9934fbc5a2859d88c4ba84a3.svg?invert_in_darkmode&sanitize=true" align=middle width=9.614228249999991pt height=22.465723500000017pt/> is the result of the minimization of a cost function <img src="/tex/20defdf68c7f0df78ac5c7312922f7b7.svg?invert_in_darkmode&sanitize=true" align=middle width=67.62772499999998pt height=24.65753399999998pt/> taking different form depending on the problem we want to solve:

Change in mean:

<p align="center"><img src="/tex/fffcde56d44d0088c63ec7f5f1443c3f.svg?invert_in_darkmode&sanitize=true" align=middle width=158.25197025pt height=18.312383099999998pt/></p>

Change in variance / change in Poisson or exponential distribtion :

<p align="center"><img src="/tex/aaf7c18bae591dd9981618e5d7193d28.svg?invert_in_darkmode&sanitize=true" align=middle width=187.63551345pt height=16.438356pt/></p>

Change in negative binomial distribution:

<p align="center"><img src="/tex/e0d8e1db9ae480cb2ed15260074ac36f.svg?invert_in_darkmode&sanitize=true" align=middle width=265.48935105pt height=16.438356pt/></p>

Considering a unique data point, the cost takes a different form for each problem.

Change in mean:

<p align="center"><img src="/tex/2e6456aeebbb17eaac4d855931e8a3be.svg?invert_in_darkmode&sanitize=true" align=middle width=167.73038369999998pt height=18.312383099999998pt/></p>

Change in variance :

<p align="center"><img src="/tex/355b2841c20631d746ffa6a05b4a41aa.svg?invert_in_darkmode&sanitize=true" align=middle width=158.23657244999998pt height=18.312383099999998pt/></p>

In this case, data a centered and data points <img src="/tex/aeeadff394a47eb25004f564c74ed751.svg?invert_in_darkmode&sanitize=true" align=middle width=43.66906829999999pt height=21.18721440000001pt/> are replaced by <img src="/tex/31dfd4fcc5fae583745a58bb17f8a473.svg?invert_in_darkmode&sanitize=true" align=middle width=42.12224939999999pt height=14.15524440000002pt/> with <img src="/tex/6f0d4c93d5e4c97f2a254622a0c64540.svg?invert_in_darkmode&sanitize=true" align=middle width=68.40755624999998pt height=26.76175259999998pt/>. The algorithm returns <img src="/tex/bdf4d2f04320c1f3fd24b8115a106321.svg?invert_in_darkmode&sanitize=true" align=middle width=51.287539049999985pt height=27.77565449999998pt/>, the standard deviation which is into the range of values <img src="/tex/2546a5b3fd7ffe4a3bb8929f05031c3f.svg?invert_in_darkmode&sanitize=true" align=middle width=52.33455975pt height=24.65753399999998pt/>.


Change in Poisson distribution :

<p align="center"><img src="/tex/eb65fec4d6b7f9399b63a6e2dc9f5199.svg?invert_in_darkmode&sanitize=true" align=middle width=153.60177195pt height=16.438356pt/></p>

Change in exponential distribution :

<p align="center"><img src="/tex/790783d8e4cfa7d3bc5fb83a6a30bde9.svg?invert_in_darkmode&sanitize=true" align=middle width=150.8621136pt height=16.438356pt/></p>

with the restriction that <img src="/tex/835160c38364cb49de65a8f867bd01a7.svg?invert_in_darkmode&sanitize=true" align=middle width=43.66906829999999pt height=21.18721440000001pt/> for all data points.

Change in negative binomial distribution:

<p align="center"><img src="/tex/7bc93e7a79b8cb10987e37e3d1cdde35.svg?invert_in_darkmode&sanitize=true" align=middle width=212.76086039999998pt height=16.438356pt/></p>

To use this cost we need to estimate the dispersion parameter <img src="/tex/f50853d41be7d55874e952eb0d80c53e.svg?invert_in_darkmode&sanitize=true" align=middle width=9.794543549999991pt height=22.831056599999986pt/> and reduce the data <img src="/tex/c36ec8a3091d8d3161307d4776dc5f2c.svg?invert_in_darkmode&sanitize=true" align=middle width=60.88276589999998pt height=24.65753399999998pt/>.
The points <img src="/tex/aeeadff394a47eb25004f564c74ed751.svg?invert_in_darkmode&sanitize=true" align=middle width=43.66906829999999pt height=21.18721440000001pt/> are replaced by <img src="/tex/58de7d1f63716d08fa779ae10f0e2bd9.svg?invert_in_darkmode&sanitize=true" align=middle width=59.72793914999999pt height=22.853275500000024pt/> to constrain the probability <img src="/tex/2ec6e630f199f589a2402fdf3e0289d5.svg?invert_in_darkmode&sanitize=true" align=middle width=8.270567249999992pt height=14.15524440000002pt/> into <img src="/tex/4d43f62a3d7f50890bf044f4d77532c5.svg?invert_in_darkmode&sanitize=true" align=middle width=59.64033239999999pt height=24.65753399999998pt/>.

The estiamation of <img src="/tex/f50853d41be7d55874e952eb0d80c53e.svg?invert_in_darkmode&sanitize=true" align=middle width=9.794543549999991pt height=22.831056599999986pt/> is given by

<p align="center"><img src="/tex/7cc02b62f5d162cd1980e19b9020fe3d.svg?invert_in_darkmode&sanitize=true" align=middle width=81.03820724999999pt height=38.973783749999996pt/></p>

with <img src="/tex/fb2c407771af04095047a75aab1127e2.svg?invert_in_darkmode&sanitize=true" align=middle width=9.90492359999999pt height=22.831056599999986pt/> and <img src="/tex/d9199ce48a4cddb329cead7e224c336e.svg?invert_in_darkmode&sanitize=true" align=middle width=9.98290094999999pt height=22.831056599999986pt/> the estimated mean and standard deviation over all the data. This kind of estimation is based on the hypothesis that most datapoints are generated by same negative binomial distribution.


<a id="ex"></a>

## Examples

We install the package from Github:

```r
#devtools::install_github("vrunge/fpop")
library(fpop)
```

We choose a data length and a penalty value

```r
n <- 2000
beta = 2 * log(n)
```

We illustrate the use of the fpop function with all the available cost functions on simple simulated data

### Change in mean


```r
y <- c(rnorm(n/2), rnorm(n/2, 1))
fpop(vectData = y, penalty = beta, change = "mean")
```

```
## changepoints
## [1]  988 2000
## 
## parameters
## [1] 0.01903607 0.92953069
## 
## globalCost
## [1] 2102.193
```

### Change in variance


```r
y <- c(rnorm(n/2, 0, 0.01), rnorm(n/2, 0, 1))
fpop(vectData = y, penalty = beta, change = "variance")
```

```
## changepoints
## [1]  999 2000
## 
## parameters
## [1] 0.01789684 0.98942122
## 
## globalCost
## [1] -6044.306
```

### Poisson distribution


```r
y <- c(rpois(n/2, lambda = 1), rpois(n/2, lambda = 1.4))
fpop(vectData = y, penalty = beta, change = "poisson")
```

```
## changepoints
## [1] 1021 2000
## 
## parameters
## [1] 0.9598433 1.3810010
## 
## globalCost
## [1] 1950.93
```


### Exponential distribution


```r
y <- c(rexp(n/2, rate = 10), rexp(n/2, rate = 0.15))
fpop(vectData = y, penalty = beta, change = "exp")
```

```
## changepoints
## [1] 1001 2000
## 
## parameters
## [1] 10.1460729  0.1430807
## 
## globalCost
## [1] 1638.2
```


### Negative binomial distribution


```r
library(MASS)
theta <- 1
mu1 <- 10
mu2 <- 1
##p <- 1/(theta/mu + 1)
1/((theta/mu1) + 1)
```

```
## [1] 0.9090909
```

```r
1/((theta/mu2) + 1)
```

```
## [1] 0.5
```

```r
y <- c(rnegbin(n/2, mu = mu1, theta = theta), rnegbin(100, mu = mu2, theta = theta), rnegbin(n/2 - 100, mu = mu1, theta = theta))
fpop(vectData = y, penalty = beta/2, change = "negbin")
```

```
## changepoints
## [1] 1002 1100 2000
## 
## parameters
## [1] 0.9180274 0.4916996 0.9171733
## 
## globalCost
## [1] 6718.507
```

[Back to Top](#top)
