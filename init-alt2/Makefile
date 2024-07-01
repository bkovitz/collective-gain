#CXXFLAGS = -std=c++11 -Wall -O3
CXXFLAGS = -std=c++20 -Wall -O3 -I/opt/homebrew/Cellar/boost/1.85.0/include
LDFLAGS = -lm -L/opt/homebrew/Cellar/boost/1.85.0/lib -lboost_program_options \
	-lboost_unit_test_framework

RANDOM_OBJS = random.o
INIT_ALT_OBJS = init-alt.o $(RANDOM_OBJS)
COLLECTIVE_GAIN_OBJS = collective-gain.o $(RANDOM_OBJS)
COLLECTIVE_GAIN_FECUNDITY_OBJS = collective-gain-fecundity.o $(RANDOM_OBJS)
C_OBJS = c.o $(RANDOM_OBJS)

OBJS = $(INIT_ALT_OBJS) $(C_OBJS)

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cc -o $*.d

run: collective-gain
	./collective-gain --num_gens 400 --show_givers --show_carrying_capacity \
		--show_next_organisms

runi: init-alt
	./init-alt

runf: collective-gain-fecundity
	./collective-gain-fecundity

collective-gain: $(COLLECTIVE_GAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

collective-gain-fecundity: $(COLLECTIVE_GAIN_FECUNDITY_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

init-alt:	$(INIT_ALT_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

c: $(C_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@


# header-file dependencies
-include $(OBJS:.o=.d)

tags:
	ctags *.cc *.h

clean:
	rm -f a b c init-alt *.o *.d

.PHONY: tags clean
