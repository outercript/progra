TARGETS=vector
.PHONY: $(TARGETS)

all: $(TARGETS)

$(TARGETS):
	$(MAKE) -C $@

clean:
	@git clean -dfx
