DIRS:= posix system_v/without_sem system_v/with_sem 

all:
	@for i in $(DIRS); do \
		$(MAKE) -C $$i; \
	done

clean:
	@for i in $(DIRS); do \
		$(MAKE) -C $$i clean; \
	done

.PHONY: clean all