#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#include "../inc/netdev.h"

# define NI_NUMERICHOST	1

hostdevice_t *device_list = NULL;

void initialize_device_list() {
    struct ifaddrs *ifaddr = NULL, *ifa = NULL;
    char tmp_ip[14];
    struct sockaddr_in an;

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
                    hostdevice_t *hd = initialize_device();

                    struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                    
                    hd->if_name = (char *) malloc(strlen(ifa->ifa_name) + 1);
                    strcpy(hd->if_name, ifa->ifa_name);

                    for (int i = 0; i < s->sll_halen; i++) {
                        hd->host_dev_mac_addr[i] = s->sll_addr[i];
                        // printf("%02x%c", (hd->host_dev_mac_addr[i]), (i + 1 != s->sll_halen) ? ':' : '\n');
                    }
                    add_device_to_list(&device_list, hd);
                }
            }
            //sprawdzenie czy dany interfejs posiada rowniez adres IP
            if ( (ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_INET) ) { 
                if (ifa->ifa_name[0] == 'e' || ifa->ifa_name[0] == 'w') {
                    hostdevice_t *nd = check_dev_data(&device_list, ifa->ifa_name);
                    if(nd) {
                        getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), tmp_ip, sizeof(tmp_ip), 
                                    NULL, 0, NI_NUMERICHOST);

                        inet_aton(tmp_ip, &an.sin_addr);

                        transform_ip(nd->host_dev_ip_addr, an.sin_addr.s_addr);
                    }
                }
            }
            
        }
    }
    loop_devices(&device_list, print_dev_data);
    //zwolnienie struktury interfejsow
    freeifaddrs(ifaddr);
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

void add_device_to_list(hostdevice_t **list, hostdevice_t *hd) {
    hd->next = *list;
    *list = hd;
}

void free_devices() {
    hostdevice_t *p = device_list;
    while(device_list != NULL) {
        p = device_list->next;
        free(device_list->if_name);
        free(device_list);
        device_list = p;
    }
}

void loop_devices(hostdevice_t **list, void(*f)(hostdevice_t *hd)) {
    hostdevice_t *wsk = *list;
    while(wsk != NULL) {
        (*f)(wsk);
        wsk = wsk->next;
    }
}

hostdevice_t *check_dev_data(hostdevice_t **list, char *iname) {
    hostdevice_t *wsk = *list;
    while(wsk != NULL) {
        if(strcmp(wsk->if_name, iname) == 0) {
            return wsk;
        }
        wsk = wsk->next;
    }
    return NULL;
}

void transform_ip(uint8_t *ip, uint32_t ip_decimal) {
    ip[0] = ip_decimal & 0xff;
    ip[1] = (ip_decimal >> 8) & 0xff;
    ip[2] = (ip_decimal >> 16) & 0xff;
    ip[3] = (ip_decimal >> 24) & 0xff;
}


void print_dev_data(hostdevice_t *nd) {
    char mac[20];
    char ip[14];

    snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
        nd->host_dev_mac_addr[0], 
        nd->host_dev_mac_addr[1], 
        nd->host_dev_mac_addr[2], 
        nd->host_dev_mac_addr[3], 
        nd->host_dev_mac_addr[4], 
        nd->host_dev_mac_addr[5]);

    snprintf(ip, sizeof(ip), "%d.%d.%d.%d",
        nd->host_dev_ip_addr[0], 
        nd->host_dev_ip_addr[1], 
        nd->host_dev_ip_addr[2], 
        nd->host_dev_ip_addr[3]);

    //wypisuje dane urzadzenia w postaci czytelnej dla człowieka

    printf("Interface name = %-20s, MAC address = %s, IP address = %s\n", 
        nd->if_name, mac, ip);
}