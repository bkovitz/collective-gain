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

# Givers have no effect (they don't give, but they still don't reproduce)
run00: collective-gain
	./collective-gain \
		--initial_g=0.01 \
		--num_gens=400 \
		--xsize=30 \
		--ysize=30 \
		--base_fecundity=1 \
		--seed=590679785 \
		--acorn_stdev=2 \
		--base_carrying_capacity=0.09 \
		--giving_radius=2 \
		--laying_radius=2 \
		--cc_delta=0 \
		--mutation_stdev=0.05 \
	  --show_givers --show_carrying_capacity \
		--show_next_organisms
# result: population hovers around 70, average around 0.09
# data, 72, 0.07, 8, 0, 0

# Givers have huge effect: cc_delta=0.40
run40: collective-gain
	./collective-gain \
		--initial_g=0.01 \
		--num_gens=400 \
		--xsize=30 \
		--ysize=30 \
		--base_fecundity=1 \
		--seed=590679785 \
		--acorn_stdev=2 \
		--base_carrying_capacity=0.09 \
		--giving_radius=2 \
		--laying_radius=2 \
		--cc_delta=0.40 \
		--mutation_stdev=0.05 \
	  --show_givers --show_carrying_capacity \
		--show_next_organisms
# result: population rapidly grows to around 400, average g around 0.10
# data, 370, 0.11, 31, 0, 0

run40_slow_mutation: collective-gain
	./collective-gain \
		--initial_g=0.01 \
		--num_gens=400 \
		--xsize=30 \
		--ysize=30 \
		--base_fecundity=1 \
		--seed=590679785 \
		--acorn_stdev=2 \
		--base_carrying_capacity=0.09 \
		--giving_radius=2 \
		--laying_radius=2 \
		--cc_delta=0.40 \
		--mutation_stdev=0.01 \
	  --show_givers --show_carrying_capacity \
		--show_next_organisms

run40_no_mutation: collective-gain
	./collective-gain \
		--initial_g=0.00 \
		--num_gens=400 \
		--xsize=30 \
		--ysize=30 \
		--base_fecundity=1 \
		--seed=590679785 \
		--acorn_stdev=2 \
		--base_carrying_capacity=0.09 \
		--giving_radius=2 \
		--laying_radius=2 \
		--cc_delta=0.40 \
		--mutation_stdev=0.00 \
		--num_initial_carriers=1 \
		--carrier_initial_g=0.05 \
		--num_initial_organisms=20 \
	  --show_givers --show_carrying_capacity \
		--show_next_organisms

run40_no_mutation_no_kin_selection: collective-gain
	./collective-gain \
		--initial_g=0.00 \
		--num_gens=400 \
		--xsize=30 \
		--ysize=30 \
		--base_fecundity=1 \
		--seed=590679785 \
		--acorn_stdev=2 \
		--base_carrying_capacity=0.09 \
		--giving_radius=15 \
		--laying_radius=2 \
		--cc_delta=0.40 \
		--mutation_stdev=0.00 \
		--num_initial_carriers=1 \
		--carrier_initial_g=0.05 \
		--num_initial_organisms=20 \
	  --show_givers --show_carrying_capacity \
		--show_next_organisms
# giving_radius=15 should cover the whole world

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
