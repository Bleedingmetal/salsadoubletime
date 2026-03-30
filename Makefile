
original: salsa20.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h
	gcc salsa20.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h -o original

threaded: salsa20_threaded.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h
	gcc salsa20_threaded.c main.c ecrypt-config.h ecrypt-machine.h ecrypt-portable.h ecrypt-sync.h -o original

clean:
	rm -rf original threaded