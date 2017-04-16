#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ev.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>

#include "antouch_server.h"
#include "ProtoATCI.h"

//
// Created by isthisloss on 18.02.17.
//


#define TCP_PORT 12345
#define UDP_PORT 12346

static struct ev_loop*  loop;
static struct ev_io*    acceptor;
static struct ev_io*    broadcast_watcher;
static struct ProtoAtci* atci;

static void acceptor_init();
static void acceptor_close();
static void broadcast_acceptor_init();
static void broadcast_acceptor_close();

static void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int);
static void response_cb(struct ev_loop* loop, struct ev_io* watcher, int);
static void broadcast_cb(struct ev_loop* loop, struct ev_io* watcher, int);

static void sig_term(int);

static char* get_ip_of_current_machine();

void antouch_server_init()
{
    loop = ev_default_loop(0);
    acceptor_init();
    broadcast_acceptor_init();
    atci = atci_init();
}

void antouch_server_start()
{
    struct sigaction act;
    act.sa_handler = sig_term;
    sigaction(SIGTERM, &act, NULL);

    ev_io_start(loop, acceptor);
    ev_io_start(loop, broadcast_watcher);
    ev_run(loop, 0);
}

void acceptor_init()
{
    /* acceptor of TCP connection */
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(TCP_PORT);

    bind(master_socket, (struct sockaddr*)&addr_in, sizeof(addr_in));
    listen(master_socket, SOMAXCONN);

    acceptor = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(acceptor, accept_cb, master_socket, EV_READ);
}

void acceptor_close()
{
    ev_io_stop(loop, acceptor);
    shutdown(acceptor->fd, SHUT_RDWR); // ???
    close(acceptor->fd);
    free(acceptor);
}

void broadcast_acceptor_init()
{
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in local_addr;
    //socklen_t len;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(UDP_PORT);

    bind(udp_sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
    broadcast_watcher = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(broadcast_watcher, broadcast_cb, udp_sock, EV_READ);
}

void broadcast_acceptor_close()
{
    ev_io_stop(loop, broadcast_watcher);
    close(broadcast_watcher->fd);
    free(broadcast_watcher);
}

void broadcast_cb(struct ev_loop* loop, struct ev_io* watcher, int revent)
{
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0x0,sizeof(dest_addr));
    socklen_t len = sizeof(dest_addr);
    char buf[128];
    ssize_t s = recvfrom(watcher->fd, (void*)buf, sizeof(buf), MSG_NOSIGNAL,
                         (struct sockaddr*)&dest_addr, &len);
    if (s)
    {
        const char* ip = get_ip_of_current_machine();
        //const char* ip = inet_ntoa(dest_addr.sin_addr);
        printf("Current local ip is %s, len = %lu\n", ip, strlen(ip)+1);
        sendto(watcher->fd, ip, strlen(ip)+1, MSG_NOSIGNAL, (struct sockaddr*)&dest_addr, len);
    }
}

void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revent)
{
    int socket = accept(acceptor->fd, NULL, 0);
    struct ev_io* new_client_watcher = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(new_client_watcher, response_cb, socket, EV_READ);
    ev_io_start(loop, new_client_watcher);
    printf("NET\tNew connection\n");

    ev_io_stop(loop, acceptor);
    ev_io_stop(loop, broadcast_watcher);
    printf("NET\tAcceptor and Broadcast has been stopped\n");
}

void response_cb(struct ev_loop* loop, struct ev_io* watcher, int revents)
{
    uint8_t head = 0;
    ssize_t s = recv(watcher->fd, (char*)&head, sizeof(uint8_t), MSG_NOSIGNAL);
    if (s == 0)
    {
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
        ev_io_start(loop, acceptor);
        ev_io_start(loop, broadcast_watcher);
        //acceptor_init();
        //broadcast_acceptor_init();
        printf("NET\tAcceptor and Broadcast has been restarted\n");
    }
    else
    {
        printf("head = 0x%X, head_body = 0x%X\n", head & 0x03, head >> 2);
        uint8_t type = atci_get_type(head);
        if (type == MOVE)
        {
            int16_t d[2];
            recv(watcher->fd, (char*)&d, sizeof(d) * sizeof(int16_t), MSG_NOSIGNAL);
            printf("%d %d\n", d[0], d[1]);
            atci_mouse_move(atci, d[0], d[1]);
        }
        else if (type == COMMAND)
        {
            atci_command(atci, head);
        }
        else if (type == V_SCROLL)
        {
            int16_t dy;
            recv(watcher->fd, (char*)&dy, sizeof(int16_t), MSG_NOSIGNAL);
            atci_mouse_scroll(atci, dy);
        }
        else
        {
            return;
        }
    }
}

char* get_ip_of_current_machine()
{
    //!
    struct ifaddrs *ifaddrs, *ifa;
    getifaddrs(&ifaddrs);
    for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next)
    {
        struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
        if (addr && addr->sin_family == AF_INET && strstr(ifa->ifa_name, "wl"))
        {
            freeifaddrs(ifaddrs);
            return inet_ntoa(addr->sin_addr);
        }
    }
    return NULL;
}

void sig_term(int revents)
{
    printf("NET\tApplication got the SIGTERM signal. Aborting...\n");
    atci_close(atci);
    acceptor_close();
    broadcast_acceptor_close();
    ev_break(loop, EVBREAK_ALL);
}
