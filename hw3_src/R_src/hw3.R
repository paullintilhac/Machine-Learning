library(e1071)
library(caret)
library(lattice)
library(data.table)
source("~/Machine Learning/hw3_src/getStumps.R")
source("~/Machine Learning/hw3_src/boost.R")
source("~/Machine Learning/hw3_src/crossValidate.R")
dat = read.csv("~/Machine Learning/training.csv",header = F)
testDat = read.csv("~/Machine Learning/testing.csv",header = F)
setnames(testDat, "V1","response")
testDat = data.table(testDat)
setnames(dat, "V1","response")
dat = data.table(dat)
testDat[response ==0]$response = -1
dat[response==0]$response = -1

results =data.table(matrix(0,nrow=0,ncol=3))
setnames(results,c("T","cv_accuracy","test_accuracy"))
for (i in 1:4){
  t = 4^i
  print(paste0("t: ",i))
  
  cvAcc = crossValidate(dat,5,t)
  model = trainBoost(dat,t)
  gTest = testBoost(testDat,model)
  testAcc = mean(gTest==testDat$response)
  thing = data.table(t(c(t,cvAcc,testAcc)))
  setnames(thing,c("T","cv_accuracy","test_accuracy"))
  results = rbind(results,thing)
}

write.csv(results,"~/Machine Learning/hw3_results.csv")
