first: recv_udp send_udp

sec: source gateway sink

recv_udp: recv_udp.c
	gcc recv_udp.c -o recv

send_udp: send_udp.c
	gcc send_udp.c -o send

source: source.c
	gcc source.c -o source

gateway: gateway.c
	gcc gateway.c -o gateway

sink: sink.c
	gcc sink.c -o sink

clean:
	rm -f *.o recv send  source sink gateway 