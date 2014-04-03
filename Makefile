ifneq (, $(findstring clang, $(shell $(CXX) --version)))
	FLAGS = -stdlib=libc++
endif

COMMAND = $(CXX) -g -pthread -std=c++11 $(FLAGS)
OPT_COMMAND = $(COMMAND) -O3

all: list_coarse list_fine hashtable_naive hashtable_smart

debug: list_coarse_debug list_fine_debug hashtable_naive_debug hashtable_smart_debug

clean:
	rm -f list_coarse list_fine hashtable_naive hashtable_smart
	rm -f *_debug
	rm -rf *.dSYM
	rm *.class

LIST_COARSE_DEPS = list_coarse.cpp benchmark.hpp
LIST_FINE_DEPS = list_fine.cpp benchmark.hpp

list_coarse: $(LIST_COARSE_DEPS)
	$(OPT_COMMAND) list_coarse.cpp -o list_coarse
list_coarse_debug: $(LIST_COARSE_DEPS)
	$(COMMAND) list_coarse.cpp -o list_coarse_debug

list_fine: $(LIST_FINE_DEPS)
	$(OPT_COMMAND) list_fine.cpp -o list_fine
list_fine_debug: $(LIST_FINE_DEPS)
	$(COMMAND) list_fine.cpp -o list_fine_debug

HASHTABLE_NAIVE_DEPS = hashtable_naive.cpp benchmark.hpp
HASHTABLE_SMART_DEPS = hashtable_smart.cpp benchmark.hpp

hashtable_naive: $(HASHTABLE_NAIVE_DEPS)
	$(OPT_COMMAND) hashtable_naive.cpp -o hashtable_naive
hashtable_naive_debug: $(HASHTABLE_NAIVE_DEPS)
	$(COMMAND) hashtable_naive.cpp hashtable_naive

hashtable_smart: $(HASHTABLE_SMART_DEPS)
	$(OPT_COMMAND) hashtable_smart.cpp -o hashtable_smart
hashtable_smart_debug: $(HASHTABLE_SMART_DEPS)
	$(COMMAND) hashtable_smart.cpp -o hashtable_smart

hashtable_java: HashtableJava.java
	javac HashtableJava.java
