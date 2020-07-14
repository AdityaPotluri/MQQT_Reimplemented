#ifndef MQQT_H
#define MQQT_H

#include<stdio.h>

#define MQQT_HEADER_LEN 2
#define MQQT_ACK_LEN    4


#define CONNACK_BYTE  0x20
#define PUBLISH_BYTE  0x30
#define PUBACK_BYTE   0x40
#define PUBREC_BYTE   0x50
#define PUBREL_BYTE   0x60
#define PUBCOMP_BYTE  0x70
#define SUBACK_BYTE   0x90
#define UNSUBACK_BYTE 0xB0
#define PINGRESP_BYTE 0xD0

enum packet_types{
    CONNECT     = 1,
    CONNACK     = 2,
    PUBLISH     = 3,
    PUBAK       = 4,
    PUBREC      = 5,
    PUBREL      = 6,
    PUBCOMP     = 7,
    SUBSCRIBE   = 8,
    SUBACK      = 9,
    UNSUBSCRIBE = 10,
    UNSUBACK    = 11,
    PINGREQ     = 12,
    PINGRESP    = 13,
    DISCONNECT  = 14,
};

enum qos_level { AT_MOST_ONCE , AT_LEAST_ONCE , EXACTLY_ONCE };

union mqtt_header {
    unsigned char byte;
    struct {
        unsigned retain:1;
        unsigned qos   :2;
        unsigned dop   :1;
        unsigned type  :4;

    } bits;
};

struct mqqt_connect {
    union mqqt_header header;
    union {
        unsigned char byte;
        struct {
            int reserved : 1;
            unsigned clean_session:1;
        }
    }
}