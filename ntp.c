#include "ntp.h"

#define NTP_PCK_LEN     (48)
#define LI              (0)
#define VN              (3)
#define MODE            (3)
#define STRATUM         (0)
#define POLL            (4)
#define PREC            (-6)

#define JAN_1970	(0x83aa7e80) /* 1900年～1970年之间的时间秒数 */
#define NTPFRAC(x)	(4294*(x)+((1981*(x))>>11))
#define USEC(x)		(((x)>>12)-759*((((x)>>10)+32768)>>16))

static uint32_t ntp_htonl(uint32_t val)
{
    uint32_t retval;

    retval = (uint8_t) (val >> 0);
    retval <<= 8;
    retval |= (uint8_t) (val >> 8);
    retval <<= 8;
    retval |= (uint8_t) (val >> 16);
    retval <<= 8;
    retval |= (uint8_t) (val >> 24);
    return (retval);
}

int ntp_packet_build(char *buff, uint16_t size, int version)
{
    uint32_t utmp32;

    if (!buff || size < NTP_PCK_LEN)
        return (-1);

    if (version <= 0 || version > 4)
        version = 1;/* NTP/V1 */

    memset(buff, 0, NTP_PCK_LEN);
    utmp32 = ntp_htonl((LI << 30) | (version << 27) | (MODE << 24) | (STRATUM << 16) | (POLL << 8) | (PREC & 0xFF));
    memcpy(buff, &utmp32, sizeof(utmp32));

    /* 设置Root Delay、Root Dispersion和Reference Indentifier */
    utmp32 = ntp_htonl(1 << 16);
    memcpy(&buff[4], &utmp32, sizeof(utmp32));
    memcpy(&buff[8], &utmp32, sizeof(utmp32));

    /* 设置Timestamp部分 */
    /* 设置Transmit Timestamp coarse */
    utmp32 = ntp_htonl(JAN_1970 + 0);
    memcpy(&buff[40], &utmp32, sizeof(utmp32));

    /*设置Transmit Timestamp fine*/
    utmp32 = ntp_htonl(NTPFRAC(0));
    memcpy(&buff[44], &utmp32, sizeof(utmp32));
    return (NTP_PCK_LEN);
}

time_t ntp_parse_sec(char *buff, uint16_t len)
{
    time_t coarse;
    time_t fine;

    if (!buff || len < NTP_PCK_LEN)
        return (NULL);

    coarse = ntp_htonl(*(int*) &(buff[40])) - JAN_1970;/* 返回的时间为1900开始 */
    fine = ntp_htonl(*(int*) &(buff[44]));
    return (coarse);
}
