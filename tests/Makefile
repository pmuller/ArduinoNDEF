build:
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Build:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i); \
	done

test: build
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Test:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) run; \
	done

clean:
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Clean:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
