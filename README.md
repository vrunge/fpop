devtools::install_github("vrunge/fpop", force = TRUE)

library(fpop)

n <- 1000

beta = 2 * log(2*n)

### Change in mean

y <- c(rnorm(n), rnorm(n,1))

fpop(vectData = y, penalty = beta, change = "mean")

### Change in variance

y <- c(rnorm(n,2), rnorm(n, 2, 1.5))

fpop(vectData = y, penalty = beta, change = "variance")

### Poisson loss

y <- c(rpois(n, lambda = 1), rpois(n, lambda = 2))

fpop(vectData = y, penalty = beta, change = "poisson")

### Exponential loss

y <- c(rexp(n, rate = 1), rexp(n, rate = 2))

fpop(vectData = y, penalty = beta, change = "exp")

