
dat = read.csv("~/Downloads/average_train_errors.txt")
dat = t(dat)
nam = rownames(dat)
dat = data.table(dat)
dat$C = nam
dat$C = substr(dat$C,2,nchar(dat$C))
dat$exp = lapply(strsplit(dat$C,split=".",fixed = T), function(x) x[3])
dat$C = substr(dat$C,1,nchar(dat$C)-4)
dat$C = as.numeric(dat$C)
dat$exp = as.numeric(dat$exp)
dat$newC = dat$C*10^dat$exp
names(dat)[1:3] = c("error","upper_band","lower_band")
dat$error=  as.numeric(dat$error)
dat$upper_band = as.numeric(dat$upper_band)
dat$lower_band = as.numeric(dat$lower_band)

dat = read.csv("~/Machine Learning/results.csv")
setnames(dat,c("C","Degree","accuracy","SD"))
dat$C = as.numeric(dat$C)
dat$logC = log2(dat$C)
dat$upper_band = dat$accuracy+dat$SD
dat$lower_band = dat$accuracy-dat$SD
dat = data.table(dat)
plotData = dat[,list("error" =mean(accuracy),
                "upper_band"=mean(accuracy)+sd(accuracy),
                "lower_band" = mean(accuracy-sd(accuracy))),
                by = "C"]
dat = dat[order(dat$C)]

xyplot(accuracy+upper_band+lower_band~logC|Degree,ylim=c(0,20),dat,type ="l",auto.key=T)

unique(dat$logC)  