CC ?= gcc
CXX ?= g++
NAME = FLTA
OPT = 2
MPI = 4
7ZIP := 7z

CFLAGS = -I. -lm -O$(OPT) -fPIC -U_FORTIFY_SOURCE -g -ftree-vectorize -pipe -Wno-psabi
CXXFLAGS = $(CFLAGS) -fpermissive  
LDFLAGS = -lm 
INC =
LIBS_PATH =

C_FILES = ert_main.c QSMA_TAWS.c timestamp.c Data.c emxapi_comm_ringbuf_thread.c _emx_s_FLTA.c bouncer.c _emx_s_FLTA_STEP.c loadData.c
CPP_FILES = 
H_FILES = _emx_main.h emx_ringbuf.h FLTA.h emxapi_comm_ringbuf_thread.h emxapi_defines.h QSMA_TAWS_private.h Data.h errors.h timestamp.h QSMA_TAWS_types.h emx_ringbuf_defines.h rtwtypes.h FLTA_STEP.h QSMA_TAWS.h

OBJECTS = $(patsubst %.c, %.o, $(C_FILES)) $(patsubst %.cpp, %.o, $(CPP_FILES))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS_PATH) $(LDFLAGS)

run: $(NAME)
	ulimit -s unlimited; ./FLTA

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.cpp $(H_FILES)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
	
loadData.c: loadData.c.xz
	$(7ZIP) x $

clean:
	rm -f *.o FLTA
