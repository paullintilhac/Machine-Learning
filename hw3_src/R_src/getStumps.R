getStumps = function(dat,D){
  #print("distribution sum = 1?")
  #print(sum(D)==1)
  bestAccuracies = rep(0,ncol(dat)-1)
  bestThresholds = rep(0,ncol(dat)-1)
  bestSigns = rep(-1,ncol(dat)-1)
  bestRowInds = rep(0,ncol(dat)-1)
  for (i in 1:(ncol(dat)-1)){
    #print(i)
    component = as.matrix(cbind(dat$response,dat[[i+1]],rep(-1,nrow(dat)),D,seq(1,nrow(dat))))
    component = component[order(component[,2]),]  
    threshold=component[1,2]
    bestAccuracy = abs(weighted.mean(component[,1]==-1,component[,4]))
    accuracy = bestAccuracy
    bestThreshold = component[1,2]
    bestSign = sign(bestAccuracy)
    bestRowInd = 1
    if (FALSE){
    component=cbind(component,  (2*(component[,2]<=.039)-1) * Sgn)
    component = cbind(component,component[,5]==dat$response)
    component=component[order(component[,5]),]
   # print("weighted mean")
    newH = component[,5]
    weighted.mean(component[,3] == component[,1],component[,4])
    #print(paste0("wm: ",wm))
    }
    for (j in 1:nrow(dat)){
      #print(component[j,2])
      
      component[j,3]=1
      if (component[j,1]==1){
        accuracy=accuracy+component[j,4]
      } else{
        accuracy=accuracy-component[j,4]
      }
      #accuracy2 = weighted.mean(component[,3]==component[,1], component[,4])
      #print(c("accuracy: ",accuracy,", accuracy2: ",accuracy2))
      if (j<nrow(dat))
      if (component[j,2]==component[j+1,2] )
        next
      if (abs(accuracy-.5)>=abs(bestAccuracy-.5)){
        
        #print(paste0(j,"new best accuracy"))
        bestAccuracy = accuracy
        bestThreshold =component[j,2]
        if (j==nrow(component))
          bestThreshold=bestThreshold
        bestSign = sign(accuracy-.5)
        bestRowInd = j
      }
      
    }
    
    bestAccuracies[i]=bestAccuracy
    bestThresholds[i]=bestThreshold
    bestSigns[i] = bestSign
    bestRowInds[i]=bestRowInd
    
  }
  return(cbind(bestAccuracies,bestThresholds,bestSigns))
}