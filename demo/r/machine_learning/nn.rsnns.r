library(RSNNS)
data(iris)
dim(iris)
names(iris)

#shuffle the vector
iris <- iris[sample(1:nrow(iris),length(1:nrow(iris))),1:ncol(iris)]
dim(iris)
names(iris)
irisValues <- iris[,1:4]
irisTargets <- decodeClassLabels(iris[,5])
#irisTargets <- decodeClassLabels(iris[,5], valTrue=0.9, valFalse=0.1)

iris <- splitForTrainingAndTest(irisValues, irisTargets, ratio=0.15)
names(iris)
iris$inputsTest
## iris$(names(iris)[4])
## 将数据归一化, 默认是归一化 type = "norm", 也就是化为均值为0, 方差为1
iris <- normTrainingAndTestSet(iris)

model <- mlp(iris$inputsTrain, iris$targetsTrain, size=5, learnFuncParams=c(0.1), maxit=50, inputsTest=iris$inputsTest, targetsTest=iris$targetsTest)
summary(model)
model

weightMatrix(model)
extractNetInfo(model)
par(mfrow=c(2,2))
plotIterativeError(model)

predictions <- predict(model,iris$inputsTest)
plotRegressionError(predictions[,2], iris$targetsTest[,2])
confusionMatrix(iris$targetsTrain,fitted.values(model))
confusionMatrix(iris$targetsTest,predictions)
plotROC(fitted.values(model)[,2], iris$targetsTrain[,2])
plotROC(predictions[,2], iris$targetsTest[,2])
#confusion matrix with 402040-method
confusionMatrix(iris$targetsTrain, encodeClassLabels(fitted.values(model), method="402040", l=0.4, h=0.6))
