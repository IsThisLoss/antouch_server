#include <stdio.h>
#include <ev.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#include "xlib_wrapper.h"

// There are only testing functions for first try
void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);

void accept_cb(struct ev_loop* loop, struct ev_io* acceptor, int revents);

// todo
//
// replace TCP with UDP (if it's possible, I dunno)
// add sigterm handler
//
// distant plans
// add all muse's clicks and buttons emulation
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

    xlw_init();

    ev_run(loop, 0);
    xlw_close();
    return 0;
}

void accept_cb(struct ev_loop* loop, struct ev_io* acceptor, int revents)
{
    int socket = accept(acceptor->fd, NULL, 0);
    struct ev_io* watcher = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(watcher, response_cb, socket, EV_READ);
    ev_io_start(loop, watcher);
    printf("connected\n");
}

void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents)
{
    char buff[512];
    ssize_t size = recv(watcher->fd, (void*)buff, sizeof(buff), MSG_NOSIGNAL);
    write(STDOUT_FILENO, buff, size);
    if (size == 0)
    {
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
    }
    else
    {
        int cmd = 0, dx = 0, dy = 0;
        sscanf(buff, "%d %d %d", &cmd, &dx, &dy);
        if (cmd == 1 || cmd == 3)
            xlw_mouse_click();
        else
            xlw_mouse_move(dx, dy);
    }
}
