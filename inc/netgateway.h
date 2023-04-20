#ifndef _NETGATEWAY_H_
#define _NETGATEWAY_H_

#include <sys/socket.h>
// #include <sys/time.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>

#include "netdev.h"

#define BUFFER_SIZE 4096

void prep_netlink_msg(struct nlmsghdr *netlmsg, char *netlmsgbuf, int msgseq);
hostdevice_t *parse_gateway(struct nlmsghdr *nlhdr, int rcv_bytes, hostdevice_t *nd);
int getgatewayandiface();

#endif // _NETGATEWAY_H_