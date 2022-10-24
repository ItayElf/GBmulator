OUTDIR=out
OUT=$(OUTDIR)/GBmulator.exe
CC=g++
ODIR=obj
SDIR=src
CFLAGS=-w -Werror

_OBJS = main.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	@IF not exist $(ODIR) mkdir $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OUT): $(OBJS)
	@IF not exist $(OUTDIR) mkdir $(OUTDIR)
	$(CC) $(CFLAGS) -g $^ -o $@

clean:
	rm -f $(ODIR)/*.o $(OUT)