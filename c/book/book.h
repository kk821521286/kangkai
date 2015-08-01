#ifndef __BOOK_H_
#define __BOOK_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/************************************************************************/
/*   基础数据定义                                                       */
/************************************************************************/

#define MAX_MSG_LEN        100 
#define MAX_CMD_TMP_LEN    100
#define MAX_BOOK_NAME_LEN  30
#define MAX_BOOK_COUNT     10
#define MIN_BOOK_NUMBER    1000
#define MAX_BOOK_NUMBER    MIN_BOOK_NUMBER + MAX_BOOK_COUNT - 1
#define DEFAULT_BOOK_ID    0

// 生成消息集合相关的宏定义
#define END_ID_OF_MSG_SET      -1
#define BEGIN_MSG_SET          MSG_INFO g_szMsg[] = {
#define ADD_MSG(ID,MSG_STR)    {ID,MSG_STR},
#define END_MSG_SET            {END_ID_OF_MSG_SET,""}};

typedef unsigned char UINT8;
typedef unsigned int BookNumber;

// 系统时间格式
typedef struct tagSysTime
{
	UINT8 nHour;
	UINT8 nMin;
	UINT8 nSec;
}SYS_TIME;

// 布尔值
typedef enum emBoolVal
{
	TRUE,
	FALSE
}BOOL;

// 图书状态
typedef enum emBookState
{
	BOOK_IN,
	BOOK_OUT
}BOOK_STATE_EM;

// 图书操作
typedef enum emBookOption
{
	BOOK_OP_IN,
	BOOK_OP_OUT
}BOOK_OP_EM;

// 消息ID 系统通过该ID打印消息
typedef enum emMsgID
{
	MSG_SYS_INNER_ERROR  = 0 ,  /* 内部错误     */
	MSG_SYS_INIT_SUCCESS = 1 ,  /* 初始化成功   */
	MSG_SYS_BAD_CMD      = 2 ,  /* 命令错误     */
	MSG_BOOK_BAD_ID      = 11,  /* 图书编号错误 */
	MSG_BOOK_NOT_IN      = 12,  /* 图书已经外借 */
	MSG_BOOK_NOT_OUT     = 13,  /* 图书没有外借 */
	MSG_BOOK_BRW_SCS     = 14,  /* 图书外借成功 */
	MSG_BOOK_RTN_SCS     = 15,  /* 图书归还成功 */
	MSG_BOOK_NOT_HAS     = 16,  /* 无该书信息   */
	MSG_BOOK_QRY_SCS     = 17,  /* 图书查询成功 */
	MSG_BOOK_HIS_SCS     = 18   /* 历史查询成功 */
}MSG_ID_EM;

// 消息结构体
typedef struct tagMsgInfo
{
	MSG_ID_EM emID;
	char szMessage[MAX_MSG_LEN];
}MSG_INFO;

// 图书信息
typedef struct tagBookInfo
{
	BookNumber nNum;
	BOOK_STATE_EM emState;
	char szName[MAX_BOOK_NAME_LEN + 1];
} BOOK_INFO;

// 图书节约历史信息
typedef struct tagBookHistInfo
{
	BOOK_INFO   *pstBookInfo ;
	BOOK_OP_EM   emBookOption;
	SYS_TIME     stOptionTime;
}BOOK_HIS_INFO;

// 图书节约历史信息的链表节点
typedef struct tagBookHisNode
{
	BOOK_HIS_INFO stBookHisInfo;
	struct tagBookHisNode *pstNext;
}BOOK_HIS_NODE;

// 生成消息集，供系统按ID打印消息
BEGIN_MSG_SET
ADD_MSG(MSG_SYS_INNER_ERROR , "内部错误"    )
ADD_MSG(MSG_SYS_INIT_SUCCESS, "初始化成功"  )
ADD_MSG(MSG_SYS_BAD_CMD     , "命令错误"    )
ADD_MSG(MSG_BOOK_BAD_ID     , "图书编号错误")
ADD_MSG(MSG_BOOK_NOT_IN     , "图书已经外借")
ADD_MSG(MSG_BOOK_NOT_OUT    , "图书没有外借")
ADD_MSG(MSG_BOOK_BRW_SCS    , "图书外借成功")
ADD_MSG(MSG_BOOK_RTN_SCS    , "图书归还成功")
ADD_MSG(MSG_BOOK_NOT_HAS    , "无该书信息"  )
ADD_MSG(MSG_BOOK_QRY_SCS    , "图书查询成功")
ADD_MSG(MSG_BOOK_HIS_SCS    , "历史查询成功")
END_MSG_SET


//======================================
BOOK_INFO g_szBooks[MAX_BOOK_COUNT];  //图书信息数组 
BOOK_HIS_NODE *g_pstHisLogHead = NULL;  //历史记录链表头指针 （包含数据）
BOOL GetLocalTime();

void apiPrintMsg(MSG_ID_EM emMsgID); 
void apiPrintBookInfo(BOOK_INFO* pstBookInfo,int iCount);
void apiPrintBookHistoryInfo(BOOK_HIS_INFO *pstBookHistroyList,int iCount);

SYS_TIME GetSysTime();
extern void FreeHisList(BOOK_HIS_NODE *pstHead);
extern int GetBookHisCount(BookNumber nBookBumber);
extern BOOK_HIS_INFO *GetHisNodeByBookID(BookNumber nBookNumber,int *nCount);
extern BOOL AddHisNode(BOOK_INFO *pstBookInfo,BOOK_OP_EM emOption);
extern BOOL IsBookNumRight(BookNumber nBookNumber);
extern BOOK_INFO* GetBookInfoByID(BookNumber nBookNumbe);
extern SYS_TIME GetSysTime();
extern void FreeHisList(BOOK_HIS_NODE *pstHead);
extern int GetBookHisCount(BookNumber nBookBumber);
extern BOOK_HIS_INFO *GetHisNodeByBookID(BookNumber nBookNumber,int *nCount);
extern BOOL AddHisNode(BOOK_INFO *pstBookInfo,BOOK_OP_EM emOption);
extern BOOL IsBookNumRight(BookNumber nBookNumber);
extern BOOK_INFO* GetBookInfoByID(BookNumber nBookNumbe);
extern BOOL GetUserCommand();
#endif 



















