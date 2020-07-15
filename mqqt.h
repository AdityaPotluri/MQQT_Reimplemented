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

/* 
    CONNECT is the packet which requests a method to the server
    CONNACK is the packet which acknowledges the connection to the server
    PUBLISH "publishes" the message to the server
    PUBAK acknowledges the publishing of the server
    PUBREC responds to the server with something called QOS2 which is the quality of assurance that the packet has been delivered exactly once
    PUBREL or publish release responds to PUBREC with the same packet identifier as PUBREC
    PUBCOMP is the packet which responds to PUBREL and contains the same packet identifier to signal that it is being acknowledged
    SUBSCRIBE is the packet which requests to create a subscription to a certain topic
    SUBACK acknowledges the SUBSCRIBE packet
    UNSUBSCRIBE unsubscribes from certain topics 
    UNSUBACK acknowledges the unsubscription and as always with acknowledgements makes sure to have the same packet identifier
    PINGREQ the pingreq can be used to indicate the client, server , and network connection is alive
    PINGRESP responds to the PINGREQ
    DISCONNECT is the final packet sent from client to the server to show that the client is disconnecting early






*/
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

// different levels of quality assurance describing how many packets are published with EXACTLY_ONCE being the most reliable and desirable for MQTT
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
            unsigned clean_session : 1;
            unsigned will : 1;
            unsigned will_qos : 1;
            unsigned will_retain : 1;
            unsigned password : 1;
            unsigned username : 1;
        } bits;

    };

    struct {
        unsigned short keepalive;
        unsigned char *client_id;
        unsigned char *username;
        unsigned char *password;
        unsigned char *will_topic;
        unsigned char *will_message;
    } payload;

};
struct mqqt_connack {
    union mqqt_header header;
    unsigned short pkt_id;
    union{
        unsigned char byte;
        struct {
            unsigned flags : 7;
            unsigned session_present:1;
        } connection_flags;
    };

    unsigned char rc;
    
};
struct mqqt_subscribe {
    union mqqt_header header;
    unsigned short pkt_id;
    unsigned short tuples_len;
    struct {
        unsigned short topic_len;
        unsigned char *topic;
        unsigned qos;
    } *tuples;
};








