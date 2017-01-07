#include <stdio.h>
#include <ev.h>
#include <X11/Xlib.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

// There are only testing functions for first try
void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);

void accept_cb(struct ev_loop* loop, struct ev_io* acceptor, int revents);

// todo
// to get rid of opening display every time when we've gotten new incoming message
// replace TCP with UDP (if it's possible, I dunno)
// add sigterm handler
//
// distant plans
// add single muse click and buttons emulation
int main(int argc, const char** argv)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(12345);
    bind(sock, (struct sockaddr*)&addr_in, sizeof(addr_in));
    listen(sock, SOMAXCONN);

    struct ev_loop* loop = ev_default_loop(0);
    struct ev_io acceptor;
    ev_io_init(&acceptor, accept_cb, sock, EV_READ);
    ev_io_start(loop, &acceptor);

    ev_run(loop, 0);
    return 0;
}

void accept_cb(struct ev_loop* loop, struct ev_io* acceptor, int revents)
{
    int socket = accept(acceptor->fd, NULL, 0);
    struct ev_io* watcher = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(watcher, response_cb, socket, EV_READ);
    ev_io_start(loop, watcher);
}

void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents)
{
    char buff[512];
    ssize_t size = recv(watcher->fd, (void*)buff, sizeof(buff), MSG_NOSIGNAL);
    if (size == 0)
    {
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
    }
    else
    {
        Display* display = XOpenDisplay(NULL);
        int x_offset = 0, y_offset = 0;
        sscanf(buff, "%d %d", &x_offset, &y_offset);
        XWarpPointer(display, None, None, 0, 0, 0, 0, x_offset, y_offset);
        XFlush(display);
        XCloseDisplay(display);
    }
}
