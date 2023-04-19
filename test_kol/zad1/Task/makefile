#  STATIC with no .a
static:
	gcc -Wall -c libbibl1.c
	gcc -Wall -o zad1 zad1.c libbibl1.o -static

clean_static:
	rm -rf *.o zad1

# STATIC with .a
static_a:
	gcc -Wall -c libbibl1.c
	ar rcs libbibl1.a libbibl1.o
	gcc -Wall -o zad1 zad1.c -L. -lbibl1

clean_static_a:
	rm -rf *.o *.a zad1


## Dynamic
dynamic: 
	gcc -Wall -c libbibl1.c
	gcc -Wall -o zad1 zad1.c libbibl1.o
	gcc -Wall -fPIC -shared -o libbibl1.so libbibl1.c 
	gcc -Wall -o share_zad1 zad1.c -L. -lbibl1
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

clean_dynamic:
	rm -rf *.so *.o zad1 share_zad1
	