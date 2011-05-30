CXX = g++
CXXFLAGS = -std=c++98 -pedantic -Wall -Wextra -O3
HPRJ = um.hpp
HMEM = mem.hpp blk.hpp raw.hpp
HPMM = $(HPRJ) $(HMEM)
HPRC = prc.hpp ldr.hpp reg.hpp log.hpp $(HPMM)
HALL = $(HPRC) tst.hpp
OALL = main.o tst.o prc.o reg.o mem.o ldr.o end.o blk.o raw.o log.o con.o

um : $(OALL)
	$(CXX) -o um $(OALL)

.PHONY : clean all

clean :
	-rm um $(OALL)

all : um

main.o tst.o: $(HALL)

prc.o : $(HPRC)

reg.o : reg.hpp $(HPRJ)

mem.o : log.hpp $(HPMM)

ldr.o : ldr.hpp end.hpp $(HPMM)

end.o : end.hpp $(HPRJ)

blk.o : blk.hpp raw.hpp log.hpp $(HPRJ)

raw.o : raw.hpp log.hpp $(HPRJ)

log.o : log.hpp $(HPRJ)

con.o : con.hpp $(HPRJ)

