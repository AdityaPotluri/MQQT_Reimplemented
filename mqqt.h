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

// different levels of quality assurance describing how many packets are published with EXACTLY_ONCE being the most reliable and desirable for MQQT
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

struct mqqt_unsubscribe {
    union mqqt_header header;
    unsigned short pkt_id;
    unsigned short tuples_len;
    struct {
        unsigned short topic_len;
        unsigned char *topic;
    } *tuples_len;
};

struct mqqt_suback
{
    union mqqt_header header;
    unsigned short pkt_id;
    unsigned short topic_len;
    unsigned char *topic;
    unsigned char payloadlen;
    unsigned char *payload;
};

// generic template for a group of packets that simply acknowledge or respond without a payload or other information
struct mqqt_ack
{
    union mqqt_header header;
    unsigned short pkt_id;
};


//these packets 
typedef struct mqtt_ack mqtt_puback;
typedef struct mqtt_ack mqtt_pubrec;
typedef struct mqtt_ack mqtt_pubrel;
typedef struct mqtt_ack mqtt_pubcomp;
typedef struct mqtt_ack mqtt_unsuback;

typedef union mqtt_header mqtt_pingreq;
typedef union mqtt_header mqtt_pingresp;
typedef union mqtt_header mqtt_disconnect;



union mqtt_packet {
    struct mqtt_ack ack;
    union mqtt_header header;
    struct mqtt_connect connect;
    struct mqtt_connack connack;
    struct mqtt_suback suback;
    struct mqtt_publish publish;
    struct mqtt_subscribe subscribe;
    struct mqtt_unsubscribe unsubscribe;
};

//two methods for encoding and decoding (will support serialization)
int mqqt_encode_length(unsigned char *, size_t);
unsigned long long mqqt_decode_length(const unsigned char **);

//two methods for serializing and or marshalling data 
int unpack_mqqt_packet(const unsigned char *, union mqqt_packet *);
unsigned char *pack_mqqt_packet(const union mqqt_packet *, unsigned);


//basically "constructors" to build packets 
union mqtt_header *mqtt_packet_header(unsigned char);
struct mqtt_ack *mqtt_packet_ack(unsigned char , unsigned short);
struct mqtt_connack *mqtt_packet_connack(unsigned char ,
                                         unsigned char ,
                                         unsigned char);
struct mqtt_suback *mqtt_packet_suback(unsigned char, unsigned short,
                                       unsigned char *, unsigned short);
struct mqtt_publish *mqtt_packet_publish(unsigned char, unsigned short, size_t,
                                         unsigned char *,
                                         size_t, unsigned char *);

//releases heap allocated packets
void mqtt_packet_release(union mqtt_packet *, unsigned);


#endif

