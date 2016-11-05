getStumps = function(dat){
  bestAccuracies = rep(0,ncol(dat)-1)
  bestThresholds = rep(0,ncol(dat)-1)
  bestSigns = rep(0,ncol(dat)-1)
  for (i in 1:(ncol(dat)-1)){
    print(i)
    component = cbind(dat$response,dat[[i+1]])
    
    component = component[order(component[,2]),]
    threshold=component[1,2]
    bestAccuracy = abs(mean(component[,1]==0)-.5)
    bestThreshold = component[1,2]
    bestSign = sign(mean(component[,1]==0)-.5)
    for (j in 1:(nrow(component)-1)){
      accuracy = abs((sum(component[1:j,1]==1)+sum(component[(j+1):nrow(component),1]==0))/nrow(component)-.5)
      if (accuracy>bestAccuracy){
        bestAccuracy = accuracy
        bestThreshold =component[j,2]
        bestSign = sign((sum(component[1:j,1]==1)+sum(component[(j+1):nrow(component),1]==0))/nrow(component))
      }
    }
    accuracy = abs(mean(component[,1] ==1)-.5)
    if (accuracy >bestAccuracy){
      bestAccuracy = accuracy
      bestThreshold = component[nrow(component),2]+.1
      bestSign = sign(mean(component[,1] ==1))
    }
    bestAccuracies[i]=bestAccuracy
    bestThresholds[i]=bestThreshold
    bestSigns[i] = bestSign
  }
  return(cbind(bestAccuracies,bestThresholds,bestSigns))
}