/** *************************************************************************
 *  @file          fast_struct.h
 *  @brief		FAST平台的数据类型（结构）定义
 * 
 *  FAST平台主要的数据结构类型主要包括：枚举的消息类型、动作类型；报文数据结构、
 *  流表数据结构；回调函数类型等。
 * 
 *  @date		2017/04/09 12:25:57 星期日
 *  @author		XDL(Copyright  2017  XuDongLai)
 *  @email		<XuDongLai0923@163.com>
 *  @version	0.2.0
 ****************************************************************************/
/*
 * fast_struct.h
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
#ifndef __FAST_STRUCT_H__
#define __FAST_STRUCT_H__

#pragma pack(1)
/*-------------------------NetMagic 08----------------------------------*/
/** 
 * @brief NetMagic08的访问控制（NMAC）报文的报文类型枚举定义
 * 该枚举类型主要包括软件与NetMagic08之间的NMAC通信报
 * 文的所有类型
 */
typedef enum{	
	NM_CONN = 1,	/**< NMAC的连接报文*/	
	NM_RELESE,		/**< NMAC的释放（关闭）报文*/
	NM_REG_RD,		/**< NMAC的读请求报文*/	
	NM_REG_WR,		/**< NMAC的写操作报文*/	
	NM_RD_RPL,		/**< NMAC的读回应报文*/	
	NM_WR_RPL,		/**< NMAC的写回应报文*/
}NMAC_TYPE;


/** 
 * @brief NetMagic08访问控制（NMAC）报文的头部结构定义
 */
struct nm_head
{
	u8 count;			/**< @brief NMAC报文头的计数值，表示后面的读写操作包含多少个*/
    u8 reserve8_A;	    /**< @brief 暂时保留不用*/
    u16 seq;			/**< @brief NMAC报文头的序号，每发一个报文计数加1*/
    u16 reserve16_B;	/**< @brief 报文计数值*/
    u8 type;			/**< @brief NMAC报文头的类型，@see ::NMAC_TYPE*/
    u8 parameter_1;		/**< @brief 报文计数值1*/
	u8 parameter_2;		/**< @brief 报文计数值2*/
    u8 reserve8_C;		/**< @brief 报文计数值*/
};

/** 
 * @brief NMAC报文的完整定义
 * 
 * NMAC报文由NMAC头、寄存器地址和寄存器值三部分组成。
 */
struct nm_packet
{	
	struct nm_head nm;  /**< @brief NMAC报文头*/	
	u64 regaddr;		/**< @brief 读或写的寄存器地址*/	
	u64 regvalue;		/**< @brief 读回或写入的寄存器地址对应的值*/	
};

/*-------------------------NetMagic 08----------------------------------*/


/*-------------------------UA----------------------------------*/
//UA
/** 
 * @brief UA注册的NetLink消息类型
 */
#define FAST_UA_NETLINK 23
/** 
 * @brief UA注册的NetLink消息长度
 */
#define FAST_UA_REG_LEN 32
/** 
 * @brief UA通信的NetLink消息最大长度
 */
#define FAST_UA_PKT_MAX_LEN 2048

/** 
 * @brief 支持UA注册的最大个数
 */
#define UA_MAX_MID_CNT 256

/** 
 * @brief UA模块与内核交互消息定义
 * 消息内型主要包括：注册、注销等类型
 */
typedef enum {	
	UA_REG = 21,	/**< UA注册*/	
	UA_UNREG = 32,  /**< UA注销*/	
	UA_OK = 26,		/**< UA操作成功*/
	UA_ERR  = 37	/**< UA操作失败*/
}UA_STATE;

/*-------------------------UA----------------------------------*/
/** 
 * @brief FAST平台硬件的ACTION动作类型枚举定义
 * 类型定义主要包括：硬件丢弃、设置报文进程号（队列号／端口号）、
 * 轮循分派到各个进程（队列／端口）、硬件端口输出、设置报文目的模块ID、
 * 设置RTP报文队列等
 */
typedef enum{
        ACTION_DROP = 0,		/**< 报文在硬件,由FPGA处理：丢弃报文*/
        ACTION_SET_PID = 1,		/**< 报文送软件,由CPU处理：设置报文的处理线程ID号(处理线程数则驱动模块参数决定,默认从0开始,不能超过线程最大值)*/
        ACTION_POLL = 2,		/**< 报文送软件,由CPU处理：将报文循环分派到处理线程(处理线程数则驱动模块参数决定,在指定线程数内循环派送)*/
        ACTION_PORT = 3,		/**< 报文在硬件,由FPGA处理：从指定硬件物理端口输出报文*/
        ACTION_SET_MID = 4,		/**< 报文送软件,由CPU处理：设置报文分派的模块ID号*/
		ACTION_SET_QUEUE_RTP = 5,/**< 支持传媒RTP视频流的队列调度*/
		ACTION_REPLACE_PROTO = 6,/**< 支持协议替换操作*/
		ACTION_KEEP_RESOURCE = 7,/**< 支持流在端口转发时的资源预留*/
}ACTION;

/*-----------------------------------流表相关----------------------------------------------*/

/** 
 * @brief FAST平台硬件流表中流的格式定义
 */
struct flow /*2017/06/01开始启用，支持IPv6*/
{	
	u8  dmac[ETH_ALEN];	/**< @brief Ethernet source address. */
	u8  smac[ETH_ALEN];	/**< @brief Ethernet destination address. */
	u16 tci;			/**< @brief 0 if no VLAN, VLAN_TAG_PRESENT set otherwise. */
	u16 type;			/**< @brief Ethernet frame type. */
	//eth-16;
	
	u8  proto;			/**< @brief IP protocol or lower 8 bits of ARP opcode. */
	u8  tos;			/**< @brief IP ToS. */
	u8  ttl;			/**< @brief IP TTL/hop limit. */
	u8  frag:4,			/**< @brief One of OVS_FRAG_TYPE_*. */
		port:4;			/**< @brief Input Port*/
	//ip=16+4;
	
	union {
		struct {			
			u32 src;	/**< @brief IP source address. */
			u32 dst;	/**< @brief IP destination address. */
			//addr;
			
			union {
				struct {
					u16 sport;		/**< @brief TCP/UDP/SCTP source port. */
					u16 dport;		/**< @brief TCP/UDP/SCTP destination port. */
					u16 flags;		/**< @brief TCP flags. */
				} tp;				/**< @brief IPv4的传输层端口与标志信息*/
				struct {
					u8 sha[ETH_ALEN];	/**< @brief ARP source hardware address. */
					u8 tha[ETH_ALEN];	/**< @brief ARP target hardware address. */
				} arp;					/**< @brief ARP的MAC地址信息*/
			};							/**< @brief IPv4传输层信息与ARP信息共用体，二者互斥存在*/
		} ipv4;							/**< @brief IPv4协议相关字段*/
		struct {
			struct in6_addr src;	/**< @brief IPv6 source address. */
			struct in6_addr dst;	/**< @brief IPv6 destination address. */
			//addr;
			
			u32 label;				/**< @brief IPv6 flow label. */
			union {
				struct {
					u16 sport;		/**< @brief TCP/UDP/SCTP source port. */
					u16 dport;		/**< @brief TCP/UDP/SCTP destination port. */
					u16 flags;		/**< @brief TCP flags. */
				} tp;				/**< @brief IPv6的传输层端口与标志信息*/
				/*512位宽表项不够，暂不使用ND协议中的targetIP!!!为方便以后扩展，将其移至末尾*/
#if 0
				struct {					
					u8 sll[ETH_ALEN];	/**< @brief ND source link layer address. */
					u8 tll[ETH_ALEN];	/**< @brief ND target link layer address. */					
					struct in6_xaddr target;	/**< @brief ND target address. */
				} nd;
#endif
			};						/**< @brief IPv6传输层信息与ND信息共用体，二者互斥存在*/
			u16 nil;
		} ipv6;						/**< @brief IPv6协议相关字段*/
	};
}; 
#if 0
struct flow_old /*2017/06/01之前版本使用，硬件仅支持OF1.0，不支持IPv6等*/
{
	u8 dmac[6]; /* Ethernet destination address. */
	u8 smac[6]; /* Ethernet source address. */
	u32 tag; /* 北邮使用的TAG字段，4个字节. */
/*16B,128b*/	
	u16 type; /* Ethernet frame type. */
	u8 tos; /* IP ToS (actually DSCP field, 6 bits). */
	u8 proto; /* IP protocol or lower 8 bits of ARP opcode. */
	u32 src; /* IP source address. */
	u32 dst; /* IP destination address. */
	u16 sport; /* TCP/UDP source port. */
	u16 dport; /* TCP/UDP destination port. */
/*32B,256b*/		
	u16 port; /* Input switch port. */
/*34B,272b*/
	u8 pad[6];/*ADD align:2017/02/28 15:26 ADD*/
	u64 cookie;/*ADD cookie:2017/02/28 15:26 ADD*/
	u8 pad2[16];/*30->16:2017/02/28 15:26 ADD*/
/*64B,512b*/
/*----------------------RULE 512bit--------------------------*/
}__attribute__((packed));
struct um_metadata{
#if defined(__LITTLE_ENDIAN) /*INTER*/	
	u64 ts;				/*报文的接收时间戳*/
	u64 seq:16,			/*报文接收的序号*/
		reserve:12,		/*暂时保留未用*/
		dstmid:8,		/*报文分派的目的模块编号*/
		srcmid:8,		/*报文处理的源模块编号*/
		len:12,			/*报文的总长度,表示为:UM数据结构大小+2字节IP对齐+完整以太网报文大小*/
		inout:8;		/*报文的输入/输出端口号,接收时表示输入,发送时表示输出*/
	u64 none[2];		/*用户自定义预留*/
#elif defined(__BIG_ENDIAN)  /**/	
	u64 seq:16,
		reserve:12,
		dstmid:8,
		srcmid:8,
		len:12,
		inout:8;	
	u64 ts;
	u64 none[2];
	#else
#error	"Please fix <asm/byteorder.h>"
#endif
}__attribute__((packed));
#endif

/** 
 * @brief FAST平台硬件规则格式定义
 * FAST规则主要由流关键字、关键字对应掩码、优先级、规则有效位、规则对应动作等组成。
 * 其他字段主要在软件使用
 */
struct fast_rule{
	struct flow key;		/**< @brief 规则内容存储结构 @see ::flow*/
	struct flow mask;		/**< @brief 规则掩码设置结构,与key一一对应 @see ::flow*/
	u32 priority;			/**< @brief 规则的优先级设置*/
	u32 valid;				/**< @brief 规则的有效标志设置,要删除一条规则只需要将此标记置0,并更新到硬件*/
	u32 action;				/**< @brief 规则所对应的执行动作，动作由两部分组成，高4位为ACTION枚举类型，低28位不此类型对应的值 @see ::ACTION*/
	/*此前数据必须要写入到硬件*/	
	u32 flow_stats_len;		/**< @brief 流表信息长度*/
	u64 *flow_stats_info;   /**< @brief 流表信息存储指针位置*/
#ifdef OPENBOX_S4
	u32 *tmp;				/*在S4系统中，上面变量指针大小为32*/
#endif
	u64 cookie;				/**< @brief 用来存储流的cookie信息，软件使用*/
	u64 cookie_mask;		/**< @brief 用来存储流cookie的掩码信息，软件使用*/
	u32 md5[4];				/**< @brief MD5值 @note 用来区分规则的唯一性，软件生成规则时即会判断是否规则重复*/	
	u32 pad[18];			/**< @brief 总长256B，此为确保数据结构大小做的填充*/
};
/*保持硬件一条流表的完整长度为64B*4=256B长度*/
/*-----------------------------------流表相关----------------------------------------------*/


/*-----------------------------------BV新定义位宽104位，5元组查表-----------------------------------*/
struct tuple5
{
	u32 sip;
	u32 dip;
	u16 sport;
	u16 dport;
	u16 pad0;
	u8 pad1;
	u8 proto;
};
struct row_bv
{
	struct tuple5 key;
	struct tuple5 mask;	
	u32 idx:5,		/**< 规则索引*/
		valid:1,	/**< 规则有效位*/
		pad:26;		/**< 暂时不用*/
	u32 action;
};
/*-----------------------------------BV新定义位宽104位，5元组查表-----------------------------------*/

/*-----------------------------------报文相关----------------------------------------------*/
/** 
 * @brief UM模块数据格式定义
 */
struct um_metadata{
#if defined(__LITTLE_ENDIAN) /*INTER*/	
#ifdef FAST_10
	u64 ts:32,			/**< @brief 报文接收的时间戳 @note 如果用户需要使用表示更大的时间，建议存储在第二拍数据中（user[2]字段）*/
		ts2:12,
		flowID:14,		/**< @brief 流ID号*/
		priority:3,		/**< @brief 报文优先级*/
		discard:1,		/**< @brief 指示报文是否丢弃 @note 默认为0，表示不丢弃，置1时表示丢弃*/
		pktdst:1,		/**< @brief 报文的输出目的方向 @note 0表示输出到网络端口，1表示输出到CPU*/
		pktsrc:1;		/**< @brief 报文的输入源方向 @note 0表示网络端口输入，1表示从CPU输入*/
	u64 outport:16,		/**< @brief 报文输出端口号 @note 以bitmap形式表示，1表示从0号端口输出；8表示从3号端口输出*/
		seq:12,			/**< @brief 报文接收时的序列号 @note 每个端口独立维护一个编号*/
		dstmid:8,		/**< @brief 报文下次处理的目的模块编号*/
		srcmid:8,		/**< @brief 报文上次处理时的模块编号*/
		len:12,			/**< @brief 报文长度 @note 最大可表示4095字节，但FAST平台报文缓存区最大为2048，完整以太网报文的MTU不要超过1500*/
		inport:4,		/**< @brief 输入端口号 @note 取值：0——15，最多表示16个输入端口*/
		ttl:4;			/**< @brief 报文通过模块的TTL值，每过一个处理模块减1*/
#elif FAST_20
	u64 ts:32,			/**< @brief 时间戳*/
		reserve:17,		/**< @brief 保留*/
		pktsrc:1,		/**< @brief 分组的来源，0为网络接口输入，1为CPU输入，此比特位在进入硬件时会交换到pkttype位置，保留位为18位*/
		flowID:14;		/**< @brief 流ID*/
	u64	seq:8,			/**< @brief 分组接收序列号*/
		pst:8,			/**< @brief 标准协议类型（参考硬件定义）*/
		dstmid:8,			/**< @brief 下一个处理分组的模块ID*/
		srcmid:8,			/**< @brief 最近一次处理分组的模块ID*/
		len:12,			/**< @brief 报文长度*/
		discard:1,		/**< @brief 丢弃位*/
		priority:3,		/**< @brief 分组优先级*/
		outport:6,		/**< @brief 单播：分组输出端口ID，组播/泛洪：组播/泛洪表地址索引*/
		outtype:2,		/**< @brief 输出类型，00：单播，01：组播，10：泛洪，11：从输入接口输出*/
		inport:6,		/**< @brief 分组的输入端口号*/
		pktdst:1,		/**< @brief 分组目的，0为网络接口输出，1为送CPU*/
		pkttype:1;		/**< @brief 报文类型，0：数据报文，1：控制报文。硬件识别报文类别后，会将pktsrc位交换到此，恢复硬件数据格式*/		
#endif
	u64 user[2];		/**< @brief 用户自定义metadata数据格式与内容 @remarks 此字段由可用户改写，但需要保证数据大小严格限定在16字节*/
#elif defined(__BIG_ENDIAN)  /**/	
	#error	"(__BIG_ENDIAN)Please fix <asm/byteorder.h>"
#else
	#error	"Please fix <asm/byteorder.h>"
#endif
};

/*FAST 2.0环形控制通路控制报文*/
struct ctl_metadata
{
	u64 data:32,
		mask:32;
	u64 addr:32,
		dstmid:8,
		srcmid:8,
		seq:12,
		type:3,
		pkttype:1;
	u64 reserve;
	u64 sessionID;
};

struct common_metadata
{
	u64 a;
	u64 b:32,
		c:31,
		pkttype:1;
	u64 d;
	u64 e;
};

/**
 * @brief FAST架构完整报文格式定义
 * 
 * FAST平台报文由3部分级成
 * UM头 + 对齐标志 + 完整以太网报文
 * UM头共32节字，@see ::um_metadata
 * 对齐标志2字节，为对对齐IP地址填充使用
 * 完成以太网报文大小为1514字节，存储完整以太网报文内容
 */
struct fast_packet
{
	union
	{
		struct um_metadata um;		/**< @brief UM模块数据格式定义 @see ::um_metadata */
		struct ctl_metadata cm;		/**< 控制报文格式定义*/
		struct common_metadata md;  /**< 公共控制信息，报文类型（0：数据，1：控制）*/
	};
#ifdef OPENBOX_S28
	u16 flag;					/**< @brief 2字节对齐标志，主要是为了IP地址的对齐 @note 此标志在内核会用来标记是一个FAST类型报文，建议用户不使用些字段*/
#endif
	u8 data[1514];				/**< @brief 完整以太网报文数据，暂时不含CRC数据*/
};

/**
 * @brief UA与内核消息交互结构定义
 */
struct fast_ua_kernel_msg
{
	int mid;			/**< @brief 提供相应的处理MID号 @note 软件模块MID号范围为：129-255*/
	int pid;			/**< @brief 用户程序的PID，UA注册时，低层库函数自动获取当前进程号*/
	UA_STATE state;		/**< @brief UA消息枚举状态 @see ::UA_STATE*/
};

/**
* @brief UA模块用户回调函数类型定义
*
* @param pkt 表示接收到的报文，以指针方式传递
* @warning 此指针数据为库函数底层循环使用变量，用户若要存储数据需要自己申请新空间并从此指针位置拷贝数据
* @param pkt_len 表示此报文的有效长度 @note 此长度是指整个struct fast_packet的长度，取值为pkt->um.len
*
* @return 
*/
typedef int(*fast_ua_recv_callback)(struct fast_packet *pkt,int pkt_len);

/** 
 *  @brief IPv6地址定义
 */
struct in6_xaddr
{
	u8 addr[16];		/**< @brief 存储IPv6地址的每个字节对象*/
};

/* Ensure that we can do comparisons as longs. */
#endif //__FAST_STRUCT_H__
