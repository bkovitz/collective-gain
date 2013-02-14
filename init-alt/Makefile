CXXFLAGS = -std=c++11 -Wall -O3
LDFLAGS = -lm

RANDOM_OBJS = random.o
INIT_ALT_OBJS = init-alt.o $(RANDOM_OBJS)
C_OBJS = c.o $(RANDOM_OBJS)

OBJS = $(INIT_ALT_OBJS) $(C_OBJS)

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cc -o $*.d

run: init-alt
	./init-alt

init-alt:	$(INIT_ALT_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

c: $(C_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# header-file dependencies
-include $(OBJS:.o=.d)

clean:
	rm -f a b c init-alt *.o *.d
