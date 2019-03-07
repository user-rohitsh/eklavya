LINK_TARGET = arraymani.exe

# Here is a Make Macro that uses the backslash to extend to multiple lines.
OBJS =  main.o

all : $(LINK_TARGET)
	echo All done

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^

%.o : %.cpp
	g++ -g -std=c++1y -o $@ -c $<