library(e1071)
library(caret)
library(lattice)
dat = read.csv("~/Machine Learning/hw 2 src/training.csv",header = F)


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
  for (j in -10:10){
  for (k in 1:4){
  testIndexes <- which(folds==i,arr.ind=TRUE)
  testData <- dat[testIndexes, ]
  actuals = testData$response
  testData[,response:=NULL]
  trainData <- dat[-testIndexes, ]
  svm.model = svm(x=as.matrix(trainData[,2:58,with = F]),y=trainData[,1,with = F],type = "C-classification",scale = T,kernel = "polynomial",coef0 = 2^j,degree=k)
  svm.predict = predict(svm.model,testData)
  testData$predicted = svm.predict
  testData$actuals = actuals
  testData$c = 2^j
  testData$d=k
  testDatas[[count]] = testData
  count=count+1
  }
  }
}
dat=list("testData" = testData,"plotData" = plotData)
save(dat,file = "~/Machine Learning/rsvmlib.RDATA")
result = rbindlist(testDatas)
plotData = result[,list("error" =mean(actuals!=predicted),
             "sd" =sd(actuals!=predicted)
             ),by = c("c,d")]
plotData$upper_band = plotData$error+plotData$sd
plotData$lower_band = plotData$error-plotData$sd
plotData$logC = log(plotData$c)
xyplot(error+upper_band+lower_band~logC|d,plotData,type="l")
