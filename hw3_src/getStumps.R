getStumps = function(dat,D){
  print("distribution sum = 1?")
  print(sum(D)==1)
  bestAccuracies = rep(0,ncol(dat)-1)
  bestThresholds = rep(0,ncol(dat)-1)
  bestSigns = rep(0,ncol(dat)-1)
  for (i in 1:(ncol(dat)-1)){
    print(i)
    component = as.matrix(cbind(dat$response,dat[[i+1]]))
    
    component = component[order(component[,2]),]
    threshold=component[1,2]
    bestAccuracy = abs(weighted.mean(component[,1]==0,D)-.5)
    bestThreshold = component[1,2]
    bestSign = sign(weighted.mean(component[,1]==0,D)-.5)
    for (j in 1:(nrow(component)-1)){
      #print(j)
      accuracy = abs(weighted.mean(xor(component[,1],c(rep(1,j),rep(0,nrow(component)-j)))==0,D)-.5)
      if (accuracy>bestAccuracy){
        bestAccuracy = accuracy
        bestThreshold =component[j,2]
        bestSign = sign(weighted.mean(xor(component[,1],c(rep(1,j),rep(0,nrow(component)-j)))==0,D)-.5)
      }
    }
    accuracy = abs(weighted.mean(component[,1] ==1,D)-.5)
    if (accuracy >bestAccuracy){
      bestAccuracy = accuracy
      bestThreshold = component[nrow(component),2]+.1
      bestSign = sign(weighted.mean(component[,1] ==1,D)-.5)
    }
    bestAccuracies[i]=bestAccuracy
    bestThresholds[i]=bestThreshold
    bestSigns[i] = bestSign
  }
  return(cbind(bestAccuracies,bestThresholds,bestSigns))
}