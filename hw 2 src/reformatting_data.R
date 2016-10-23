# first run the following command in bash:
#wget -O ./spambase.csv http://www.cs.nyu.edu/~mohri/ml16/spambase.data.shuffled"
#then:
dat = read.csv("~/Machine Learning/hw 2 src/spambase.csv",header = F)
library(data.table)
dat = data.table(dat)
responses = dat[[58]]
dat[[58]]=NULL
dat = cbind(responses,dat)
#for (i in 1:57){
#  dat[[i+1]] = paste0(i,":",dat[[i+1]])
#}
write.table(dat,
            file = "~/Machine Learning/hw 2 src/spambase.csv",
            row.names=F,
            col.names=F,
            sep = ",",
            quote = F)

training = dat[1:3450,]
testing = dat[3451:nrow(dat),]
write.table(training,
            file = "~/Machine Learning/hw 2 src/training.csv",
            row.names=F,
            col.names=F,
            sep=",",
            quote = F)

write.table(testing,
            file = "~/Machine Learning/hw 2 src/testing.csv",
            row.names=F,
            col.names =F,
            sep=",",
            quote = F)
#training= read.csv("~/Machine Learning/hw 2 src/training.csv")
