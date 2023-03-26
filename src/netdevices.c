#include "../inc/netdev.h"

hostdevice_t *device_list = NULL;

void initialize_device_list(){

}

hostdevice_t *initialize_device() {
    hostdevice_t *hd = malloc(sizeof(hostdevice_t));
    if(!hd)
        return NULL;
    memset(hd, 0, sizeof(hostdevice_t));
    hd->if_name = NULL;
    hd->next = NULL;
    return hd;
}