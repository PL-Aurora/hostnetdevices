#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netdb.h>
#include <ifaddrs.h>

#include "../inc/netdev.h"

hostdevice_t *device_list = NULL;

void initialize_device_list(){
    struct ifaddrs *ifaddr = NULL, *ifa = NULL;

    if(getifaddrs(&ifaddr) == -1) {
        fprintf(stderr, "Cannot get interfaces info. Exiting...\n");
        exit(EXIT_FAILURE);
    } else {
        for ( ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next ) {
            //sa_family = AF_PACKET umozliwia dostep adresu MAC
            if ( (ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET) ) 
            {
                // jesli nazwa zaczyna sie na 'e' -> to mamy do czynienia z interfejsem ethernetowym
                // jesli zaczyna sie na 'w' -> wtedy z wireless

                if (ifa->ifa_name[0] == 'e' || ifa->ifa_name[0] == 'w') {
                    hostdevice_t *hd = malloc(sizeof(hostdevice_t));

                    struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                    
                    hd->if_name = (char *) malloc(strlen(ifa->ifa_name) + 1);
                    strcpy(hd->if_name, ifa->ifa_name);

                    for (int i = 0; i < s->sll_halen; i++) 
                        hd->host_dev_mac_addr[i] = s->sll_addr[i];
                        // printf("%02x%c", (nd->net_dev_mac_addr[i]), (i+1!=s->sll_halen)?':':'\n');
                    
                    // TODO:
                    // add_dev_to_list(&dev_list, nd);
                }
            }
            //sprawdzenie czy dany interfejs posiada rowniez adres IP
            
        }
    }
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