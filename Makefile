gFLAGS += --target=thumb
#SFLAGS += -ggdb
SFLAGS += -mthumb
#SFLAGS += -mcpu=cortex-m3
SFLAGS += -mcpu=exynos-m3
PREFIX :=

.PHONY: all clean
.SUFFIXES:

all: main.o

%.o: %.S Makefile
	$(PREFIX)as $(SFLAGS) -c -o $@ $<

%.o: %.c Makefile
	clang $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)
