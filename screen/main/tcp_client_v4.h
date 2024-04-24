#ifndef TCP_CLIENT_V4_H
#define TCP_CLIENT_V4_H

void send_and_receive(void);
void close_sock_and_restart(int sock);
void tcp_connect_ver4(void);

#endif /* TCP_CLIENT_V4_H */