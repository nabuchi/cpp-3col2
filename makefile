TARGET = col2
OBJS = col2.o mt.o

all : $(OBJS)
	g++ -o $(TARGET) $(OBJS)

clean :
	rm *.o

.c.o :
	g++ -c $<
.cpp.o :
	g++ -c $<
