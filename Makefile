CFILES = main.c
CFLAGS = -pedantic

bin/main: $(CFILES)
	mkdir -p $(@D)
	gcc $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf bin

