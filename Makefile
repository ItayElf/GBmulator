OUTDIR=out
OUT=$(OUTDIR)/GBmulator.exe
CC=g++
ODIR=obj
SDIR=src
CFLAGS=-w -Werror

_OBJS = cpu/registers.o \
		cpu/memory_bus.o \
		cpu/cpu.o \
		main.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	@IF not exist "$(@D)" mkdir "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OUT): $(OBJS)
	@IF not exist "$(@D)" mkdir "$(@D)"
	$(CC) $(CFLAGS) -g $^ -o $@

clean:
	rd /s /q $(OUTDIR)
	rd /s /q $(ODIR)