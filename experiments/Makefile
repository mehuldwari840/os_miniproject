# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread

# Target executables for the 4 OS experiments
TARGETS = exp6_scheduler exp7_sync exp8_memory exp10_lru

all: $(TARGETS)

exp6_scheduler: exp6_priority_aging_scheduler.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

exp7_sync: exp7_synchronization_producer_consumer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

exp8_memory: exp8_memory_management_paging_segmentation.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

exp10_lru: exp10_lru_page_replacement.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run-all: all
	@echo "=========================================================="
	@echo ">>> RUNNING EXPERIMENT 6: SCHEDULING WITH AGING <<<"
	@echo "=========================================================="
	./exp6_scheduler
	@echo "\n=========================================================="
	@echo ">>> RUNNING EXPERIMENT 7: PRODUCER-CONSUMER MUTEX/SYNC <<<"
	@echo "=========================================================="
	./exp7_sync
	@echo "\n=========================================================="
	@echo ">>> RUNNING EXPERIMENT 8: PAGING & SEGMENTATION MMU <<<"
	@echo "=========================================================="
	./exp8_memory
	@echo "\n=========================================================="
	@echo ">>> RUNNING EXPERIMENT 10: LRU PAGE REPLACEMENT <<<"
	@echo "=========================================================="
	./exp10_lru

clean:
	rm -f $(TARGETS) exp6_priority_aging_scheduler exp7_synchronization_producer_consumer exp8_memory_management_paging_segmentation exp10_lru_page_replacement *.o
