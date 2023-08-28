
PLUGINS = $(shell  find * -maxdepth 0  -type d | grep -v .svn )


.PHONY: subdirs $(DIRS) 

all:  subdirs

subdirs: 	
	for dir in $(PLUGINS); do \
	$(MAKE) -C $$dir all; \
	done

clean:
	for dir in $(PLUGINS); do \
	$(MAKE) clean -C $$dir; \
	done
	

print:
	@echo $(PLUGINS)	
