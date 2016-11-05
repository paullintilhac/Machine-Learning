library(e1071)
library(caret)
library(lattice)
dat = read.csv("~/Machine Learning/hw_2_src/training.csv",header = F)
testDat = read.csv("~/Machine Learning/hw_2_src/testing.csv",header = F)
setnames(testDat, "V1","response")
testDat = data.table(testDat)
testDat<-testDat[sample(nrow(testDat)),]
testDat$response = as.factor(testDat$response)
setnames(dat, "V1","response")
dat = data.table(dat)
dat<-dat[sample(nrow(dat)),]
dat$response = as.factor(dat$response)
#Create 10 equally size folds
folds <- cut(seq(1,nrow(dat)),breaks=10,labels=FALSE)

testDatas = list()
#Perform 10 fold cross validation
count =1
for(i in 1:10){
  for (j in 1){
  for (k in 1:4){
  print(paste0("logc: ",j,", d: ",k))
 
  actuals = testDat$response
  trainData <- data.table(dat)
  for (k in 1:4){
  svm.model = svm(x=as.matrix(trainData[,2:58,with = F]),y=trainData[,1,with = F],type = "C-classification",scale = T,kernel = "polynomial",cost = 2^(2*j),degree=k)
  svm.predict = predict(svm.model,testDat[,2:58,with = F])
  testDat$predicted = svm.predict
  testDat$actuals = actuals
  testDat$c = 2^(2*j)
  testDat$d=k
  testDatas[[count]] = testDat
  count = count+1
  }
  count=count+1
  }
  }
}
dat=list("testDatas" = testDatas,"plotData" = plotData)
save(dat,file = "~/Machine Learning/rsvmlib.RDATA")
result = rbindlist(testDatas)
plotData = result[,list("error" =mean(actuals!=predicted),
             "sd" =(sd(actuals!=predicted))
             ),by = c("c,d")]
plotData$sd = plotData$sd*sqrt(10/nrow(dat))
plotData$upper_band = plotData$error+plotData$sd
plotData$lower_band = plotData$error-plotData$sd
plotData$logC = log2(plotData$c)
xyplot(error+upper_band+lower_band~d,plotData,type="l",auto.key=T)
