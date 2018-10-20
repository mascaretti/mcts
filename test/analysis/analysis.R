#INITIALISATION -----------------------------
#Loading the Tidyverse Library (https://www.tidyverse.org/)
library(tidyverse)

#Setting the current workind directory
setwd("~/PACS/virtual/apc")

#Reading the dataset
results <- read_csv("./test/data_games.csv", col_names = TRUE)

#NIM----------------------------------------
#Creating the Nim dataset
nim <- results %>%
  filter(GAME == "nim") %>%
  select(-c(GAME, N_MATCH, N_DRAW))

#Victories of random player
nim_ran_victories <- results %>%
  filter(GAME == "nim_rand") %>%
  select(N_WIN) %>%
  as.numeric()

#Plotting for the first Nim player
nim %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = nim_ran_victories, linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 1 at Nim")

#Save plot
#ggsave("./test/analysis/nim_01.png", width = 5, height = 5)


#Plotting for the second Nim player
nim %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = nim_ran_victories, linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Nim")

#Save plot
#ggsave("./test/analysis/nim_02.png", width = 5, height = 5)

#OXO-------------------------------

#Create the oxo dataset
oxo <- results %>%
  filter(GAME == "oxo") %>%
  mutate(N_LOSS = N_MATCH - (N_WIN + N_DRAW)) %>%
  select(-c(GAME))

#Compute results of the random player
oxo_ran_victories <- results %>%
  filter(GAME == "oxo_rand") %>%
  mutate(N_LOSS = N_MATCH - (N_WIN + N_DRAW)) %>%
  select(c(N_WIN, N_DRAW, N_LOSS)) %>%
  as.vector()

#Plotting for the first Oxo player
oxo %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[1]), linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 1 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_win_01.png", width = 5, height = 5)

#draws
oxo %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_DRAW)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[2]), linetype = "dashed", colour = "blue") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 1 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_draw_01.png", width = 5, height = 5)


#Plotting for the second Oxo player
oxo %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[1]), linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Tic-tac-toe")

#Saving the plot
ggsave("./test/analysis/oxo_win_02.png", width = 5, height = 5)

#draws
oxo %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_DRAW)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[2]), linetype = "dashed", colour = "blue") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Tic-tac-toe")

#Saving the plot
ggsave("./test/analysis/oxo_draw_02.png", width = 5, height = 5)

