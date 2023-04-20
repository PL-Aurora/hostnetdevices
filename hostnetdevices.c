
#include <stdio.h>

#include "inc/netdev.h"
#include "inc/netgateway.h"

int main(int, char**) {
    initialize_device_list();

    getgatewayandiface();

    free_devices();
    return 0;
}
