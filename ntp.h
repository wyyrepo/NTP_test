#ifndef NTP_H
#define NTP_H
#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#include <stdint.h>
#include <time.h>

typedef struct
{
    uint32_t coarse;
    uint32_t fine;
} ntp_time_t;

typedef struct
{
    uint32_t h32bit;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t ref_identifier;
    ntp_time_t ref_timestamp;
    ntp_time_t orig_timestamp;
    ntp_time_t recv_timestamp;
    ntp_time_t trans_timestamp;
    /* Key identifier(optional) 32bit */
    /* Message digest(optional) 128bit */
} ntp_packet_t;

int ntp_packet_build(char *buff, uint16_t size, int version);
time_t ntp_parse_sec(char *buff, uint16_t len);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif // NTP_H
