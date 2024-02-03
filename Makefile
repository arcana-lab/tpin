tpin:
	clang++ \
	  -DTHREADS_PER_CORE=$$(lscpu | grep "Thread(s)" | egrep -o "[0-9]+") \
	  -DNUMA_DOMAINS=$$(lscpu | grep "NUMA node(s)" | egrep -o "[0-9]+") \
	  -fopenmp \
	  -o tpin \
	  tpin.cpp

clean:
	rm -f tpin

.PHONY: clean
