#ifndef _NETDEV_H_
#define _NETDEV_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


struct gateway {
    
};

struct hostdevice {
    char *if_name;
    
    uint8_t host_dev_mac_addr[6];
    uint8_t host_dev_ip_addr[4];

    unsigned char host_gtw_ip_addr[14];

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

hostdevice_t *check_dev_data(hostdevice_t **list, char *iname);

void transform_ip(uint8_t *ip, uint32_t ip_decimal);

/* inne funkcje */
void print_dev_data(hostdevice_t *nd);

#endif //_NETDEV_H_