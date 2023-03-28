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

void loop_devices(const hostdevice_t **list, void (*f)(hostdevice_t *hd));

/*
void traverse_devices(const DevList *dl, void (*f)(NetDev *nd));

NetDev *check_dev_ip(const DevList *dl, char *ifname);

void print_dev_data(NetDev *nd);
 */

/* inne funkcje */
int cmp_ifaces(char *dev_iface, char *iface);

#endif //_NETDEV_H_