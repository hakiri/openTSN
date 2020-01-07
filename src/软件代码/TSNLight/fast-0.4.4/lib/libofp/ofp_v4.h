/***************************************************************************
 *            ofp_v4.h
 *
 *  2017/02/27 09:45:02 星期一
 *  Copyright  2017  XuDongLai
 *  <XuDongLai0923@163.com>
 ****************************************************************************/
/*
 * ofp_v4.h
 *
 * Copyright (C) 2017 - XuDongLai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OFP_V4_H__
#define __OFP_V4_H__

#define MAXLINE 20480

#if 0
#define OFP_ASSERT(EXPR)                                                \
        extern int (*build_assert(void))[ sizeof(struct {               \
                    unsigned int build_assert_failed : (EXPR) ? 1 : -1; })]
#endif

#ifdef SWIG
#define OFP_ASSERT(EXPR)        /* SWIG can't handle OFP_ASSERT. */
#elif !defined(__cplusplus)
/* Build-time assertion for use in a declaration context. */
#define OFP_ASSERT(EXPR)                                               
#if 0
        //  extern int (*build_assert(void))[ sizeof(struct {               
          //          unsigned int build_assert_failed : (EXPR) ? 1 : -1; })]
#endif
#else /* __cplusplus */
#include <boost/static_assert.hpp>
#define OFP_ASSERT BOOST_STATIC_ASSERT
#endif /* __cplusplus */


enum{
	OFPMP_REPLY_MORE_NO = 0,
	OFPMP_REPLY_MORE_YES,
};


/* Defines the userspace specific data types
 * for files included from user space. */
typedef unsigned long long uint64,ovs_be64;
typedef long long int64;
typedef unsigned int uint32, uint32_t, ovs_be32;
typedef unsigned short uint16, uint16_t, ovs_be16;
typedef unsigned char uint8, uint8_t;
typedef uint64 __u64, __be64;
typedef uint32 __u32, __be32;
typedef uint16 __u16, __be16;
typedef uint8 __u8;

#if 0
/* Defines the  userspace specific data types for file
 * included within kernel only. */
typedef UINT8 BE8;
typedef UINT16 BE16;
typedef UINT32 BE32;
typedef UINT64 BE64;
#endif

#define ETH_ALEN 6
#define OFP_MAX_TABLE_NAME_LEN 32
#define OFP_MAX_PORT_NAME_LEN  16

enum ofp_version {
    OFP10_VERSION = 0x01,
    OFP11_VERSION = 0x02,
    OFP12_VERSION = 0x03,
    OFP13_VERSION = 0x04,
    OFP14_VERSION = 0x05,
    OFP15_VERSION = 0x06
};


/* The match type indicates the match structure (set of fields that compose the
* match) in use. The match type is placed in the type field at the beginning
* of all match structures. The "OpenFlow Extensible Match" type corresponds
* to OXM TLV format described below and must be supported by all OpenFlow
* switches. Extensions that define other match types may be published on the
* ONF wiki. Support for extensions is optional.
*/
enum ofp_match_type {
	OFPMT_STANDARD = 0, 		/* Deprecated. */
	OFPMT_OXM = 1, 				/* OpenFlow Extensible Match */
};


/* Why is this packet being sent to the controller? */
enum ofp_packet_in_reason {
	OFPR_NO_MATCH = 0, /* No matching flow (table-miss flow entry). */
	OFPR_ACTION = 1, /* Action explicitly output to controller. */
	OFPR_INVALID_TTL = 2, /* Packet has invalid TTL */
};
enum ofputil_port_config {
    /* OpenFlow 1.0 and 1.1 share these values for these port config bits. */
    OFPUTIL_PC_PORT_DOWN    = 1 << 0, /* Port is administratively down. */
    OFPUTIL_PC_NO_RECV      = 1 << 2, /* Drop all packets received by port. */
    OFPUTIL_PC_NO_FWD       = 1 << 5, /* Drop packets forwarded to port. */
    OFPUTIL_PC_NO_PACKET_IN = 1 << 6, /* No send packet-in msgs for port. */
    /* OpenFlow 1.0 only. */
    OFPUTIL_PC_NO_STP       = 1 << 1, /* No 802.1D spanning tree for port. */
    OFPUTIL_PC_NO_RECV_STP  = 1 << 3, /* Drop received 802.1D STP packets. */
    OFPUTIL_PC_NO_FLOOD     = 1 << 4, /* Do not include port when flooding. */
    /* There are no OpenFlow 1.1-only bits. */
};

enum ofputil_port_state {
    /* OpenFlow 1.0 and 1.1 share this values for these port state bits. */
    OFPUTIL_PS_LINK_DOWN   = 1 << 0, /* No physical link present. */
    /* OpenFlow 1.1 only. */
    OFPUTIL_PS_BLOCKED     = 1 << 1, /* Port is blocked */
    OFPUTIL_PS_LIVE        = 1 << 2, /* Live for Fast Failover Group. */
    /* OpenFlow 1.0 only. */
    OFPUTIL_PS_STP_LISTEN  = 0 << 8, /* Not learning or relaying frames. */
    OFPUTIL_PS_STP_LEARN   = 1 << 8, /* Learning but not relaying frames. */
    OFPUTIL_PS_STP_FORWARD = 2 << 8, /* Learning and relaying frames. */
    OFPUTIL_PS_STP_BLOCK   = 3 << 8, /* Not part of spanning tree. */
    OFPUTIL_PS_STP_MASK    = 3 << 8  /* Bit mask for OFPPS10_STP_* values. */
};


/* Features. */
enum netdev_features {
    NETDEV_F_10MB_HD =    1 << 0,  /* 10 Mb half-duplex rate support. */
    NETDEV_F_10MB_FD =    1 << 1,  /* 10 Mb full-duplex rate support. */
    NETDEV_F_100MB_HD =   1 << 2,  /* 100 Mb half-duplex rate support. */
    NETDEV_F_100MB_FD =   1 << 3,  /* 100 Mb full-duplex rate support. */
    NETDEV_F_1GB_HD =     1 << 4,  /* 1 Gb half-duplex rate support. */
    NETDEV_F_1GB_FD =     1 << 5,  /* 1 Gb full-duplex rate support. */
    NETDEV_F_10GB_FD =    1 << 6,  /* 10 Gb full-duplex rate support. */
    NETDEV_F_40GB_FD =    1 << 7,  /* 40 Gb full-duplex rate support. */
    NETDEV_F_100GB_FD =   1 << 8,  /* 100 Gb full-duplex rate support. */
    NETDEV_F_1TB_FD =     1 << 9,  /* 1 Tb full-duplex rate support. */
    NETDEV_F_OTHER =      1 << 10, /* Other rate, not in the list. */
    NETDEV_F_COPPER =     1 << 11, /* Copper medium. */
    NETDEV_F_FIBER =      1 << 12, /* Fiber medium. */
    NETDEV_F_AUTONEG =    1 << 13, /* Auto-negotiation. */
    NETDEV_F_PAUSE =      1 << 14, /* Pause. */
    NETDEV_F_PAUSE_ASYM = 1 << 15, /* Asymmetric pause. */
};


/* Controller roles. */
enum ofp_controller_role {
OFPCR_ROLE_NOCHANGE = 0, /* Don¡¯t change current role. */
OFPCR_ROLE_EQUAL = 1, /* Default role, full access. */
OFPCR_ROLE_MASTER = 2, /* Full access, at most one master. */
OFPCR_ROLE_SLAVE = 3, /* Read-only access. */
};


enum ofp_instruction_type {
	OFPIT_GOTO_TABLE = 1, /* Setup the next table in the lookup
	pipeline */
	OFPIT_WRITE_METADATA = 2, /* Setup the metadata field for use later in
	pipeline */
	OFPIT_WRITE_ACTIONS = 3, /* Write the action(s) onto the datapath action
	set */
	OFPIT_APPLY_ACTIONS = 4, /* Applies the action(s) immediately */
	OFPIT_CLEAR_ACTIONS = 5, /* Clears all actions from the datapath
	action set */
	OFPIT_METER = 6, /* Apply meter (rate limiter) */
	OFPIT_EXPERIMENTER = 0xFFFF /* Experimenter instruction */
};

enum ofp_action_type {
	OFPAT_OUTPUT = 0, /* Output to switch port. */
	OFPAT_COPY_TTL_OUT = 11, /* Copy TTL "outwards" -- from next-to-outermost
	to outermost */
	OFPAT_COPY_TTL_IN = 12, /* Copy TTL "inwards" -- from outermost to
	next-to-outermost */
	OFPAT_SET_MPLS_TTL = 15, /* MPLS TTL */
	OFPAT_DEC_MPLS_TTL = 16, /* Decrement MPLS TTL */
	OFPAT_PUSH_VLAN = 17, /* Push a new VLAN tag */
	OFPAT_POP_VLAN = 18, /* Pop the outer VLAN tag */
	OFPAT_PUSH_MPLS = 19, /* Push a new MPLS tag */
	OFPAT_POP_MPLS = 20, /* Pop the outer MPLS tag */
	OFPAT_SET_QUEUE = 21, /* Set queue id when outputting to a port */
	OFPAT_GROUP = 22, /* Apply group. */
	OFPAT_SET_NW_TTL = 23, /* IP TTL. */
	OFPAT_DEC_NW_TTL = 24, /* Decrement IP TTL. */
	OFPAT_SET_FIELD = 25, /* Set a header field using OXM TLV format. */
	OFPAT_PUSH_PBB = 26, /* Push a new PBB service tag (I-TAG) */
	OFPAT_POP_PBB = 27, /* Pop the outer PBB service tag (I-TAG) */
	OFPAT_REPLACE_PROTO = 33,   /*协议替换*/
	OFPAT_KEEP_RESOURCE = 34,   /*资源预留*/
	OFPAT_EXPERIMENTER = 0xffff
};

/* OXM Class IDs.
* The high order bit differentiate reserved classes from member classes.
* Classes 0x0000 to 0x7FFF are member classes, allocated by ONF.
* Classes 0x8000 to 0xFFFE are reserved classes, reserved for standardisation.
*/
enum ofp_oxm_class {
	OFPXMC_NXM_0 = 0x0000, /* Backward compatibility with NXM */
	OFPXMC_NXM_1 = 0x0001, /* Backward compatibility with NXM */
	OFPXMC_OPENFLOW_BASIC = 0x8000, /* Basic class for OpenFlow */
	OFPXMC_EXPERIMENTER = 0xFFFF, /* Experimenter class */
};


enum ofp_type {
	/* Immutable messages. */
	OFPT_HELLO = 0, 				/* Symmetric message */
	OFPT_ERROR = 1, 				/* Symmetric message */
	OFPT_ECHO_REQUEST = 2, 			/* Symmetric message */
	OFPT_ECHO_REPLY = 3, 			/* Symmetric message */
	OFPT_EXPERIMENTER = 4, 			/* Symmetric message */
	
	/* Switch configuration messages. */
	OFPT_FEATURES_REQUEST = 5, 		/* Controller/switch message */
	OFPT_FEATURES_REPLY = 6, 		/* Controller/switch message */
	OFPT_GET_CONFIG_REQUEST = 7, 	/* Controller/switch message */
	OFPT_GET_CONFIG_REPLY = 8, 		/* Controller/switch message */
	OFPT_SET_CONFIG = 9, 			/* Controller/switch message */
	
	/* Asynchronous messages. */
	OFPT_PACKET_IN = 10, 			/* Async message */
	OFPT_FLOW_REMOVED = 11, 		/* Async message */
	OFPT_PORT_STATUS = 12, 			/* Async message */
	
	/* Controller command messages. */
	OFPT_PACKET_OUT = 13, 			/* Controller/switch message */
	OFPT_FLOW_MOD = 14, 			/* Controller/switch message */
	OFPT_GROUP_MOD = 15, 			/* Controller/switch message */
	OFPT_PORT_MOD = 16, 			/* Controller/switch message */
	OFPT_TABLE_MOD = 17, 			/* Controller/switch message */
	
	/* Multipart messages. */
	OFPT_MULTIPART_REQUEST = 18, 	/* Controller/switch message */
	OFPT_MULTIPART_REPLY = 19, 		/* Controller/switch message */
	
	/* Barrier messages. */
	OFPT_BARRIER_REQUEST = 20, 		/* Controller/switch message */
	OFPT_BARRIER_REPLY = 21, 		/* Controller/switch message */
	
	/* Queue Configuration messages. */
	OFPT_QUEUE_GET_CONFIG_REQUEST = 22, 	/* Controller/switch message */
	OFPT_QUEUE_GET_CONFIG_REPLY = 23, 		/* Controller/switch message */
	
	/* Controller role change request messages. */
	OFPT_ROLE_REQUEST = 24, 				/* Controller/switch message */
	OFPT_ROLE_REPLY = 25, 					/* Controller/switch message */
	
	/* Asynchronous message configuration. */
	OFPT_GET_ASYNC_REQUEST = 26, 			/* Controller/switch message */
	OFPT_GET_ASYNC_REPLY = 27, 				/* Controller/switch message */
	OFPT_SET_ASYNC = 28, 					/* Controller/switch message */
	
	/* Meters and rate limiters configuration messages. */
	OFPT_METER_MOD = 29, 					/* Controller/switch message */
};

enum ofperr {
	OFPERR_TEST=1,
};

enum ofp_flow_mod_command
{
	OFPFC_ADD=0, /* New flow. */
	OFPFC_MODIFY=1, /* Modify all matching flows. */
	OFPFC_MODIFY_STRICT =2, /* Modify entry strictly matching wildcards and priority. */	
	OFPFC_DELETE= 3, /* Delete all matching flows. */
	OFPFC_DELETE_STRICT = 4, /* Delete entry strictly matching wildcards and priority. */
};

/* Table Feature property types.
* Low order bit cleared indicates a property for a regular Flow Entry.
* Low order bit set indicates a property for the Table-Miss Flow Entry.
*/
enum ofp_table_feature_prop_type {
	OFPTFPT_INSTRUCTIONS = 0, 		/* Instructions property. */
	OFPTFPT_INSTRUCTIONS_MISS = 1, /* Instructions for table-miss. */
	OFPTFPT_NEXT_TABLES = 2, /* Next Table property. */
	OFPTFPT_NEXT_TABLES_MISS = 3, /* Next Table for table-miss. */
	OFPTFPT_WRITE_ACTIONS = 4, /* Write Actions property. */
	OFPTFPT_WRITE_ACTIONS_MISS = 5, /* Write Actions for table-miss. */
	OFPTFPT_APPLY_ACTIONS = 6, /* Apply Actions property. */
	OFPTFPT_APPLY_ACTIONS_MISS = 7, /* Apply Actions for table-miss. */
	OFPTFPT_MATCH = 8, /* Match property. */
	OFPTFPT_WILDCARDS = 10, /* Wildcards property. */
	OFPTFPT_WRITE_SETFIELD = 12, /* Write Set-Field property. */
	OFPTFPT_WRITE_SETFIELD_MISS = 13, /* Write Set-Field for table-miss. */
	OFPTFPT_APPLY_SETFIELD = 14, /* Apply Set-Field property. */
	OFPTFPT_APPLY_SETFIELD_MISS = 15, /* Apply Set-Field for table-miss. */
	OFPTFPT_EXPERIMENTER = 0xFFFE, /* Experimenter property. */
	OFPTFPT_EXPERIMENTER_MISS = 0xFFFF, /* Experimenter for table-miss. */
};




enum ofp_port_no {
	/* Maximum number of physical and logical switch ports. */
	OFPP_MAX = 0xffffff00,
	/* Reserved OpenFlow Port (fake output "ports"). */
	OFPP_IN_PORT = 0xfffffff8, /* Send the packet out the input port. This
	 reserved port must be explicitly used
	 in order to send back out of the input
	 port. */
	OFPP_TABLE = 0xfffffff9, /* Submit the packet to the first flow table
	 NB: This destination port can only be
	 used in packet-out messages. */
	OFPP_NORMAL = 0xfffffffa, /* Process with normal L2/L3 switching. */
	OFPP_FLOOD = 0xfffffffb, /* All physical ports in VLAN, except input
	 port and those blocked or link down. */
	OFPP_ALL = 0xfffffffc, /* All physical ports except input port. */
	OFPP_CONTROLLER = 0xfffffffd, /* Send to controller. */
	OFPP_LOCAL = 0xfffffffe, /* Local openflow "port". */
	OFPP_ANY = 0xffffffff
		/* Wildcard port used only for flow mod
		 (delete) and flow stats requests. Selects all flows regardless of output port
			 (including flows with no output port). */
};

enum eth_packet_type {
	//ETH_IPv4 = 0x0800,
	//ETH_ARP = 0x0806,
	//ETH_IPv6 = 0x86DD,
	ETH_P_LLDP = 0x88CC,
	ETH_P_XX = 0x8942,
};


enum ofp_error_type {
	OFPET_HELLO_FAILED = 0, /* Hello protocol failed. */
	OFPET_BAD_REQUEST = 1, /* Request was not understood. */
	OFPET_BAD_ACTION = 2, /* Error in action description. */
	OFPET_BAD_INSTRUCTION = 3, /* Error in instruction list. */
	OFPET_BAD_MATCH = 4, /* Error in match. */
	OFPET_FLOW_MOD_FAILED = 5, /* Problem modifying flow entry. */
	OFPET_GROUP_MOD_FAILED = 6, /* Problem modifying group entry. */
	OFPET_PORT_MOD_FAILED = 7, /* Port mod request failed. */
	OFPET_TABLE_MOD_FAILED = 8, /* Table mod request failed. */
	OFPET_QUEUE_OP_FAILED = 9, /* Queue operation failed. */
	OFPET_SWITCH_CONFIG_FAILED = 10, /* Switch config request failed. */
	OFPET_ROLE_REQUEST_FAILED = 11, /* Controller Role request failed. */
	OFPET_METER_MOD_FAILED = 12, /* Error in meter. */
	OFPET_TABLE_FEATURES_FAILED = 13, /* Setting table features failed. */
	OFPET_EXPERIMENTER = 0xffff		/* Experimenter error messages. */
};

enum ofp_bad_request_code {
	OFPBRC_BAD_VERSION = 0, /* ofp_header.version not supported. */
	OFPBRC_BAD_TYPE = 1, /* ofp_header.type not supported. */
	OFPBRC_BAD_MULTIPART = 2, /* ofp_multipart_request.type not supported. */
	OFPBRC_BAD_EXPERIMENTER = 3, /* Experimenter id not supported (in ofp_experimenter_header or ofp_multipart_request or ofp_multipart_reply). */
	OFPBRC_BAD_EXP_TYPE = 4, /* Experimenter type not supported. */
	OFPBRC_EPERM = 5, /* Permissions error. */
	OFPBRC_BAD_LEN = 6, /* Wrong request length for type. */
	OFPBRC_BUFFER_EMPTY = 7, /* Specified buffer has already been used. */
	OFPBRC_BUFFER_UNKNOWN = 8, /* Specified buffer does not exist. */
	OFPBRC_BAD_TABLE_ID = 9, /* Specified table-id invalid or does not exist. */
	OFPBRC_IS_SLAVE = 10, /* Denied because controller is slave. */
	OFPBRC_BAD_PORT = 11, /* Invalid port. */
	OFPBRC_BAD_PACKET = 12, /* Invalid packet in packet-out. */
	OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13, /* ofp_multipart_request_flags*/
};

/* Header on all OpenFlow packets. */
struct ofp_header{
	uint8_t version; 		/* OFP_VERSION. */
	uint8_t type; 			/* One of the OFPT_ constants. */
	uint16_t length; 		/* Length including this ofp_header. */
	uint32_t xid; 			/* Transaction id associated with this packet.
							   Replies use the same id as was in the request
							   to facilitate pairing. */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_header) == 8);


/* Using this struct instead of a bare array makes an ethernet address field
 * assignable.  The size of the array is also part of the type, so it is easier
 * to deal with. */
struct eth_addr {
    union {
        uint8_t ea[6];
        ovs_be16 be16[3];
    };
}__attribute__((packed));


/* OXM Flow match field types for OpenFlow basic class. */
enum oxm_ofb_match_fields {
	OFPXMT_OFB_IN_PORT = 0, /* Switch input port. */
	OFPXMT_OFB_IN_PHY_PORT = 1, /* Switch physical input port. */
	OFPXMT_OFB_METADATA = 2, /* Metadata passed between tables. */
	OFPXMT_OFB_ETH_DST = 3, /* Ethernet destination address. */
	OFPXMT_OFB_ETH_SRC = 4, /* Ethernet source address. */
	OFPXMT_OFB_ETH_TYPE = 5, /* Ethernet frame type. */
	OFPXMT_OFB_VLAN_VID = 6, /* VLAN id. */
	OFPXMT_OFB_VLAN_PCP = 7, /* VLAN priority. */
	OFPXMT_OFB_IP_DSCP = 8, /* IP DSCP (6 bits in ToS field). */
	OFPXMT_OFB_IP_ECN = 9, /* IP ECN (2 bits in ToS field). */
	OFPXMT_OFB_IP_PROTO = 10, /* IP protocol. */
	OFPXMT_OFB_IPV4_SRC = 11, /* IPv4 source address. */
	OFPXMT_OFB_IPV4_DST = 12, /* IPv4 destination address. */
	OFPXMT_OFB_TCP_SRC = 13, /* TCP source port. */
	OFPXMT_OFB_TCP_DST = 14, /* TCP destination port. */
	OFPXMT_OFB_UDP_SRC = 15, /* UDP source port. */
	OFPXMT_OFB_UDP_DST = 16, /* UDP destination port. */
	OFPXMT_OFB_SCTP_SRC = 17, /* SCTP source port. */
	OFPXMT_OFB_SCTP_DST = 18, /* SCTP destination port. */
	OFPXMT_OFB_ICMPV4_TYPE = 19, /* ICMP type. */
	OFPXMT_OFB_ICMPV4_CODE = 20, /* ICMP code. */
	OFPXMT_OFB_ARP_OP = 21, /* ARP opcode. */
	OFPXMT_OFB_ARP_SPA = 22, /* ARP source IPv4 address. */
	OFPXMT_OFB_ARP_TPA = 23, /* ARP target IPv4 address. */
	OFPXMT_OFB_ARP_SHA = 24, /* ARP source hardware address. */
	OFPXMT_OFB_ARP_THA = 25, /* ARP target hardware address. */
	OFPXMT_OFB_IPV6_SRC = 26, /* IPv6 source address. */
	OFPXMT_OFB_IPV6_DST = 27, /* IPv6 destination address. */
	OFPXMT_OFB_IPV6_FLABEL = 28, /* IPv6 Flow Label */
	OFPXMT_OFB_ICMPV6_TYPE = 29, /* ICMPv6 type. */
	OFPXMT_OFB_ICMPV6_CODE = 30, /* ICMPv6 code. */
	OFPXMT_OFB_IPV6_ND_TARGET = 31, /* Target address for ND. */
	OFPXMT_OFB_IPV6_ND_SLL = 32, /* Source link-layer for ND. */
	OFPXMT_OFB_IPV6_ND_TLL = 33, /* Target link-layer for ND. */
	OFPXMT_OFB_MPLS_LABEL = 34, /* MPLS label. */
	OFPXMT_OFB_MPLS_TC = 35, /* MPLS TC. */
	OFPXMT_OFP_MPLS_BOS = 36, /* MPLS BoS bit. */
	OFPXMT_OFB_PBB_ISID = 37, /* PBB I-SID. */
	OFPXMT_OFB_TUNNEL_ID = 38, /* Logical Port Metadata. */
	OFPXMT_OFB_IPV6_EXTHDR = 39, /* IPv6 Extension Header pseudo-field */
};


struct ofp_oxm{
	uint16_t classname;
	uint8_t has_mask:1,
		filed:7;
	uint8_t length;
};


enum ofp_multipart_type {
	/* Description of this OpenFlow switch.
	* The request body is empty.
	* The reply body is struct ofp_desc. */
	OFPMP_DESC = 0,
	
	/* Individual flow statistics.
	* The request body is struct ofp_flow_stats_request.
	* The reply body is an array of struct ofp_flow_stats. */
	OFPMP_FLOW = 1,
	
	/* Aggregate flow statistics.
	* The request body is struct ofp_aggregate_stats_request.
	* The reply body is struct ofp_aggregate_stats_reply. */
	OFPMP_AGGREGATE = 2,
	
	/* Flow table statistics.
	* The request body is empty.
	* The reply body is an array of struct ofp_table_stats. */
	OFPMP_TABLE = 3,
	
	/* Port statistics.
	* The request body is struct ofp_port_stats_request.
	* The reply body is an array of struct ofp_port_stats. */
	OFPMP_PORT_STATS = 4,
	
	/* Queue statistics for a port
	* The request body is struct ofp_queue_stats_request.
	* The reply body is an array of struct ofp_queue_stats */
	OFPMP_QUEUE = 5,
	
	/* Group counter statistics.
	* The request body is struct ofp_group_stats_request.
	* The reply is an array of struct ofp_group_stats. */
	OFPMP_GROUP = 6,
	
	/* Group description.
	* The request body is empty.
	* The reply body is an array of struct ofp_group_desc. */
	OFPMP_GROUP_DESC = 7,
	
	/* Group features.
	* The request body is empty.
	* The reply body is struct ofp_group_features. */
	OFPMP_GROUP_FEATURES = 8,
	
	/* Meter statistics.
	* The request body is struct ofp_meter_multipart_requests.
	* The reply body is an array of struct ofp_meter_stats. */
	OFPMP_METER = 9,
	
	/* Meter configuration.
	* The request body is struct ofp_meter_multipart_requests.
	* The reply body is an array of struct ofp_meter_config. */
	OFPMP_METER_CONFIG = 10,
	
	/* Meter features.
	* The request body is empty.
	* The reply body is struct ofp_meter_features. */
	OFPMP_METER_FEATURES = 11,
	
	/* Table features.
	* The request body is either empty or contains an array of
	* struct ofp_table_features containing the controller¡¯s
	* desired view of the switch. If the switch is unable to
	* set the specified view an error is returned.
	* The reply body is an array of struct ofp_table_features. */
	OFPMP_TABLE_FEATURES = 12,
	
	/* Port description.
	* The request body is empty.
	* The reply body is an array of struct ofp_port. */
	OFPMP_PORT_DESC = 13,
	
	/* Experimenter extension.
	* The request and reply bodies begin with
	* struct ofp_experimenter_multipart_header.
	* The request and reply bodies are otherwise experimenter-defined. */
	OFPMP_EXPERIMENTER = 0xffff
};


/* Abstract ofp10_phy_port or ofp11_port. */
struct ofp_port {
    uint32_t port_no;
	uint8_t pad[4];
	uint8_t hw_addr[ETH_ALEN];
	uint8_t pad2[2]; /* Align to 64 bits. */
    char name[OFP_MAX_PORT_NAME_LEN];
	
    uint32_t config;
    uint32_t state;

    /* NETDEV_F_* feature bitmasks. */
    uint32_t curr;       /* Current features. */
    uint32_t advertised; /* Features advertised by the port. */
    uint32_t supported;  /* Features supported by the port. */
    uint32_t peer;       /* Features advertised by peer. */

    /* Speed. */
    uint32_t curr_speed;        /* Current speed, in kbps. */
    uint32_t max_speed;         /* Maximum supported speed, in kbps. */
}__attribute__((packed));


/*数据通路的一个物理端口改变 */
struct ofp_port_status {	
	uint8_t reason;
	/*一个 ofppr_*/
	uint8_t pad[7];
	/*对齐到64位。 */
	struct ofp_port desc;
};
OFP_ASSERT(sizeof(struct ofp_port_status) == 80);

/*是什么改变了关于物理端口,reason可以是下列值之一:*/
enum ofp_port_reason {
	OFPPR_ADD = 0, /*端口添加。 */
	OFPPR_DELETE = 1, /*端口被删除*/
	OFPPR_MODIFY = 2, /*某些端口的属性已经改变了。*/
};

/* Fields to match against flows */
struct ofp_packet_in_match {
	uint16_t type; 				/* One of OFPMT_* */
	uint16_t length; 			/* Length of ofp_match (excluding padding) */
	/* Followed by:
	* - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
	* - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
	* all-zero bytes
	* In summary, ofp_match is padded as needed, to make its overall size
	* a multiple of 8, to preserve alignement in structures using it.
	*/
	struct ofp_oxm oxm_idl[0] ; 		/* 0 or more OXM match fields */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_match) == 8);


/* Fields to match against flows */
struct ofp_match {
	uint16_t type; 				/* One of OFPMT_* */
	uint16_t length; 			/* Length of ofp_match (excluding padding) */
	/* Followed by:
	* - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
	* - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
	* all-zero bytes
	* In summary, ofp_match is padded as needed, to make its overall size
	* a multiple of 8, to preserve alignement in structures using it.
	*/
	struct ofp_oxm oxm_idl[0] ; 		/* 0 or more OXM match fields */
	uint8_t pad[4]; 			/* Zero bytes - see above for sizing */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_match) == 8);

/* Action structure for OFPAT_OUTPUT, which sends packets out ¡¯port¡¯.
* When the ¡¯port¡¯ is the OFPP_CONTROLLER, ¡¯max_len¡¯ indicates the max
* number of bytes to send. A ¡¯max_len¡¯ of zero means no bytes of the
* packet should be sent. A ¡¯max_len¡¯ of OFPCML_NO_BUFFER means that
* the packet is not buffered and the complete packet is to be sent to
* the controller. */
struct ofp_action_output {
	uint16_t type; /* OFPAT_OUTPUT. */
	uint16_t len; /* Length is 16. */
	uint32_t port; /* Output port. */
	uint16_t max_len; /* Max length to send to controller. */
	uint8_t pad[6]; /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_action_output) == 16);


/* Instruction header that is common to all instructions. The length includes
* the header and any padding used to make the instruction 64-bit aligned.
* NB: The length of an instruction *must* always be a multiple of eight. */
struct ofp_instruction {
	uint16_t type; /* Instruction type */
	uint16_t len; /* Length of this struct in bytes. */	
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_instruction) == 4);

/* Instruction header that is common to all instructions. The length includes
* the header and any padding used to make the instruction 64-bit aligned.
* NB: The length of an instruction *must* always be a multiple of eight. */
struct ofp_instruction_flow_stats {
	uint16_t type; /* Instruction type */
	uint16_t len; /* Length of this struct in bytes. */
	uint8_t pad[4];
	struct ofp_action_output action_output[0];
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_instruction) == 4);

/* Common header for all Hello Elements */
struct ofp_hello_elem_header {
    ovs_be16    type;        /* One of OFPHET_*. */
    ovs_be16    length;      /* Length in bytes of this element. */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_hello_elem_header) == 4);

/* Version bitmap Hello Element */
struct ofp_hello_elem_versionbitmap {
uint16_t type; /* OFPHET_VERSIONBITMAP. */
uint16_t length; /* Length in bytes of this element. */
/* Followed by:
* - Exactly (length - 4) bytes containing the bitmaps, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
* bytes of all-zero bytes */
uint32_t bitmaps[0]; /* List of bitmaps - supported versions */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_hello_elem_versionbitmap) == 4);


/* OFPT_HELLO. This message includes zero or more hello elements having
 * variable size. Unknown elements types must be ignored/skipped, to allow
 * for future extensions. */
struct ofp_hello {
	//struct ofp_header header;
	/* Hello element list */
	//struct ofp_hello_elem_header elements[0]; /* List of elements - 0 or more */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_hello) == 8);


/* OFPT_ERROR: Error message (datapath -> controller). */
struct ofp_error_msg {
	//struct ofp_header header;
	uint16_t type;
	uint16_t code;
	uint8_t data[0]; 			/* Variable-length data. Interpreted based
								   on the type and code. No padding. */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_error_msg) == 12);

	/* Switch features. */
struct ofp_switch_features {
	//struct ofp_header header;
	uint64_t datapath_id;		 /* Datapath unique ID. The lower 48-bits are for
									a MAC address, while the upper 16-bits are
									implementer-defined. */
	uint32_t n_buffers; 		 /* Max packets buffered at once. */
	uint8_t n_tables; 			 /* Number of tables supported by datapath. */
	uint8_t auxiliary_id; 		 /* Identify auxiliary connections */
	uint8_t pad[2]; 			 /* Align to 64-bits. */
	
	/* Features. */
	uint32_t capabilities;		 /* Bitmap of support "ofp_capabilities". */
	uint32_t reserved;
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_switch_features) == 32);

/* Switch configuration. */
struct ofp_switch_config {
	//struct ofp_header header;
	uint16_t flags; 				/* Bitmap of OFPC_* flags. */
	uint16_t miss_send_len; 		/* Max bytes of packet that datapath
										should send to the controller. See
										ofp_controller_max_len for valid values.*/
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_switch_config) == 12);


/* Packet received on port (datapath -> controller). */
struct ofp_packet_in {
	//struct ofp_header header;
	uint32_t buffer_id; 		/* ID assigned by datapath. */
	uint16_t total_len; 		/* Full length of frame. */
	uint8_t reason; 			/* Reason packet is being sent (one of OFPR_*) */
	uint8_t table_id; 			/* ID of the table that was looked up */
	uint64_t cookie; 			/* Cookie of the flow entry that was looked up. */
	struct ofp_packet_in_match match; 	/* Packet metadata. Variable size. */
	
	/* The variable size and padded match is always followed by:
	* - Exactly 2 all-zero padding bytes, then
	* - An Ethernet frame whose length is inferred from header.length.
	* The padding bytes preceding the Ethernet frame ensure that the IP
	* header (if any) following the Ethernet header is 32-bit aligned.
	*/
	
	//uint8_t pad[2]; 			/* Align to 64 bit + 16 bit */
	//uint8_t data[0]; 			/* Ethernet frame */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_packet_in) == 32);

/* Send packet (controller -> datapath). */
struct ofp_packet_out {
	struct ofp_header header;
	uint32_t buffer_id;
	/* ID assigned by datapath (OFP_NO_BUFFER if none). */
	uint32_t in_port;
	/* Packet’s input port or OFPP_CONTROLLER. */
	uint16_t actions_len;
	/* Size of action array in bytes. */
	uint8_t pad[6];
	struct ofp_action_output actions[0];/* Action list - 0 or more. XDL Modify*/
	//struct ofp_action_header actions[0]; /* Action list - 0 or more. */
	/* The variable size action list is optionally followed by packet data.
	 * This data is only present and meaningful if buffer_id == -1.*/
	 /* uint8_t data[0]; */
	 /* Packet data. The length is inferred
	  from the length field in the header. */
};
OFP_ASSERT(sizeof(struct ofp_packet_out) == 24);


/* Flow removed (datapath -> controller). */
struct ofp_flow_removed {
	//struct ofp_header header;
	uint64_t cookie;				/* Opaque controller-issued identifier. */
	uint16_t priority; 				/* Priority level of flow entry. */
	uint8_t reason; 				/* One of OFPRR_*. */
	uint8_t table_id; 				/* ID of the table */
	uint32_t duration_sec; 			/* Time flow was alive in seconds. */
	uint32_t duration_nsec; 		/* Time flow was alive in nanoseconds beyond
										duration_sec. */
	uint16_t idle_timeout; 			/* Idle timeout from original flow mod. */
	uint16_t hard_timeout; 			/* Hard timeout from original flow mod. */
	uint64_t packet_count;
	uint64_t byte_count;
	struct ofp_match match; 		/* Description of fields. Variable size. */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_flow_removed) == 56);


/* Flow setup and teardown (controller -> datapath). */
struct ofp_flow_mod {
	//struct ofp_header header;
	uint64_t cookie; 			/* Opaque controller-issued identifier. */
	uint64_t cookie_mask; 		/* Mask used to restrict the cookie bits
									that must match when the command is
									OFPFC_MODIFY* or OFPFC_DELETE*. A value
									of 0 indicates no restriction. */
									
	/* Flow actions. */
	uint8_t table_id;	 			/* ID of the table to put the flow in.
										For OFPFC_DELETE_* commands, OFPTT_ALL
										can also be used to delete matching
										flows from all tables. */
	uint8_t command; 				/* One of OFPFC_*. */
	uint16_t idle_timeout;			/* Idle time before discarding (seconds). */
	uint16_t hard_timeout;			/* Max time before discarding (seconds). */
	uint16_t priority; 				/* Priority level of flow entry. */
	uint32_t buffer_id; 			/* Buffered packet to apply to, or
										OFP_NO_BUFFER.
										Not meaningful for OFPFC_DELETE*. */
	uint32_t out_port; 				/* For OFPFC_DELETE* commands, require
										matching entries to include this as an
										output port. A value of OFPP_ANY
										indicates no restriction. */
	uint32_t out_group; 			/* For OFPFC_DELETE* commands, require
										matching entries to include this as an
										output group. A value of OFPG_ANY
										indicates no restriction. */
	uint16_t flags;					/* Bitmap of OFPFF_* flags. */
	uint8_t pad[2];
	struct ofp_match match; 		/* Fields to match. Variable size. */
	
	/* The variable size and padded match is always followed by instructions. */
	struct ofp_instruction instructions[0];		 /* Instruction set - 0 or more.
													The length of the instruction
													set is inferred from the
													length field in the header. */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_flow_mod) == 52);


#define DESC_STR_LEN   256
#define SERIAL_NUM_LEN 32
/* Body of reply to OFPST_DESC request.  Each entry is a NULL-terminated ASCII
 * string. */
struct ofp_desc_stats {
    char mfr_desc[DESC_STR_LEN];       /* Manufacturer description. */
    char hw_desc[DESC_STR_LEN];        /* Hardware description. */
    char sw_desc[DESC_STR_LEN];        /* Software description. */
    char serial_num[SERIAL_NUM_LEN];   /* Serial number. */
    char dp_desc[DESC_STR_LEN];        /* Human readable description of
                                          the datapath. */
};
OFP_ASSERT(sizeof(struct ofp_desc_stats) == 1056);

struct ofp_next_table{
	uint8_t next_table_id;
	uint8_t pad[3];
};

struct ofp_action{
	uint16_t type;
	uint16_t length;
};





/* Common header for all Table Feature Properties */
struct ofp_table_feature_prop_header {
	uint16_t type; /* One of OFPTFPT_*. */
	uint16_t length; /* Length in bytes of this property. */
	union{
		struct ofp_instruction instruction_ids[0]; /* List of instructions */
		struct ofp_next_table next_table_ids[0]; /* List of table ids. */
		struct ofp_action action_ids[0]; /* List of actions */
		struct ofp_oxm oxm_ids[0]; /* Array of OXM headers */
	};
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_header) == 4);

/* Body for ofp_multipart_request of type OFPMP_TABLE_FEATURES./
* Body of reply to OFPMP_TABLE_FEATURES request. */
struct ofp_table_features {
	uint16_t length; 			/* Length is padded to 64 bits. */
	uint8_t table_id; 			/* Identifier of table. Lower numbered tables
								are consulted first. */
	uint8_t pad[5]; 			/* Align to 64-bits. */
	char name[OFP_MAX_TABLE_NAME_LEN];
	uint64_t metadata_match; 	/* Bits of metadata table can match. */
	uint64_t metadata_write; 	/* Bits of metadata table can write. */
	uint32_t config; 			/* Bitmap of OFPTC_* values */
	uint32_t max_entries; 		/* Max number of entries supported. */
	/* Table Feature Property list */
	struct ofp_table_feature_prop_header properties[0]; /* List of properties */
};
OFP_ASSERT(sizeof(struct ofp_table_features) == 64);


/* Body for ofp_multipart_request of type OFPMP_FLOW. */
struct ofp_flow_stats_request {
	uint8_t table_id; 			/* ID of table to read (from ofp_table_stats),
									OFPTT_ALL for all tables. */
	uint8_t pad[3]; 			/* Align to 32 bits. */
	uint32_t out_port; 			/* Require matching entries to include this
									as an output port. A value of OFPP_ANY
									indicates no restriction. */
	uint32_t out_group; 		/* Require matching entries to include this
									as an output group. A value of OFPG_ANY
									indicates no restriction. */
	uint8_t pad2[4]; 			/* Align to 64 bits. */
	uint64_t cookie; 			/* Require matching entries to contain this
									cookie value */
	uint64_t cookie_mask; 		/* Mask used to restrict the cookie bits that
									must match. A value of 0 indicates
									no restriction. */
	struct ofp_match match; 	/* Fields to match. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_flow_stats_request) == 40);

/* Body of reply to OFPMP_FLOW request. */
struct ofp_flow_stats {
	uint16_t length; 			/* Length of this entry. */
	uint8_t table_id; 			/* ID of table flow came from. */
	uint8_t pad;
	uint32_t duration_sec; 		/* Time flow has been alive in seconds. */
	uint32_t duration_nsec; 	/* Time flow has been alive in nanoseconds beyond
									duration_sec. */
	uint16_t priority; 			/* Priority of the entry. */
	uint16_t idle_timeout; 		/* Number of seconds idle before expiration. */
	uint16_t hard_timeout; 		/* Number of seconds before expiration. */
	uint16_t flags; 			/* Bitmap of OFPFF_* flags. */
	uint8_t pad2[4]; 			/* Align to 64-bits. */
	uint64_t cookie; 			/* Opaque controller-issued identifier. */
	uint64_t packet_count; 		/* Number of packets in flow. */
	uint64_t byte_count; 		/* Number of bytes in flow. */
	struct ofp_match match; 	/* Description of fields. Variable size. */
	/* The variable size and padded match is always followed by instructions. */
	struct ofp_instruction_flow_stats instructions[0]; /* Instruction set - 0 or more. */
};
OFP_ASSERT(sizeof(struct ofp_flow_stats) == 56);

/* Body for ofp_multipart_request of type OFPMP_AGGREGATE. */
struct ofp_aggregate_stats_request {
	uint8_t table_id; 		/* ID of table to read (from ofp_table_stats)
								OFPTT_ALL for all tables. */
	uint8_t pad[3]; 		/* Align to 32 bits. */
	uint32_t out_port; 		/* Require matching entries to include this
								as an output port. A value of OFPP_ANY
								indicates no restriction. */
	uint32_t out_group; 	/* Require matching entries to include this
								as an output group. A value of OFPG_ANY
								indicates no restriction. */
	uint8_t pad2[4]; 		/* Align to 64 bits. */
	uint64_t cookie; 		/* Require matching entries to contain this
							cookie value */
	uint64_t cookie_mask; 	/* Mask used to restrict the cookie bits that
								must match. A value of 0 indicates
								no restriction. */
	struct ofp_match match; /* Fields to match. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_aggregate_stats_request) == 40);

/* Body of reply to OFPMP_AGGREGATE request. */
struct ofp_aggregate_stats_reply {
	uint64_t packet_count; 		/* Number of packets in flows. */
	uint64_t byte_count; 		/* Number of bytes in flows. */
	uint32_t flow_count; 		/* Number of flows. */
	uint8_t pad[4]; 			/* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_aggregate_stats_reply) == 24);

/* Body of reply to OFPMP_PORT request. If a counter is unsupported, set
* the field to all ones. */
struct ofp_port_stats {
	uint32_t port_no;
	uint8_t pad[4]; 			/* Align to 64-bits. */
	uint64_t rx_packets; 		/* Number of received packets. */
	uint64_t tx_packets; 		/* Number of transmitted packets. */
	uint64_t rx_bytes; 			/* Number of received bytes. */
	uint64_t tx_bytes; 			/* Number of transmitted bytes. */
	uint64_t rx_dropped; 		/* Number of packets dropped by RX. */
	uint64_t tx_dropped; 		/* Number of packets dropped by TX. */
	uint64_t rx_errors; 		/* Number of receive errors. This is a super-set
									of more specific receive errors and should be
									greater than or equal to the sum of all
									rx_*_err values. */
	uint64_t tx_errors; 		/* Number of transmit errors. This is a super-set
									of more specific transmit errors and should be
									greater than or equal to the sum of all
									tx_*_err values (none currently defined.) */
	uint64_t rx_frame_err; 		/* Number of frame alignment errors. */
	uint64_t rx_over_err; 		/* Number of packets with RX overrun. */
	uint64_t rx_crc_err; 		/* Number of CRC errors. */
	uint64_t collisions; 		/* Number of collisions. */
	uint32_t duration_sec; 		/* Time port has been alive in seconds. */
	uint32_t duration_nsec; 	/* Time port has been alive in nanoseconds beyond
									duration_sec. */
};
OFP_ASSERT(sizeof(struct ofp_port_stats) == 112);

/* Body of reply to OFPMP_TABLE request. */
struct ofp_table_stats {
	uint8_t table_id;
	/* Identifier of table. Lower numbered tables are consulted first. */
	uint8_t pad[3];/* Align to 32-bits. */
	uint32_t active_count;/* Number of active entries. */
	uint64_t lookup_count;/* Number of packets looked up in table. */
	uint64_t matched_count;/* Number of packets that hit table. */
};
OFP_ASSERT(sizeof(struct ofp_table_stats) == 24);


/* Body of reply to OFPMP_GROUP_FEATURES request. Group features. */
struct ofp_group_features {
	uint32_t types;			/* Bitmap of OFPGT_* values supported. */
	uint32_t capabilities;	/* Bitmap of OFPGFC_* capability supported. */
	uint32_t max_groups[4];	/* Maximum number of groups for each type. */
	uint32_t actions[4];	/* Bitmaps of OFPAT_* that are supported. */
};
OFP_ASSERT(sizeof(struct ofp_group_features) == 40);

struct ofp_multipart {
	//struct ofp_header header;
	uint16_t type; 					/* One of the OFPMP_* constants. */
	uint16_t flags; 				/* OFPMPF_REQ_* flags. */
	uint8_t pad[4];
	union{
		struct ofp_table_stats table_stats[0];
		struct ofp_desc_stats ofpmp_desc[0];
		struct ofp_port ofpmp_port_desc[0];
		struct ofp_port_stats ofpmp_port_stats[0];
		struct ofp_flow_stats_request ofp_flow_stats_request[0];
		struct ofp_flow_stats ofp_flow_stats[0];
		struct ofp_aggregate_stats_reply ofpmp_aggregate_reply[0];
		struct ofp_aggregate_stats_request ofpmp_aggregate_request[0]; 
		struct ofp_table_features ofpmp_table_features[0];
		uint8_t body[0]; 			/* Body of the request. 0 or more bytes. */
	};
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_multipart_request) == 16);

enum ofp_multipart_request_flags {
	OFPMPF_REQ_MORE = 1 << 0 /* More requests to follow. */
};
#if 0
/* Role request and reply message. */
struct ofp_controller_role{
	//struct ofp_header header; 		/* Type OFPT_ROLE_REQUEST */
	uint32_t role; 					/* One of OFPCR_ROLE_*. */
	uint8_t pad[4]; 				/* Align to 64 bits. */
	uint64_t generation_id; 		/* Master Election Generation Id */
}__attribute__((packed));
OFP_ASSERT(sizeof(struct ofp_controller_role) == 24);
#endif



/* Role request and reply message. */
struct ofp_role {
	//struct ofp_header header; /* Type OFPT_ROLE_REQUEST/OFPT_ROLE_REPLY. */
	uint32_t role; /* One of OFPCR_ROLE_*. */
	uint8_t pad[4]; /* Align to 64 bits. */
	uint64_t generation_id; /* Master Election Generation Id */
};
OFP_ASSERT(sizeof(struct ofp_role_request) == 24);

#endif //__OFP_V4_H__