#Loading the Tidyverse Library (https://www.tidyverse.org/)
library(tidyverse)

#Setting the current workind directory
setwd("~/PACS/apc")

#Reading the dataset
results <- read_csv("./test/data_games.csv", col_names = TRUE)

#Creating the Nim dataset
nim <- results %>%
  filter(GAME == "nim") %>%
  select(-c(GAME, N_MATCH, N_DRAW))

#Plotting for the first Nim player
nim %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_point(mapping = aes(x = IN_ITER, y = N_WIN), alpha = 0.80, position = "jitter") +
  facet_wrap(~ OUT_ITER)

oxo <- results %>%
  filter(GAME == "oxo")
