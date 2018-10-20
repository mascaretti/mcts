#INITIALISATION -----------------------------
#Loading the Tidyverse Library (https://www.tidyverse.org/)
library(tidyverse)
library(forcats)

#Setting the current workind directory
#setwd("~/PACS/virtual/apc")
setwd("~/PACS/apc")

#Reading the dataset
results <- read_csv("./test/data_games.csv", col_names = TRUE)

#setting default values for printing plots
def_width <- 10
def_height <- 10

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
#ggsave("./test/analysis/nim_01.png", width = def_width, height = def_height)

nim %>%
  filter(PLAYER == 1) %>%
  mutate(PERF_INCR = ((N_WIN - nim_ran_victories) / nim_ran_victories)) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = PERF_INCR)) +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Performance increase of Player 1 at Nim")

#Save plot
#ggsave("./test/analysis/nim_01_pi.png", width = def_width, height = def_height)



#Plotting for the second Nim player
nim %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = nim_ran_victories, linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Nim")

#Save plot
#ggsave("./test/analysis/nim_02.png", width = def_width, height = def_height)

nim %>%
  filter(PLAYER == 2) %>%
  mutate(PERF_INCR = ((N_WIN - nim_ran_victories) / nim_ran_victories)) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = PERF_INCR)) +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Performance increase of Player 2 at Nim")

#Save plot
#ggsave("./test/analysis/nim_02_pi.png", width = def_width, height = def_height)


#Checking if the number of wins changes
#considering the different players
nim %>%
  mutate(PLAYER = as.character(PLAYER)) %>%
  mutate(PLAYER = as_factor(PLAYER)) %>%
  mutate(TOT_ITER = IN_ITER * OUT_ITER) %>%
  ggplot(mapping = aes(x = PLAYER, y = N_WIN)) +
  geom_boxplot() +
  geom_jitter(mapping = aes(size = IN_ITER, colour = OUT_ITER), width = 0.2, alpha = 0.8) +
  labs(title = "Boxplot of the number of wins at Nim according to the player")

#Save plot
ggsave("./test/analysis/nim_boxplot.png", width = def_width, height = def_height)

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
  select(c(N_WIN, N_DRAW, N_LOSS))

#Checking if the number of wins/draws changes
#considering the different players
oxo %>%
  mutate(PLAYER = as.character(PLAYER)) %>%
  mutate(PLAYER = as_factor(PLAYER)) %>%
  mutate(TOT_ITER = IN_ITER * OUT_ITER) %>%
  ggplot(mapping = aes(x = PLAYER, y = N_WIN)) +
  geom_boxplot() +
  geom_jitter(mapping = aes(size = IN_ITER, colour = OUT_ITER), width = 0.2, alpha = 0.8) +
  labs(title = "Boxplot of the number of wins at Tic-tac-toe according to the player")

#Save plot
#ggsave("./test/analysis/oxo_boxplot_draws.png", width = def_width, height = def_height)


oxo %>%
  mutate(PLAYER = as.character(PLAYER)) %>%
  mutate(PLAYER = as_factor(PLAYER)) %>%
  mutate(TOT_ITER = IN_ITER * OUT_ITER) %>%
  ggplot(mapping = aes(x = PLAYER, y = N_DRAW)) +
  geom_boxplot() +
  geom_jitter(mapping = aes(size = IN_ITER, colour = OUT_ITER), width = 0.2, alpha = 0.8) +
  labs(title = "Boxplot of the number of draws at Tic-tac-toe according to the player")

#Save plot
#ggsave("./test/analysis/oxo_boxplot_draws.png", width = def_width, height = def_height)

#Plotting for the first Oxo player
oxo %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[1]), linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 1 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_win_01.png", width = def_width, height = def_height)

#draws
oxo %>%
  filter(PLAYER == 1) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_DRAW)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[2]), linetype = "dashed", colour = "blue") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 1 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_draw_01.png", width = def_width, height = def_height)


#Plotting for the second Oxo player
oxo %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_WIN)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[1]), linetype = "dashed", colour = "red") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_win_02.png", width = def_width, height = def_height)

#draws
oxo %>%
  filter(PLAYER == 2) %>%
  ggplot() +
  geom_col(mapping = aes(x = IN_ITER, y = N_DRAW)) +
  geom_hline(yintercept = as.numeric(oxo_ran_victories[2]), linetype = "dashed", colour = "blue") +
  facet_wrap(~ OUT_ITER, labeller = label_both) +
  labs(x = "Number of roll-outs", y = "Number of wins", title = "Wins of Player 2 at Tic-tac-toe")

#Saving the plot
#ggsave("./test/analysis/oxo_draw_02.png", width = def_width, height = def_height)

