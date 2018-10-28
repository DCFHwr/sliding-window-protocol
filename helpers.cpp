#include "helpers.h"

size_t create_frame(unsigned int seq_num, char *frame, char *data, size_t data_size) {
    frame[0] = 0x1;
    uint32_t net_seq_num = htonl(seq_num);
    uint32_t net_data_size = htonl(data_size);
    memcpy(frame + 1, &net_seq_num, 4);
    memcpy(frame + 5, &net_data_size, 4);
    memcpy(frame + 9, data, data_size);
    frame[9 + data_size] = 0x1;

    return data_size + (size_t)10;
}

void create_ack(unsigned int seq_num, char *ack, bool error) {
    ack[0] = error ? 0x0 : 0x1;
    uint32_t net_seq_num = htonl(seq_num);
    memcpy(ack + 1, &net_seq_num, 4);
    ack[5] = 0x1;
}

bool read_frame(unsigned int *seq_num, char *data, size_t *data_size, char *frame) {
    uint32_t net_seq_num;
    memcpy(&net_seq_num, frame + 1, 4);
    *seq_num = ntohl(net_seq_num);

    uint32_t net_data_size;
    memcpy(&net_data_size, frame + 5, 4);
    *data_size = ntohl(net_data_size);

    memcpy(data, frame + 9, *data_size);

    return frame[9 + *data_size] != 0x1;
}

bool read_ack(unsigned int *seq_num, bool *error, char *ack) {
    *error = ack[0] == 0x0 ? true : false;

    uint32_t net_seq_num;
    memcpy(&net_seq_num, ack + 1, 4);
    *seq_num = ntohl(net_seq_num);

    return ack[5] != 0x1;
}