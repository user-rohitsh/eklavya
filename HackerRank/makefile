#ARRAYMANI
TARGET_ARRAY_MANI = ./HAckerRank_ArrayMani/arraymani.exe

OBJS_ARRAY_MANI =  ./HAckerRank_ArrayMani/main.o

ARRAYMANI : $(TARGET_ARRAY_MANI)
	@echo ARRAYMANI done

$(TARGET_ARRAY_MANI) : $(OBJS_ARRAY_MANI)
	g++ -g -o $@ $^

#ARRAYMANI

#Common
%.o : %.cpp
	g++ -g -std=c++1y -o $@ -c $<

all : ARRAYMANI
	@echo All done