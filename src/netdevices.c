#include "../inc/netdev.h"

NetDev *init_dev() {
    NetDev *n = (NetDev *) malloc(sizeof(n));
    if(!n) return NULL;
    n->dev_name = NULL;
    n->next = NULL;
    return n;
}

void add_dev_to_list(DevList *dlist, NetDev *n) {
    NetDev *tmp = *dlist;
    
    n->next = NULL;
    if(tmp == NULL) 
        *dlist = n;
    else {
        while(tmp->next != NULL) 
            tmp = tmp->next;
        
        tmp->next = n;
    }
}

void traverse_devices(const DevList *dl, void (*f)(NetDev *nd)) {
    NetDev *traverser = *dl, *ptr;
    while(traverser != NULL) {
        ptr = traverser;
        (*f)(ptr);
        ptr = NULL;
        traverser = traverser->next;
    }
}

NetDev *check_dev_ip(const DevList *dl, char *ifname) {
    NetDev *traverser = *dl, *ptr;
    while(traverser != NULL) {
        ptr = traverser;

        if(cmp_ifaces(ptr->dev_name, ifname) == 0)
            return ptr;

        ptr = NULL;
        traverser = traverser->next;
    }
}

void print_dev_data(NetDev *nd) {
    char mac[20];

    snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
        nd->net_dev_mac_addr[0], nd->net_dev_mac_addr[1], 
        nd->net_dev_mac_addr[2], nd->net_dev_mac_addr[3], 
        nd->net_dev_mac_addr[4], nd->net_dev_mac_addr[5]);

    //wypisuje dane urzadzenia w postaci czytelnej dla człowieka

    printf("Interface name = %-20s, MAC address = %s, IP address = %s\n", 
        nd->dev_name, mac, nd->net_dev_ip_addr);
}

int cmp_ifaces(char *dev_iface, char *iface) {
    return strcmp(dev_iface, iface);
}

void free_devices(DevList *n) {
    NetDev *ptr;
    while(*n != NULL) {
        ptr = (*n)->next;
        free((*n)->dev_name);
        free(*n);
        *n = ptr;
    }
    /* free((*n)->next);
    free(n); */
}