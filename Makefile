CPP = g++

#uncomment this for counting number of iterations
OPTS = -fopenmp -DCOUNT

# Uncomment this for production run without counters
#OPTS = -fopenmp 

#uncomment this for optimization
#OPTS = -fopenmp -O

	
.o.exe:
	$(CPP) $(OPTS) -o $@ $< 

.cpp.o:
	$(CPP) $(OPTS) -c $<

.SUFFIXES:
.SUFFIXES: .o .a .cpp .c .exe

all: vectordp.exe smartdp.exe vectordppar.exe smartdppar.exe

clean:
	rm -f *.o *.exe
