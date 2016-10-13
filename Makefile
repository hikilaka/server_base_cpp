VERBOSE=1
DEBUG=1

ifdef VERBOSE
	Q =
	E = @true 
else
	Q = @
	E = @echo 
endif

CFILES:=$(shell find src -mindepth 1 -maxdepth 5 -name "*.cpp")

INFILES:=$(CFILES) $(CXXFILES)

OBJFILES:=$(CFILES:src/%.cpp=%)
DEPFILES:=$(CFILES:src/%.cpp=%)
OFILES:=$(OBJFILES:%=obj/%.o)

BINFILE=test

COMMONFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-lboost_system -llua5.3

ifdef DEBUG
        COMMONFLAGS := $(COMMONFLAGS) -g
endif

CC=clang++
CFLAGS=$(COMMONFLAGS) --std=c++1z -I ./include/ -I /usr/local/include/boost/ 

CFILES:=$(shell find src -mindepth 1 -maxdepth 5 -name "*.cpp")

INFILES:=$(CFILES)
OBJFILES:=$(CFILES:src/%.cpp=%)
DEPFILES:=$(CFILES:src/%.cpp=%)
OFILES:=$(OBJFILES:%=obj/%.o)

all: $(BINFILE)

.PHONY: clean all depend
.SUFFIXES:
obj/%.o: src/%.cpp
	$(E)C-compiling $<
	$(Q)if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(Q)$(CC) -o $@ -c $< $(CFLAGS)

Makefile.dep: $(CFILES)
	$(E)Depend
	$(Q)for i in $(^); do $(CC) $(CFLAGS) -MM "$${i}" -MT obj/`basename $${i%.*}`.o; done > $@

$(BINFILE): $(OFILES)
	$(E)Linking $@
	$(Q)$(CC) -o $@ $(OFILES) $(LDFLAGS)

clean:
	$(Q)rm -rf $(BINFILE) obj Makefile.dep

