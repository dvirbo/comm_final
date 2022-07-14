all: recv_udp send_udp

recv_udp: recv_udp.c
	gcc recv_udp.c -o recv

send_udp: send_udp.c
	gcc send_udp.c -o send


clean:
	rm -f *.o recv send