#include <stdio.h>
#include <ev.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

#include "xlib_wrapper.h"

// There are only testing functions for first try
void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);

void accept_cb(struct ev_loop* loop, struct ev_io* acceptor, int revents);

// todo
// add sigterm handler
//
// distant plans
// add all muse's clicks and buttons emulation

void sig_term(int signum);

int sock;
struct ev_loop* loop;
struct ev_io acceptor;

int main(int argc, const char** argv)
{
    struct sigaction act;
    act.sa_handler = sig_term;
    sigaction(SIGTERM, &act, NULL);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(12345);
    bind(sock, (struct sockaddr*)&addr_in, sizeof(addr_in));
    listen(sock, SOMAXCONN);

    loop = ev_default_loop(0);
    ev_io_init(&acceptor, accept_cb, sock, EV_READ);
    ev_io_start(loop, &acceptor);

    xlw_init();

    ev_run(loop, 0);
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
        /*
         * 0 - move
         * 1 - left
         * 3 - right
         * 2 - wheel
         * 4 - wheel up
         * 5 - wheel - down
         */
        if (cmd == 0)
            xlw_mouse_move(dx, dy);
        else
            xlw_mouse_click(cmd);
    }
}

void sig_term(int signum)
{
    xlw_close();
    close(acceptor.fd);
    ev_io_stop(loop, &acceptor);
    ev_break(loop, EVBREAK_ALL);
}
