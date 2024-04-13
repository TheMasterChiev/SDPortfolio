banaan <- read.csv("DS35A_ID.csv")

png(filename = "/home/RPlays/ClusterResults/DS3AElbow.png")
# Elbow method
fviz_nbclust(banaan, kmeans, method = "wss") +
  geom_vline(xintercept = 4, linetype = 2)+
  labs(subtitle = "Elbow method")
dev.off()

png(filename = "/home/RPlays/ClusterResults/DS3ASilho.png")
# Silhouette method
fviz_nbclust(banaan, kmeans, method = "silhouette")+
  labs(subtitle = "Silhouette method")
dev.off()


png(filename = "/home/RPlays/ClusterResults/DS37AGap.png")
# Gap statistic
# nboot = 50 to keep the function speedy. 
# recommended value: nboot= 500 for your analysis.
# Use verbose = FALSE to hide computing progression.
set.seed(123)
fviz_nbclust(banaan4, kmeans, nstart = 25,  method = "gap_stat", nboot = 500)+
  labs(subtitle = "Gap statistic method")
dev.off()
____________
# Gap statistic
# nboot = 50 to keep the function speedy. 
# recommended value: nboot= 500 for your analysis.
# Use verbose = FALSE to hide computing progression.

library(magrittr)
library(NbClust)
library(cluster)
library(factoextra)
png(filename = "/home/RPlays/ClusterResults/DS35A_Gap.png")
set.seed(123)

# aardt <- kmeans(banaan4, centers = 4, nstart = 25, iter.max = 20)

MyKmeansFUN <- function(x,k) kmeans(x, k, nstart = 25, iter.max=50) 
#DIT WERKT

fviz_nbclust(banaan, MyKmeansFUN, method = "gap_stat", k.max = 15, nboot = 500)+
  labs(subtitle = "Gap Stat method: DS35A_ID.csv")

dev.off()
