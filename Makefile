include ../../env.mk

INC = -I../../dv-user-driver/include -I../common/include -I/usr/include/agg2 `freetype-config --cflags` -I./include/
LIB = -L../../dv-user-driver -ldmpdv -L../common/lib -ldv700_util -lagg -lfreetype

CFLAGS = -pthread -std=c++11 $(OPT) -Wall -Werror -c $(INC) -g -O0 
LFLAGS = -pthread -std=c++11 $(OPT)

DEPS = Seed_gen.h
OBJS = KerasGoogLeNet_gen.o seed_predict.o
TGT  = ./bin/Seed-predict

%.o: src/%.cpp
	$(GPP) $(CFLAGS) -o $@ $<

all : $(OBJS)
	$(GPP) $(LFLAGS) $(OBJS) -o $(TGT) $(LIB)

clean:
	rm -f *.o $(TGT)
