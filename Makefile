
original: salsa20.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h
	gcc salsa20.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h -o original

modified: salsa20_modified.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h
	gcc salsa20_modified.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h -o modified

clean:
	rm -rf original modified