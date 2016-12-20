library(urca)
library(egcm)
library(cluster)
source("~/Machine Learning/Final Project/getDistanceMatrix.R")
N=10
n=50
classVec = rep(0,N)
X = rnorm(n)
for (i in 1:(N-1)){
  X = cbind(X,rnorm(n))
}

cors = runif(N-1)

#real correlations
for (i in 1:(N-1)){
  X = cbind(X,cors[i]*X[,i]+sqrt(1-cors[i]^2)*X[,i+1])
}
classVec = c(classVec,rep(1,N-1))

#spurious correlations
for (i in 1:(N)){
  X = cbind(X,cumsum(X[,i]))
}

#cointegrated
#X = egcm::rcoint(n)

for (i in 1:(40)){
  X = cbind(X,egcm::rcoint(n))
}

ncols = ncol(X)
#normalize, de-mean
for (i in 1:ncols){
  thisMean =mean(X[,i])
  thisSD = sd(X[,i])
  X[,i] = (X[,i]-thisMean)/thisSD
}

result = getDistanceMatrix(X)
dists = result$dist
penalty = result$penalty
cointMat = result$cointMat
intMat = result$intMat
corMat = result$corMat
lambdas = seq(0,.2,length=10)


for (lambda in lambdas){
  
distMat = 2-2*corMat+lambda*penalty

#nearest neighbors
ind = 5
L=3
topResults = getNearestNeighbors(ind,distMat,L)
plot(X[,ind],type = "o")
for (i in 1:L){
lines(X[,topResults[i]],col=i)  
}

#clustering
K=5
mediods = pam(distMat,K,diss=TRUE)
clusterVec = mediods$clustering


if (FALSE){
##############plot cluster centroids
plot(X[,mediods$id.med[1]],type="l")
for (i in 2:K){
  lines(X[,mediods$id.med[i]],col=i)
}


clus = 5
plot(X[,min(which(clusterVec==clus))],type="l",ylim=c(-15,15))
for (i in 1:ncols){
  if (clusterVec[i]==clus)
    lines(X[,i])
}

}

###############summarize cluster statistics
sumClass3Vec = rep(0,K)
sumClass3Vec2 = rep(0,K)
for (i in 1:K){
  thisVec = which(clusterVec==i)
  count = 0
  ind =mediods$id.med[i]
  for (j in 1:length(thisVec)){  
      count = count+1
      for (k in 1:length(thisVec)){
        sumClass3Vec2[i]=sumClass3Vec2[i]+penalty[thisVec[j],thisVec[i]]
      }
      sumClass3Vec[i] = sumClass3Vec[i] + penalty[thisVec[j],ind]
  }
  #print(count)
}
sumErrors = sum(sumClass3Vec)
sumErrors2 = sum(sumClass3Vec2)
print(paste0(c("lambda: ",lambda,", error with centroid: ",sumErrors,", total in cluster: ",sumErrors2)))
}


####################
topIndices = matrix(0,m,2)
topCategories = matrix(0,m,2)
topCors = rep(-Inf,m)
#how many top results to look at?
m=ncols
diag(corMat) = -Inf

for (i in 1:m){
  thisInd = arrayInd(which.max(corMat),.dim = c(ncols,ncols))
  topIndices[i,] = thisInd
  topCategories[i,] = c(classVec[thisInd[1]],classVec[thisInd[2]])
  topCors = corMat[thisInd]
  corMat[thisInd]= -Inf
  corMat[thisInd[2],thisInd[1]]=-Inf
}




