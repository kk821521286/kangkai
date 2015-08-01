
#include "book.h"

/*  界面显示  API                                                       */

/************************************************************************/

char g_szTimeStamp[10];                                   // 系统打印时间的用的字符串
char *g_szBookStateString[3]  = {"库存","外借","其他"};   // 图书状态的字符串
char *g_szBookOptionString[2] = {"借出","归还"};          // 图书操作的字符串

//********************************************
// Method      :  GetLocalTime
// Description :  将时间戳打印到 g_szTimeStamp
// Returns     :  BOOL 是否打印成功
//*******************************************
BOOL GetLocalTime()
{
	time_t ulTime ;
	struct tm *stTime = NULL;    
	char *pcTimeStr = NULL;

	time(&ulTime);
	stTime = localtime(&ulTime); // 此处根据系统原理，不会返回空指针，故不作检测

	sprintf(g_szTimeStamp, "%02d:%02d:%02d", stTime->tm_hour, stTime->tm_min, stTime->tm_sec);
	g_szTimeStamp[9] = '\0';
	// localtime 实现的原理不是每次申请内存，而是使用一个全局变量所以返回得指着不需要释放！！！
	// free(stTime); 

	return TRUE;
}

//********************************************
// Method      :  apiPrintMsg
// Description :  按消息ID（MSG_ID_EM）打印系统消息
// Param       :  输入 MSG_ID_EM emMsgID 消息ID
// Returns     :  BOOL 是否打印成功
//*******************************************
void apiPrintMsg(MSG_ID_EM emMsgID)
{
	int i;
	char *pcTimeStr = NULL;
	for ( i = 0; END_ID_OF_MSG_SET != g_szMsg[i].emID; i++ )
	{
		if ( g_szMsg[i].emID == emMsgID )
		{
			if ( TRUE == GetLocalTime() )
			{
				printf("[%s]>> %s\n\n", g_szTimeStamp, g_szMsg[i].szMessage);    
			}
			break;
		}
	}
}

//********************************************
// Method      :  apiPrintBookInfo
// Description :  打印图书详情
// Param       :  输入 BOOK_INFO* pstBookInfo 图书数组指针
//             :  输入 int iCount  数组长度
//*******************************************
void apiPrintBookInfo(BOOK_INFO* pstBookInfo, int iCount)
{
	int i = 0, iStateIndex = 2;

	if ( NULL == pstBookInfo || 0 == iCount)
	{
		apiPrintMsg(MSG_BOOK_NOT_HAS);
		return;
	}

	apiPrintMsg(MSG_BOOK_QRY_SCS);
	printf("=========================================================\n");
	printf("|   图书号   |   状态   |       图书名                   \n");
	for ( i = 0; i < iCount; i++)
	{
		switch(pstBookInfo[i].emState)
		{
			case BOOK_IN  : iStateIndex = 0; break;
			case BOOK_OUT : iStateIndex = 1; break;
			default: iStateIndex = 2;
		}
		printf("|   %6u   |   %s   |   %s\n", 
				pstBookInfo[i].nNum, g_szBookStateString[iStateIndex],
				pstBookInfo[i].szName);
	}
	printf("=========================================================\n\n");
}

//********************************************
// Method      :  apiPrintBookHistoryInfo
// Description :  打印图书详情 只使用数据 不释放空间
// Param       :  输入 BOOK_HIS_INFO *pstBookHistroyList 图书历史记录数组指针
//             :  输入 int iCount  数组长度
//*******************************************
void apiPrintBookHistoryInfo(BOOK_HIS_INFO *pstBookHistroyList, int iCount)
{
	int i = 0, iOpID = -1;

	if ( 0 > iCount)
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
		return;
	}

	if ( NULL == pstBookHistroyList || 0 == iCount)
	{
		apiPrintMsg(MSG_BOOK_NOT_HAS);
		return;
	}

	apiPrintMsg(MSG_BOOK_HIS_SCS);
	printf("====================================================================\n");
	printf("|   时间   |   图书号   |   操作   |       图书名                   \n");
	for ( i = 0; i < iCount; i++)
	{
		switch( pstBookHistroyList[i].emBookOption )
		{
			case BOOK_OP_IN  : iOpID = 1; break;
			case BOOK_OP_OUT : iOpID = 0; break;
			default: apiPrintMsg(MSG_SYS_INNER_ERROR);return;
		}

		if ( NULL == pstBookHistroyList[i].pstBookInfo )
		{
			apiPrintMsg(MSG_SYS_INNER_ERROR);
			return;
		}

		printf("| %02d:%02d:%02d |   %6u   |   %s   |   %s\n",
				pstBookHistroyList[i].stOptionTime.nHour,
				pstBookHistroyList[i].stOptionTime.nMin,
				pstBookHistroyList[i].stOptionTime.nSec,
				pstBookHistroyList[i].pstBookInfo->nNum,
				g_szBookOptionString[iOpID],
				pstBookHistroyList[i].pstBookInfo->szName);
	}
	printf("====================================================================\n");
}

/************************************************************************/
/*  数据存储和基础功能  被cmd函数调用 不允许调用 api                    */
/************************************************************************/

//BOOK_INFO g_szBooks[MAX_BOOK_COUNT];    // 图书信息数组


//BOOK_HIS_NODE *g_pstHisLogHead = NULL;  // 历史记录链表头指针（包含数据）

//********************************************
// Method      :  GetSysTime
// Description :  获得系统时间
// Return      :  包含时分秒的时间结构体
//*******************************************
SYS_TIME GetSysTime()
{
	SYS_TIME stSysTime;
	time_t ulTime ;
	struct tm *stTime = NULL;    

	time(&ulTime);
	stTime = localtime(&ulTime);// 这里不需要检测空指针

	stSysTime.nHour = stTime->tm_hour;
	stSysTime.nMin  = stTime->tm_min;
	stSysTime.nSec  = stTime->tm_sec;

	return stSysTime;
}

//********************************************
// Method      :  FreeHisList
// Description :  释放历史记录内存
// Param       :  输入 BOOK_HIS_NODE *pstHead 头指针
//*******************************************
void FreeHisList(BOOK_HIS_NODE *pstHead)
{
	BOOK_HIS_NODE *pstNode = NULL;

	while( NULL != pstHead)
	{
		pstNode = pstHead;
		pstHead = pstHead->pstNext;
		free( pstNode );
	}
}

//********************************************
// Method      :  GetBookHisCount
// Description :  图书历史记录数量，nBookBumber = 0 时返回总长度
// Param       :  输入BookNumber nBookBumber 图书号
// Return      :  int 历史记录数量
//*******************************************
int GetBookHisCount(BookNumber nBookBumber)
{
	int nSize = 0;
	BOOK_HIS_NODE *pstNode = g_pstHisLogHead;

	if ( DEFAULT_BOOK_ID == nBookBumber )
	{
		while( NULL != pstNode )
		{
			pstNode = pstNode->pstNext;
			nSize ++ ;
		}
		return nSize;
	}

	while( NULL != pstNode )
	{
		if ( NULL == pstNode->stBookHisInfo.pstBookInfo )
		{
			return -1;
		}
		nSize  += pstNode->stBookHisInfo.pstBookInfo->nNum == nBookBumber? 1 : 0 ;
		pstNode = pstNode->pstNext;
	}
	return nSize;
}

//********************************************
// Method      :  GetHisNodeByBookID
// Description :  复制图书历史记录信息到数组并返回，出错时 nCount 等于 -1
// Param       :  输入 BookNumber nBookNumber
//             :  输出 int *nCount  数组长度，由调用函数保证非空
// Return      :  BOOK_HIS_INFO* 数组首地址
//*******************************************
BOOK_HIS_INFO *GetHisNodeByBookID(BookNumber nBookNumber,int *nCount)
{
	int i = 0;
	BOOK_HIS_INFO *pstHisArrHead = NULL;
	BOOK_HIS_NODE *pstNode = g_pstHisLogHead;
	(*nCount) = GetBookHisCount(nBookNumber); // 出错时返回 -1

	if( 0 > (*nCount) )
	{
		return NULL;
	}

	pstHisArrHead = (BOOK_HIS_INFO *)malloc((*nCount) * sizeof(BOOK_HIS_INFO));
	if( NULL == pstHisArrHead )
	{
		(*nCount) = -1;
		return NULL;
	}

	if( DEFAULT_BOOK_ID == nBookNumber )
	{
		while( NULL != pstNode )
		{
			if( NULL == pstNode->stBookHisInfo.pstBookInfo )
			{
				(*nCount) = -1;
				free(pstHisArrHead);
				return NULL;
			}
			pstHisArrHead[i++] = pstNode->stBookHisInfo;
			pstNode = pstNode->pstNext;
		}
		return pstHisArrHead;
	}

	while( NULL != pstNode )
	{
		if( NULL == pstNode->stBookHisInfo.pstBookInfo )
		{
			(*nCount) = -1;
			free(pstHisArrHead);
			return NULL;
		}
		if( nBookNumber == (pstNode->stBookHisInfo.pstBookInfo->nNum) )
		{
			pstHisArrHead[i++] = pstNode->stBookHisInfo;
		}
		pstNode = pstNode->pstNext;
	}

	return pstHisArrHead;
}

//********************************************
// Method      :  AddHisNode
// Description :  向历史记录链表追加节点
// Param       :  输入 BOOK_INFO *pstBookInfo 图书信息指针
//             :  输入 BOOK_OP_EM emOption 操作枚举
// Return      :  BOOL 是否成功
//*******************************************
BOOL AddHisNode(BOOK_INFO *pstBookInfo, BOOK_OP_EM emOption)
{
	BOOK_HIS_NODE *pstTmp = NULL;
	BOOK_HIS_NODE *pstBookHisNode = (BOOK_HIS_NODE *)malloc(sizeof(BOOK_HIS_NODE));

	if ( NULL == pstBookHisNode )
	{
		return FALSE;
	}

	pstBookHisNode->pstNext = NULL;
	pstBookHisNode->stBookHisInfo.emBookOption = emOption;
	pstBookHisNode->stBookHisInfo.pstBookInfo  = pstBookInfo;
	pstBookHisNode->stBookHisInfo.stOptionTime = GetSysTime();

	if ( NULL == g_pstHisLogHead )
	{
		g_pstHisLogHead = pstBookHisNode;
		return TRUE;
	}

	pstTmp = g_pstHisLogHead;
	while( NULL != pstTmp->pstNext )
	{
		pstTmp = pstTmp->pstNext;
	}
	pstTmp->pstNext= pstBookHisNode;

	return TRUE;
}

//********************************************
// Method      :  IsBookNumRight
// Description :  判断输入图书编号是否正确
// Param       :  输入 BookNumber nBookNumber 图书编号
// Return      :  BOOL 是否正确
//*******************************************
BOOL IsBookNumRight(BookNumber nBookNumber)
{
	if ( MAX_BOOK_NUMBER >= nBookNumber && MIN_BOOK_NUMBER <= nBookNumber )
	{
		return TRUE;
	}
	return FALSE;
}

//********************************************
// Method      :  GetBookInfoByID
// Description :  获得特定ID的图书
// Param       :  输入 BookNumber nBookNumber 图书编号
// Return      :  BOOK_INFO* 指向数组中图书信息的地址，禁止释放
//*******************************************
BOOK_INFO* GetBookInfoByID(BookNumber nBookNumbe)
{
	return &(g_szBooks[nBookNumbe-MIN_BOOK_NUMBER]);
}


//********************************************

// Method      :  cmdBooksInit
// Description :  系统初始化事件
//*******************************************
void cmdBooksInit()
{
	BookNumber i;
	for ( i = 0; i < MAX_BOOK_COUNT; i++)
	{
		g_szBooks[i].emState = BOOK_IN;
		g_szBooks[i].nNum = MIN_BOOK_NUMBER + i;
		sprintf(g_szBooks[i].szName, "<< book number %u >>", i);
		g_szBooks[i].szName[MAX_BOOK_NAME_LEN] = '\0';// 赋值超长保护
	}
	FreeHisList(g_pstHisLogHead);
	g_pstHisLogHead = NULL;
	apiPrintMsg(MSG_SYS_INIT_SUCCESS);
}
//********************************************
// Method      :  cmdBooksBorrow
// Description :  借书事件
// Param       :  输入 BookNumber nBookNumber 图书编号
//*******************************************
void cmdBooksBorrow(BookNumber nBookNumber)
{
	BOOK_INFO *pstBook = NULL;
	if ( FALSE == IsBookNumRight(nBookNumber) )
	{
		apiPrintMsg(MSG_BOOK_BAD_ID);
		return;
	}
	pstBook = GetBookInfoByID(nBookNumber);
	if ( NULL == pstBook )
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
		return;
	}
	if ( BOOK_OUT == pstBook->emState )
	{
		apiPrintMsg(MSG_BOOK_NOT_IN);
		return;
	}
	pstBook->emState = BOOK_OUT;
	apiPrintMsg(MSG_BOOK_BRW_SCS);
	if ( FALSE == AddHisNode(pstBook, BOOK_OP_OUT) )
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
	}
}
//********************************************
// Method      :  cmdBooksReturn
// Description :  还书事件
// Param       :  输入 BookNumber nBookNumber 图书编号
//*******************************************
void cmdBooksReturn(BookNumber nBookNumber)
{
	BOOK_INFO *pstBook = NULL;
	if ( FALSE == IsBookNumRight(nBookNumber) )
	{
		apiPrintMsg(MSG_BOOK_BAD_ID);
		return;
	}
	pstBook = GetBookInfoByID(nBookNumber);
	if ( NULL == pstBook )
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
		return;
	}
	if ( BOOK_IN == pstBook->emState )
	{
		apiPrintMsg(MSG_BOOK_NOT_OUT);
		return;
	}
	pstBook->emState = BOOK_IN;
	apiPrintMsg(MSG_BOOK_RTN_SCS);
	if ( FALSE == AddHisNode(pstBook, BOOK_OP_IN) )
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
	}
}
//********************************************
// Method      :  cmdBooksQuery
// Description :  查询事件
// Param       :  输入 BookNumber nBookNumber 图书编号
//*******************************************
void cmdBooksQuery(BookNumber nBookNumber)
{
	BOOK_INFO *pstBook = NULL;
	if( DEFAULT_BOOK_ID == nBookNumber )
	{
		apiPrintBookInfo( &g_szBooks[0], MAX_BOOK_COUNT);
		return;
	}
	if( FALSE == IsBookNumRight(nBookNumber) )
	{
		apiPrintMsg(MSG_BOOK_BAD_ID);
		return;
	}
	pstBook = GetBookInfoByID(nBookNumber);
	if( NULL == pstBook )
	{
		apiPrintMsg(MSG_SYS_INNER_ERROR);
		return;
	}
	apiPrintBookInfo(pstBook, 1);
}
//********************************************
// Method      :  cmdBooksHistory
// Description :  历史查询事件
// Param       :  输入 BookNumber nBookNumber 图书编号
//*******************************************
void cmdBooksHistory(BookNumber nBookNumber)
{
	int nCount = 0;
	BOOK_HIS_INFO *pstHisArrayHead = NULL;
	if ( DEFAULT_BOOK_ID != nBookNumber && FALSE == IsBookNumRight(nBookNumber) )
	{
		apiPrintMsg(MSG_BOOK_BAD_ID);
		return;
	}
	pstHisArrayHead = GetHisNodeByBookID(nBookNumber, &nCount);
	apiPrintBookHistoryInfo(pstHisArrayHead, nCount);
	free(pstHisArrayHead);
}
/************************************************************************/
/*  系统函数 ，禁止其他函数调用                                         */
/************************************************************************/
//********************************************
// Method      :  GetUserCommand
// Description :  获得输入，激发系统事件 
// Return      :  BOOL 是否退出命令
//*******************************************
BOOL GetUserCommand()
{    
	BookNumber nBookNumber = 0;
	char scTmp[MAX_CMD_TMP_LEN + 1] = {0};
	scanf("%s", scTmp);
	scTmp[MAX_CMD_TMP_LEN] = '\0';
	if( 0 == strcmp(scTmp, "exit") )
	{
		return FALSE;
	}
	if( 0 == strcmp(scTmp,"reboot"))
	{
		cmdBooksInit();
		return TRUE;
	}
	if( 0 == strcmp(scTmp, "borrow") )
	{
		scanf("%u",&nBookNumber);
		cmdBooksBorrow(nBookNumber);
		return TRUE;
	}
	if( 0 == strcmp(scTmp, "return") )
	{
		scanf("%u",&nBookNumber);
		cmdBooksReturn(nBookNumber);
		return TRUE;
	}
	if( 0 == strcmp(scTmp, "query") )
	{
		scanf("%u",&nBookNumber);
		cmdBooksQuery(nBookNumber);
		return TRUE;
	}
	if( 0 == strcmp(scTmp, "history") )
	{
		scanf("%u",&nBookNumber);
		cmdBooksHistory(nBookNumber);
		return TRUE;
	}
	apiPrintMsg(MSG_SYS_BAD_CMD);
	return TRUE;
}

//********************************************
// Method      :  main
// Description :  启动函数，开始主循环
//*******************************************
int main(void)
{
	BOOL bIsRun = TRUE;
	cmdBooksInit();
	while ( TRUE == bIsRun )
	{
		bIsRun = GetUserCommand();
	}
	return 0;
}
