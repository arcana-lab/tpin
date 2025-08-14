tpin:
	clang++ -fopenmp -std=c++17 tpin.cpp -o tpin

clean:
	rm -f tpin

.PHONY: clean
