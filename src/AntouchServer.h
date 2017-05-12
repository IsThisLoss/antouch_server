#ifndef ANTOUCH_SERVER_NETWORKING_H
#define ANTOUCH_SERVER_NETWORKING_H

//
// Created by isthisloss on 18.02.17.
//

/**
 * @brief initializes server
 */
void antouch_server_init();

/**
 * @brief starts server
 * IMPORTANT: it blocks the main thread and redefine SIGTERM-handler
 */
void antouch_server_start();

#endif //ANTOUCH_SERVER_NETWORKING_H
