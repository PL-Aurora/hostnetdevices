#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct NetDevice {
    char *dev_name;
    unsigned char net_dev_mac_addr[8];
    unsigned char net_dev_ip_addr[14];
    struct NetDevice *next;
};

typedef struct NetDevice NetDev;
typedef struct NetDevice *DevList;

NetDev *init_dev();

void add_dev_to_list(DevList *dlist, NetDev *n);
void traverse_devices(const DevList *dl, void (*f)(NetDev *nd));

NetDev *check_dev_ip(const DevList *dl, char *ifname);

void print_dev_data(NetDev *nd);
int cmp_ifaces(char *dev_iface, char *iface);

void free_devices(DevList *n);