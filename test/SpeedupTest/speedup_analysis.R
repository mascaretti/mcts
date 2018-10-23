# Initialization ---------------
setwd("./PACS/virtual/apc")
library(tidyverse)

# Nim -------------
nim <- read_csv(file = "./test/SpeedupTest/speedup_nim.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
nim <- nim %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(nim, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_smooth(se = TRUE) +
  labs(title = "Elapsed Time for number of simulations - Nim (0 is serial)")

ggsave(filename = "./test/SpeedupTest/speedup_nim.png", height = 10, width = 10)

# Oxo -------------------------------

oxo <- read_csv(file = "./test/SpeedupTest/speedup_oxo.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
oxo <- oxo %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(oxo, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_point(alpha = 0.2, position = "jitter") +
  geom_smooth(se = TRUE) +
  labs(title = "Elapsed Time for number of simulations - TicTacToe (0 is serial)")

ggsave(filename = "./test/SpeedupTest/speedup_oxo.png")
