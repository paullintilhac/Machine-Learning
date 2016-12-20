trainBoost = function(dat,t){
  
  dists = rep(1/nrow(dat),nrow(dat))
  totalTime= 0
  g = rep(0,nrow(dat))
  gBin = rep(0,nrow(dat))  
  model = list("thresholds" = list(),"indices" = list(),"signs" = list())
  
  for (j in 1:t){
    startTime =Sys.time()
    #print(paste0("iteration: ",j))
    stumps = getStumps(dat,dists)
    #print(paste0(" prob sum top: ",sum(dists)))
    maxAccuracy = max(abs(stumps[,1]-.5))+.5
    maxInd = which(abs(stumps[,1]-.5)+.5==maxAccuracy)
    thresh = stumps[maxInd,2]
    Sgn = stumps[maxInd,3]
    h = (2*(dat[,maxInd+1,with=F]<=thresh)-1) * Sgn
    accuracy = weighted.mean(h==dat$response,dists)
    #print(paste0(c("accuracy: ",accuracy,", maxAccuracy: ",maxAccuracy)))
    a=.5*log((accuracy)/(1-accuracy))
    Z = 2*((1-accuracy)*(maxAccuracy))^.5
   
    g = g+a*h
    gBin = sign(g)
    
        
    dists=dists*exp(-a*dat$response*h)/Z
  
    component = cbind(dat[,maxInd+1],rep(-1,nrow(dat)),dat$response)
    #print(paste0(c("maxInd: ",maxInd,", maxAccuracy: ",maxAccuracy,", real accuracy: ", accuracy)))
   
    model$indices[[j]] = maxInd
    model$thresholds[[j]] = thresh
    model$signs[[j]] = Sgn
    weighted.mean(h==dat$response,dists)
    
    
    sumDist =sum(dists)
    dists = dists/sumDist
    #print(paste0(c("sum(dists): ",sumDist,", train accuracy: ",mean(gBin==dat$response))))
    endTime = Sys.time()
    totalTime = totalTime+endTime-startTime
    avgTime = totalTime/j
    remainingTime = (t-j)*avgTime
  }
 return(model) 
}

testBoost = function(testDat,model){
  gTest = rep(0,nrow(testDat))
  gTestBin = rep(0,nrow(testDat))
  
    for (j in 1:length(model$thresholds)){
      thresh = model$thresholds[[j]]
      maxInd = model$indices[[j]]
      Sgn = model$signs[[j]]
      
      
      hTest =(2*(testDat[,maxInd+1,with=F]<=thresh)-1) * Sgn
      accuracy = mean(hTest = testDat$response)
      a=.5*log((accuracy)/(1-accuracy))
      
      gTest =gTest+hTest*a
      gTestBin = sign(gTest)
    }
  
  
  return(gTestBin)
  #unique(dists)
}