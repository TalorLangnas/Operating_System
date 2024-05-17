SUBDIRS = Targil_1 Targil_2 Targil_3 Targil_4

all:
	for dir in $(SUBDIRS); do \
 		$(MAKE) -C $$dir; \
 done

clean:
	for dir in $(SUBDIRS); do \
 		$(MAKE) -C $$dir clean; \
 done

