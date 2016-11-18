crossValidate=function(train,nFolds){
  
  dat<-dat[sample(nrow(dat)),]
  
  folds <- cut(seq(1,nrow(dat)),breaks=nFolds,labels=FALSE)
  
  testDatas = list()
  #Perform k fold cross validation
  g = rep(0,nrow(train))
  for (i in 1:nFolds){
    print("hello")
    ind = which(folds == i)
    print(head(ind))
    model = trainBoost(dat[-ind,])
    g[ind] = testBoost(dat[-ind,],model)
  }
  
  
}