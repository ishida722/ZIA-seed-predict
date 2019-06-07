include ../../env.mk

INC = -I../../dv-user-driver/include -I../common/include -I/usr/include/agg2 `freetype-config --cflags` -I./include/
LIB = -L../../dv-user-driver -ldmpdv -L../common/lib -ldv700_util -lagg -lfreetype -lm `pkg-config --libs opencv`

PYPASS = /usr/include/python3.6

CFLAGS = -pthread -std=c++11 -fPIC $(OPT) -Wall -Werror -c $(INC) -g -O0 
LFLAGS = -pthread -std=c++11 $(OPT)
PFLAGS = -std=c++11 -03 -fPIC -c -I$(PYPASS)

DEPS = Seed_gen.h
OBJS = pumpkin20181218_verylight_gen.o seed_predict.o
TGT  = ./bin/Seed-predict

%.o: src/%.cpp
	$(GPP) $(CFLAGS) -o $@ $<

seed_net_wrap.o:
	g++ -std=c++11 -O3 -fPIC -c -I/usr/include/python3.6 $(INC) -o $@ pumpkin20181218_verylight/seed_net_wrap.cpp

seed_net_wrap.so:
	g++ -std=c++11 -shared -fPIC seed_net_wrap.o pumpkin20181218_verylight_gen.o dmp_network.o -o $@ -L/path/to/dv-user-driver -ldmpdv -lboost_python3 -lboost_numpy3

all : $(OBJS)
	$(GPP) $(LFLAGS) $(OBJS) -o $(TGT) $(LIB)

python :
	$(GPP) $(LFLAGS) $(OBJS) -o $(TGT) $(LIB)

run : $(TGT)
	make
	$(TGT)

clean:
	rm -f *.o $(TGT)
