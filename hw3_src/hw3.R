library(e1071)
library(caret)
library(lattice)
source("~/Machine Learning/hw3_src/getStumps.R")
dat = read.csv("~/Machine Learning/hw_2_src/training.csv",header = F)
testDat = read.csv("~/Machine Learning/hw_2_src/testing.csv",header = F)
setnames(testDat, "V1","response")
testDat = data.table(testDat)
testDat<-testDat[sample(nrow(testDat)),]
testDat$response = as.factor(testDat$response)
setnames(dat, "V1","response")
dat = data.table(dat)


dists = rep(1/nrow(dat),nrow(dat))
T=10

totalTime
  for (j in 1:T){
    startTime =Sys.time()
    print(paste0("iteration: ",j))
    stumps = getStumps(dat,dists)
    maxAccuracy = max(stumps[,1])
    maxInd = which(stumps[,1]==maxAccuracy)
    thresh = stumps[maxInd,2]
    Sgn = stumps[maxInd,3]
    a=.5*log((1-maxAccuracy-.5)/(maxAccuracy+.5))
    Z = 2*((1-maxAccuracy-.5)*(maxAccuracy+.5))^.5
    for (k in 1:nrow(dat)){
      if (Sgn==1){
        thisH = as.numeric(dat[k,maxInd+1]<=thresh)
      }else{
        thisH = as.numeric(dat[k,maxInd+1]>thresh)
      }
    
      dists[k]=dists[k]*exp(-a*dat$responses[k]*thisH)/Z
    }
    endTime = Sys.time()
    totalTime = totalTime+endTime-startTime
    avgTime = totalTime/j
    remainingTime = (T-j)*avgTime
    print(paste0("remaining time: ",remainingTime))
  }
