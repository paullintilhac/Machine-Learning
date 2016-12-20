getDistanceMatrix = function(X){
  ncols = ncol(X)
  
 
  
  intMat = matrix(0,ncols,ncols)
  cointMat = matrix(0,ncols,ncols)
  
  for (i in 1:ncols){
    for (j in 1:ncols){
      xTest = ur.df(X[,i],type=  "none")
      yTest = ur.df(X[,j],type="none")
      intMat[i,j] = ifelse(as.numeric(xTest@teststat)>xTest@cval[3] & as.numeric(yTest@teststat)>yTest@cval[3],1,0)
      df = data.frame(cbind(X[,i],X[,j]))
      yModel = lm(X2 ~ X1, data = df)
      r1 = yModel$residuals
      xModel = lm(X1 ~ X2, data = df)
      r2 = xModel$residuals
      r1Test= ur.df(r1,type="none")
      r2Test = ur.df(r2,type="none")
      combinedStat = as.numeric(r1Test@teststat)+ as.numeric(r2Test@teststat)
      combinedCval = r1Test@cval[1]+r2Test@cval[1]
      cointMat[i,j]= ifelse(combinedStat<combinedCval,1,0)
    }
  }
  
  classMat = matrix(0,ncols,ncols)
  classMat[which(intMat==0)]=1 #regular correlation
  classMat[intersect(which(intMat==1),which(cointMat==0))]=2
  classMat[intersect(which(intMat==1),which(cointMat==1))]=3
  corMat = cor(X)
  class3Mat = matrix(0,ncols,ncols)
  class3Mat[which(classMat==2)]=1
  dists = 2-2*corMat
  
  result = list("dist" = dists,"penalty" = class3Mat,"intMat" = intMat,"cointMat" = cointMat,"corMat"=corMat)
  return(result)
}


getNearestNeighbors = function(ind,distMat,L = 5){
  distances = distMat[,ind]
  perm = order(distances)
  return (perm[2:(L+1)])
}
