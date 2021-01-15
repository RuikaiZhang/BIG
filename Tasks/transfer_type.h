#ifndef BIG_TRANSFER_TYPE_H
#define BIG_TRANSFER_TYPE_H

#define MAXFILELEN 1024*1024
#include "stdint.h"

typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;

//功能码
enum funcodeEnum{
////公有,UDP
//LinkRequest,//连接请求
//LinkResponse,//连接响应
//LinkEndRequest,//连接结束请求
//LinkEndReturn,//连接结束返回

	
//服务器发送
monitorRequest,//监控请求
garbageTypeReturn,//垃圾类型返回
monitorEndRequest,//监控停止请求
conditionRequest,//垃圾桶状态请求
//客户端发送
conditionReturn,//垃圾桶状态返回
monitorResponse,//监控响应
garbageTypeRequest,//垃圾类型请求
monitorEndReturn//监控停止返回
};

//垃圾类别
enum garbageTypeEnum{
    garbageTypeRecyclable,
    garbageTypeKitchen,
    garbageTypeHarmful,
    garbageTypeOther
};

struct garbagePercentageType
{
	uint16_t RecyclablePercentage;
	uint16_t KitchenCondition;
	uint16_t HarmfulCondition;
	uint16_t OtherCondition;
};  


//struct ImageHead UDP
//{
//    //图像数据帧变量
//    unsigned int uDataFrameSize;        //数据帧的总大小
//    unsigned int uDataFrameTotal;       //一帧数据被分成传输帧的个数
//    unsigned int uDataFrameCurr;        //数据帧当前的帧号
//    unsigned int uDataInFrameOffset;    //数据帧在整帧的偏移
//};

struct FileData
{
    //图像数据帧信息
    unsigned int fileSize;      //数据帧的总大小
    unsigned int totalFrame;    //总帧数
		unsigned int currentFrame;    //当前帧号
		unsigned int currentFrameLength; //当前帧大小
		
};

struct FrameHeadType
{
    unsigned int funCode;           //功能码
//    unsigned int TransFrameHdrSize;    //帧头大小
//    unsigned int TransFrameSize;       //整帧大小
};

union TransferdataType
{
    struct FileData fileData;//文件数据
    unsigned int garbageType;//类型数据
    struct garbagePercentageType conditionRet;//状态数据
};

struct transfertype
{
    struct FrameHeadType frameHead;
    union TransferdataType data;
};

#endif // TRANSMISSIONTYPE_H
