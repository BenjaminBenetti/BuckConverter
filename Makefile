#passthrough to /src

.PHONY: noArg
noArg:
	make -C src/

%:
	make -C src/ $@
