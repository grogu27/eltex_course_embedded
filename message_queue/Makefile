DIRS:= posix system_v

all:
	@for i in $(DIRS); do \
		$(MAKE) -C $$i; \
	done

clean:
	@for i in $(DIRS); do \
		$(MAKE) -C $$i clean; \
	done

.PHONY: clean all