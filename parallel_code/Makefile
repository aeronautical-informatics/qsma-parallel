CC = gcc
CXX = g++
NAME = FLTA
OPT = 2
MPI = 4

CFLAGS = -I. -lm -O$(OPT) -fPIC -U_FORTIFY_SOURCE -g -ftree-vectorize -pipe -Wno-psabi  -mcpu=cortex-a7 
CXXFLAGS = $(CFLAGS) -fpermissive  
LDFLAGS = -lm 
INC =
LIBS_PATH =

C_FILES = emxapi_comm_ringbuf_fixed.c emxapi_comm_ringbuf_fixed_impl.c timestamp.c _emx_s_FLTA.c emxapi_comm_ringbuf_fixed_fork.c _emx_s_FLTA_STEP.c _emx_t_main.c
CPP_FILES = 
H_FILES = emxapi_comm_ringbuf_fixed_fork.h _emx_main.h FLTA.h emxapi_defines.h emxapi_comm_ringbuf_config.h QSMA_TAWS_private.h Data.h errors.h timestamp.h QSMA_TAWS_types.h rtwtypes.h emxapi_comm_ringbuf_fixed.h FLTA_STEP.h QSMA_TAWS.h

OBJECTS = $(patsubst %.c, %.o, $(C_FILES)) $(patsubst %.cpp, %.o, $(CPP_FILES))

all: run

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS_PATH) $(LDFLAGS)

run: $(NAME)
	ulimit -s unlimited; ./FLTA

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.cpp $(H_FILES)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f *.o FLTA
