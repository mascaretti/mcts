# Initialization ---------------
setwd("C:/Users/ANDREA/Documents/PACS/apc/")
library(tidyverse)

height <- 3
width <- 5.5

# Nim -------------
nim <- read_csv(file = "./test/speedup_test/speedup_nim.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
nim <- nim %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(nim, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_point(alpha = 0.2, position = "jitter") +
  geom_smooth(se = TRUE, method = "lm", formula = y ~ x)

ggsave(filename = "./test/speedup_test/plots/speedup_nim.png", height = height, width = width)

# Oxo -------------------------------

oxo <- read_csv(file = "./test/speedup_test/speedup_oxo.csv", col_names = FALSE,
                col_types = list(col_factor(levels = c(0, 1, 2, 3, 4), ordered = TRUE), col_integer(), col_double()))
oxo <- oxo %>%
  rename(size = X1, simulation = X2, elapsed = X3)

ggplot(oxo, mapping = aes(x = simulation, y = elapsed, colour = size)) +
  geom_point(alpha = 0.2, position = "jitter") +
  geom_smooth(se = TRUE, method = "lm", formula = y ~ x)

ggsave(filename = "./test/speedup_test/plots/speedup_oxo.png", height = height, width = width)


# Speed-up --------


grouped_nim <- nim %>%
  group_by(size, simulation) %>%
  summarize(avg = mean(elapsed))
matrix_nim <- as.matrix(grouped_nim)
base_nim <- as.numeric(matrix_nim[1:20, 3])
zero_nim <- base_nim / base_nim
one_nim <- base_nim / as.numeric(matrix_nim[21:40, 3])
two_nim <- base_nim / as.numeric(matrix_nim[41:60, 3])
three_nim <- base_nim / as.numeric(matrix_nim[61:80, 3])
four_nim <- base_nim / as.numeric(matrix_nim[81:100, 3])
speed_nim <- unlist(c(zero_nim, one_nim, two_nim, three_nim, four_nim))

grouped_nim <- add_column(grouped_nim, speed_up = speed_nim)
ggplot(grouped_nim, mapping = aes(x = simulation, y = speed_up, colour = size)) +
  geom_smooth(se = FALSE, method = "lm")
  #geom_smooth(se = FALSE, method = "lm", formula = y ~ x)

ggsave(filename = "./test/speedup_test/plots/nim_speed.png", height = height, width = width)

grouped_oxo <- oxo %>%
  group_by(size, simulation) %>%
  summarize(avg = mean(elapsed))
matrix_oxo <- as.matrix(grouped_oxo)
base_oxo <- as.numeric(matrix_oxo[1:20, 3])
zero_oxo <- base_oxo / base_oxo
one_oxo <- base_oxo / as.numeric(matrix_oxo[21:40, 3])
two_oxo <- base_oxo / as.numeric(matrix_oxo[41:60, 3])
three_oxo <- base_oxo / as.numeric(matrix_oxo[61:80, 3])
four_oxo <- base_oxo / as.numeric(matrix_oxo[81:100, 3])
speed_oxo <- unlist(c(zero_oxo, one_oxo, two_oxo, three_oxo, four_oxo))

grouped_oxo <- add_column(grouped_oxo, speed_up = speed_oxo)
ggplot(grouped_oxo, mapping = aes(x = simulation, y = speed_up, colour = size)) +
  geom_smooth(se = FALSE, method = "lm")

ggsave(filename = "./test/speedup_test/plots/oxo_speed.png", height = height, width = width)
