#ifndef _NETDEV_H_
#define _NETDEV_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* struct NetDevice {
    char *dev_name;
    unsigned char net_dev_mac_addr[8];
    unsigned char net_dev_ip_addr[14];
    struct NetDevice *next;
};

typedef struct NetDevice NetDev;
typedef struct NetDevice *DevList; */

struct hostdevice {
    char *if_name;
    unsigned char host_dev_mac_addr[8];
    unsigned char host_dev_ip_addr[14];

    struct hostdevice *next;
};

typedef struct hostdevice hostdevice_t;

extern hostdevice_t *device_list;

/* funkcje inicjalizujace */
void initialize_device_list();
hostdevice_t *initialize_device();

/* funkcje manipulujace urzadzeniem */
void add_device_to_list(hostdevice_t **list, hostdevice_t *hd);
void free_devices();

void loop_devices(hostdevice_t **list, void (*f)(hostdevice_t *hd));

hostdevice_t *check_gateway(hostdevice_t **list);
hostdevice_t *check_dev_data(hostdevice_t **list, char *iname);
/*
void traverse_devices(const DevList *dl, void (*f)(NetDev *nd));
 */

/* inne funkcje */
void print_dev_data(hostdevice_t *nd);

#endif //_NETDEV_H_