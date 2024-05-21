COPTS=-O3 -fomit-frame-pointer -std=gnu9x

breakout:	libsprite.a breakout.o
	qgcc qdos-gcc -o breakout breakout.o -lsprite

breakout.o:	breakout.c makefile
		qgcc qdos-gcc $(COPTS) -o breakout.o -c breakout.c

clean:
	rm -f breakout.o
	make -C QL-sprites clean

git:	clean
	git add .
	git commit
	git push

deploy:  breakout
	cp breakout /home/simon/emulators/ql/emulators/sQLux/flp1/
	cp breakout.lib /home/simon/emulators/ql/emulators/sQLux/flp1/
	cp QL-sprites/env.bin /home/simon/emulators/ql/emulators/sQLux/flp1/
	cp BOOT_flp1 /home/simon/emulators/ql/emulators/sQLux/flp1/BOOT

run:    deploy
	cd /home/simon/emulators/ql/emulators/sQLux && ./sqlux --SPEED=0.75 --RAMSIZE=896 --SOUND 8 -b "LRUN flp1_BOOT"

runfast:    deploy
	cd /home/simon/emulators/ql/emulators/sQLux && ./sqlux --SPEED=0 --RAMSIZE=896 --SOUND 8 -b "LRUN flp1_BOOT"


dist:   deploy
	cp BOOT_flp1 BOOT
	cp breakout.lib breakout_lib
	qltools breakout.img -fdd breakout -W
	qltools sprites.img -W BOOT breakout breakout_lib
	rm -f BOOT breakout_lib
