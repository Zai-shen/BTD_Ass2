#ifndef TCP_CLIENT_V4_H
#define TCP_CLIENT_V4_H

void send_and_receive(int sock, char *host_ip, char *rx_buffer, int *error_count);
void close_sock_and_restart(int sock);
void tcp_client(void);

#endif /* TCP_CLIENT_V4_H */