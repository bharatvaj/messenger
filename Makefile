CFLAGS+=-g -std=c99
LDFLAGS+=-lpthread -lclog

# Emscripten
# LDFLAGS+=-g4 -s WASM=1 -s SOCKET_DEBUG=1

messenger: messenger.c config.h network_utils.h
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f messenger
