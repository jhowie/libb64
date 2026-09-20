# MacOS Makefile for lyparseJSON
#
ARCHITECTURE = $(shell uname -m)
CC=cc
CFLAGS=-g
LDFLAGS=-L$(LIBDIR) -lb64
OBJDIR=obj/$(ARCHITECTURE)
LIBDIR=lib/$(ARCHITECTURE)
LIBOBJS=$(addprefix $(OBJDIR)/,b64.o)
INSTALLBINDIR=/usr/local/bin
INSTALLLIBDIR=/usr/local/lib
INSTALLINCLUDEDIR=/usr/local/include
#

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(<:.c=.o)

all: $(LIBDIR)/libb64.a $(OBJDIR)/asctob64 $(OBJDIR)/b64toasc

$(LIBDIR)/libb64.a: $(LIBOBJS) $(LIBDIR)
	ar -r $(LIBDIR)/libb64.a $(LIBOBJS)
	ranlib $(LIBDIR)/libb64.a

$(OBJDIR)/asctob64: $(OBJDIR)/asctob64.o
	cc $(LDFLAGS) -o $(OBJDIR)/asctob64 $(OBJDIR)/asctob64.o

$(OBJDIR)/b64toasc: $(OBJDIR)/b64toasc.o
	cc $(LDFLAGS) -o $(OBJDIR)/b64toasc $(OBJDIR)/b64toasc.o


clean:
	rm -rf obj lib

install: $(LIBDIR)/liblb64.a parseJSON.h
	install -d -o root -g wheel $(INSTALLLIBDIR)
	install -c -o root -g wheel -m 0444 $(LIBDIR)/libb64.a $(INSTALLLIBDIR)
	install -d -o root -g wheel $(INSTALLINCLUDEDIR)
	install -c -o root -g wheel -m 0444 b64.h $(INSTALLINCLUDEDIR)




$(LIBOBJS): $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)