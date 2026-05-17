
DIRS:= 1 2 3 4

all: 
	@for d in $(DIRS); do \
		$(MAKE) -C $$d; \
	done

clean:
	@for d in $(DIRS); do \
		$(MAKE) -C $$d clean; \
	done

.PHONY: all clean