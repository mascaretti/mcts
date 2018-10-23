# Initialization ---------------
setwd("./PACS/virtual/apc")
library(tidyverse)

height <- 3
width <- 5.5

# Nim -------------
nim <- read_csv(file = "./test/SpeedupTest/speedup_nim.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
nim <- nim %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(nim, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_point(alpha = 0.2, position = "jitter") +
  geom_smooth(se = TRUE)

ggsave(filename = "./test/SpeedupTest/speedup_nim.png", height = height, width = width)

# Oxo -------------------------------

oxo <- read_csv(file = "./test/SpeedupTest/speedup_oxo.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
oxo <- oxo %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(oxo, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_point(alpha = 0.2, position = "jitter") +
  geom_smooth(se = TRUE)

ggsave(filename = "./test/SpeedupTest/speedup_oxo.png", height = height, width = width)



speedup_nim <- nim %>%
  group_by(size, simulation) %>%
  summarize(value = mean(elapsed)) %>%
  group_by(simulation) %>%
  mutate(speed_up = value[size == 0]/value)

ggplot(data = speedup_nim, mapping = aes(x = simulation, y = speed_up, colour = size)) +
  geom_smooth()
