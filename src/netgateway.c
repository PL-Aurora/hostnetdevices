#include "../inc/netgateway.h"
#include "../inc/netdev.h"


void prep_netlink_msg(struct nlmsghdr *netlmsg, char *netlmsgbuf, int msgseq) {
    memset(netlmsgbuf, 0, BUFFER_SIZE);
    
    netlmsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    netlmsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .
    netlmsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
    netlmsg->nlmsg_seq = msgseq++; // Sequence of the message packet.
    netlmsg->nlmsg_pid = getpid(); // PID of process sending the request.
}

hostdevice_t *parse_gateway(struct nlmsghdr *nlhdr, int rcv_bytes, hostdevice_t *nd) {
    int route_attribute_len = 0;
    struct rtmsg *route_entry;
    struct rtattr *route_attribute;

    char gateway_address[INET_ADDRSTRLEN]; 
    char interface[IF_NAMESIZE];
    

    for ( ; NLMSG_OK(nlhdr, rcv_bytes); nlhdr = NLMSG_NEXT(nlhdr, rcv_bytes))
    {
        /* Get the route data */
        route_entry = (struct rtmsg *) NLMSG_DATA(nlhdr);

        /* We are just interested in main routing table */
        if (route_entry->rtm_table != RT_TABLE_MAIN)
            continue;

        route_attribute = (struct rtattr *) RTM_RTA(route_entry);
        route_attribute_len = RTM_PAYLOAD(nlhdr);
        
        /* Loop through all attributes */
        for ( ; RTA_OK(route_attribute, route_attribute_len);
              route_attribute = RTA_NEXT(route_attribute, route_attribute_len))
        {
            switch(route_attribute->rta_type) {
            case RTA_OIF:
                nd = check_dev_data(&device_list, if_indextoname(*(int *)RTA_DATA(route_attribute), interface));
                if(nd && gateway_address != NULL) 
                    memcpy(nd->host_gtw_ip_addr, gateway_address, sizeof(gateway_address));
                break;
            case RTA_GATEWAY:
                if (nd != NULL)
                    inet_ntop(AF_INET, RTA_DATA(route_attribute), nd->host_gtw_ip_addr, sizeof(nd->host_gtw_ip_addr));
                else {
                    memset(gateway_address, 0, sizeof(gateway_address));
                    inet_ntop(AF_INET, RTA_DATA(route_attribute), gateway_address, sizeof(gateway_address));
                }
                break;
            default:
                break;
            }
        }
    }

    return nd;
}


/* jedna z drog komunikacji miedzy kernelem a uzytkownikiem jest 
uzycie socketow. sockety AF_NETLINK tworza interfejs pomiedzy API kernela
a procesami uzytkownika */

int getgatewayandiface()
{
    if(device_list == NULL) {
        printf("devlist is null. no possible gateway to determine.\n");
        return EXIT_FAILURE;
    }

    int received_bytes = 0, msg_len = 0;
    int sock = -1;
    unsigned int msgseq = 0;
    struct nlmsghdr *nlh, *nlmsg;

    char msgbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
    char *ptr = buffer;

    /* utworzenie gniazda AF_NETLINK ktore pozwoli na komunikacje z jadrem*/
    /* NETLINK_ROUTE - pozyskuje dane o routingu */
    if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    memset(buffer, 0, sizeof(buffer));

    nlmsg = (struct nlmsghdr *) msgbuf;
    prep_netlink_msg(nlmsg, msgbuf, msgseq);

    if (send(sock, nlmsg, nlmsg->nlmsg_len, 0) < 0) {
        perror("send failed");
        return EXIT_FAILURE;
    }

    /* odczytanie odpowiedzi z NETLINK_ROUTE */
    do
    {
        received_bytes = recv(sock, ptr, sizeof(buffer) - msg_len, 0);
        if (received_bytes < 0) {
            perror("Blad recv");
            return EXIT_FAILURE;
        }

        nlh = (struct nlmsghdr *) ptr;

        if((NLMSG_OK(nlmsg, received_bytes) == 0) || (nlmsg->nlmsg_type == NLMSG_ERROR)) {
            perror("blad w pakiecie");
            return EXIT_FAILURE;
        }

        if (nlh->nlmsg_type == NLMSG_DONE)
            break;
        else {
            ptr += received_bytes;
            msg_len += received_bytes;
        }

        if ((nlmsg->nlmsg_flags & NLM_F_MULTI) == 0)
            break;
    }
    while ((nlmsg->nlmsg_seq != msgseq) || (nlmsg->nlmsg_pid != getpid()));

    hostdevice_t *nd = NULL;

    /* odczytanie odpowiedzi i okreslenie gateway */
    nd = parse_gateway(nlh, received_bytes, nd);
    
    printf("Gateway %s\n", nd->host_gtw_ip_addr);
    close(sock);

    return 0;
}
