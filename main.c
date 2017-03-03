#include "antouch_server.h"

int main(int argc, const char** argv)
{
    antouch_server_init();
    antouch_server_start();
    //wait_for_device();
    return 0;
}
