devtools::install_github("vrunge/fpop", force = TRUE)

library(fpop)

n <- 1000
beta = 2 * log(2*n)

### Change in mean

y <- c(rnorm(n), rnorm(n,1))
fpop(vectData = y, penalty = beta, change = "mean")

### Change in variance

y <- c(rnorm(n,0,0.01), rnorm(n, 0, 1))
fpop(vectData = y, penalty = beta, change = "variance")

### Poisson loss

y <- c(rpois(n, lambda = 1), rpois(n, lambda = 1.4))
fpop(vectData = y, penalty = beta, change = "poisson")

### Exponential loss

y <- c(rexp(n, rate = 10), rexp(n, rate = 0.15))
fpop(vectData = y, penalty = beta, change = "exp")

### Binomial loss

theta <- 1
mu1 <- 10
mu2 <- 1
##p <- 1/(theta/mu + 1)
1/((theta/mu1) + 1)
1/((theta/mu2) + 1)

y <- c(rnegbin(n, mu = mu1, theta = theta), rnegbin(100, mu = mu2, theta = theta), rnegbin(n - 100, mu = mu1, theta = theta))
fpop(vectData = y, penalty = beta/2, change = "negbin")
