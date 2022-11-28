CFLAGS ?= -O1 -Wall -Wextra

survival_demo: survival_demo.c slice.c ../survival.c
	$(CC) -o $@ -DSURVIVAL_DEBUG -I.. $^
