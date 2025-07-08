run bin:
	make bin/{{bin}}
	LD_LIBRARY_PATH=src bin/{{bin}}
