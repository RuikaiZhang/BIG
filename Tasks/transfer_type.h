#ifndef BIG_TRANSFER_TYPE_H
#define BIG_TRANSFER_TYPE_H

#define MAXFILELEN 1024*1024
#include "stdint.h"

typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;

//������
enum funcodeEnum{
////����,UDP
//LinkRequest,//��������
//LinkResponse,//������Ӧ
//LinkEndRequest,//���ӽ�������
//LinkEndReturn,//���ӽ�������

	
//����������
monitorRequest,//�������
garbageTypeReturn,//�������ͷ���
monitorEndRequest,//���ֹͣ����
conditionRequest,//����Ͱ״̬����
//�ͻ��˷���
conditionReturn,//����Ͱ״̬����
monitorResponse,//�����Ӧ
garbageTypeRequest,//������������
monitorEndReturn//���ֹͣ����
};

//�������
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
//    //ͼ������֡����
//    unsigned int uDataFrameSize;        //����֡���ܴ�С
//    unsigned int uDataFrameTotal;       //һ֡���ݱ��ֳɴ���֡�ĸ���
//    unsigned int uDataFrameCurr;        //����֡��ǰ��֡��
//    unsigned int uDataInFrameOffset;    //����֡����֡��ƫ��
//};

struct FileData
{
    //ͼ������֡��Ϣ
    unsigned int fileSize;      //����֡���ܴ�С
    unsigned int totalFrame;    //��֡��
		unsigned int currentFrame;    //��ǰ֡��
		unsigned int currentFrameLength; //��ǰ֡��С
		
};

struct FrameHeadType
{
    unsigned int funCode;           //������
//    unsigned int TransFrameHdrSize;    //֡ͷ��С
//    unsigned int TransFrameSize;       //��֡��С
};

union TransferdataType
{
    struct FileData fileData;//�ļ�����
    unsigned int garbageType;//��������
    struct garbagePercentageType conditionRet;//״̬����
};

struct transfertype
{
    struct FrameHeadType frameHead;
    union TransferdataType data;
};

#endif // TRANSMISSIONTYPE_H
