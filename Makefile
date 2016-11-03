all: backtrack brute

backtrack: backtrack.c
	gcc backtrack.c -o backtrack.out -lm
	./backtrack.out knapsack.txt

brute: brute.c
	gcc brute.c -o brute.out -lm
	./brute.out knapsack.txt

clean:
	rm -f *.out brute_output.txt backtrack_output.txt
