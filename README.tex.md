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

The package `fpop` is designed to segment univariate data $y_{1:n} = \{y_1,...,y_n\}$. The changepoint vector $\overline{\tau} = (\tau_0 < \cdots < \tau_{k+1}) \in \mathbb{N}^{k+2}$ defines the $k+1$ segments $\{\tau_i+1,...,\tau_{i+1}\}$, $i = 0,...,k$ with fixed bounds $\tau_0 = 0$ and  $\tau_{k+1} = n$. We use the set $S_n = \{\hbox{changepoint vector } \overline{\tau} \in \mathbb{N}^{k+2}\}$ to define the minimal global cost given by

$$Q_n = \min_{\overline{\tau} \in S_n}\left[ \sum_{i=0}^{k}\lbrace \mathcal{C}(y_{(\tau_i+1):\tau_{i+1}}) + \beta \rbrace \right]\,,$$

where $\beta > 0$ is a penalty parameter and $\mathcal{C}(y_{u:v})$ is the minimal cost over the segment $\{u,...,v\}$. The penalty $\beta$ is understood as an additional cost when introducing a new segment. The argminimum of this quantity gives a vector $\tau^*$ containing the last position of each segment (if we do not consider $\tau_0 = 0$).

In our setting, the cost $\mathcal{C}$ is the result of the minimization of a cost function $\theta \mapsto C(\theta)$ taking different form depending on the problem we want to solve:

Change in mean:

$$C(\theta) = A \theta^2 + B \theta + C$$

Change in variance / change in Poisson or exponential distribution :

$$C(\theta) = A \theta - B \log(\theta) + C$$

Change in negative binomial distribution:

$$C(\theta) = - A \log(\theta) - B \log(1-\theta) + C$$

Considering a unique data point, the cost takes a different form for each problem.

Change in mean:

$$C(\theta, y) = \theta^2 - 2y \theta + y^2$$

Change in variance :

$$C(\theta, y)  = y^2 \theta - \log(\theta)$$

In this case, data is centered and data points $y_i = 0$ replaced by $y_i = \epsilon$ with $\epsilon = 10^{-12}$. The algorithm returns $\sigma = \frac{1}{\sqrt{\theta}}$, the standard deviation which is now into the range of values $[\epsilon, +\infty[$.


Change in Poisson distribution :

$$C(\theta, y)  = \theta - y \log(\theta)$$

Change in exponential distribution :

$$C(\theta, y)  = y\theta - \log(\theta)$$

with the restriction that $y_i > 0$ for all data points.

Change in negative binomial distribution:

$$C(\theta, y) = - y \log(\theta) - \log(1-\theta)$$

To use this cost we need to estimate the dispersion parameter $\phi$ and to reduce the data $y \leftarrow y / \phi$.
The points $y_i = 0$ are replaced by $y_i = \frac{\epsilon}{1 - \epsilon}$ to constrain the probability $p$ into $[0, 1- \epsilon]$.

The estiamation of $\phi$ is given by

$$\phi = \frac{\hat{\mu}^2}{\hat{\sigma}^2 - \hat{\mu}}$$

with $\hat{\mu}$ and $\hat{\sigma}$ the estimated mean and standard deviation over all the data. This kind of estimation is based on the hypothesis that most datapoints are generated by same negative binomial distribution.


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
