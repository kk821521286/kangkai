/************************************************************************/
/* 面向对象演示                                                        */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

#define CID_OBJ 1
#define CID_ANM 101
#define CID_PLT 102


typedef struct stObj OBJECT_STRU;

typedef char* (*ToString_PF)(void *pObj, char *pcStrBuf);
typedef unsigned int ClassID;

/************************************************************************/
/*         object  基类                                                 */
/************************************************************************/

struct stObj
{
    ClassID m_uiCID;
    ToString_PF pfToString;
};

char* OBJ_ToString(void *pObj, char *pcStrBuf)
{
    sprintf(pcStrBuf, "ClassName:%s;ClassID:%06u", "Object", ((OBJECT_STRU *)pObj)->m_uiCID);
    return pcStrBuf;
}

OBJECT_STRU *new_Object()
{
    OBJECT_STRU* pstObj = (OBJECT_STRU*)malloc(sizeof(OBJECT_STRU));

    if ( NULL == pstObj )
    {
        return NULL;
    }

    pstObj->m_uiCID = CID_OBJ;
    pstObj->pfToString = OBJ_ToString;

    return pstObj;
}


/************************************************************************/
/*         animal      子类一                                          */
/************************************************************************/

typedef struct stAnimal
{
    // 父类实体，必须放在第一个，强制转换才有效，利用这种方法产生继承的效果
    OBJECT_STRU m_stBaseObj; 

    // 新增属性
    char acName[NAME_LEN];  
    
    // 也可增加新增方法 
    // int (* pfGetAge)();
}ANIMAL_STRU;

// 覆盖父类方法 
char* ANM_ToString(void *pObj, char *pcStrBuf)
{
    sprintf(pcStrBuf, "Classname:%s; AnimalName:%s; ClassID:%06u", 
        "Animal", 
        ((ANIMAL_STRU *)pObj)->acName,
        ((ANIMAL_STRU *)pObj)->m_stBaseObj.m_uiCID);
    return pcStrBuf;
}

ANIMAL_STRU *new_Animal()
{
    ANIMAL_STRU* pstObj = (ANIMAL_STRU*)malloc(sizeof(ANIMAL_STRU));
    
    if ( NULL == pstObj )
    {
        return NULL;
    }
    
    pstObj->m_stBaseObj.m_uiCID = CID_ANM;
    pstObj->m_stBaseObj.pfToString = ANM_ToString; // 覆盖父类方法，产生多态效果
    pstObj->acName[0] = '\0';
    
    return pstObj;
}


/************************************************************************/
/*         plant     子类二                                             */
/************************************************************************/

typedef struct stPlant
{
    OBJECT_STRU m_stBaseObj;
    char acName[NAME_LEN];
}PLANT_STRU;

char* PLT_ToString(void *pObjSelf, char *pcStrBuf)
{
    sprintf(pcStrBuf, "Classname:%s; PlantName:%s; ClassID:%06u", 
        "Plant", 
        ((PLANT_STRU *)pObjSelf)->acName,
        ((PLANT_STRU *)pObjSelf)->m_stBaseObj.m_uiCID);
    return pcStrBuf;
}

PLANT_STRU *new_Plant()
{
    PLANT_STRU* pstObj = (PLANT_STRU*)malloc(sizeof(PLANT_STRU));
    
    if ( NULL == pstObj )
    {
        return NULL;
    }
    
    pstObj->m_stBaseObj.m_uiCID = CID_PLT;
    pstObj->m_stBaseObj.pfToString = PLT_ToString;
    pstObj->acName[0] = '\0';
    
    return pstObj;
}

/************************************************************************/
/*     测试                                                             */
/************************************************************************/

int main()
{
    char acStrBuf[200] = {0};

    OBJECT_STRU *pObj = NULL;
    ANIMAL_STRU *pAnm = NULL;
    PLANT_STRU  *pPlt = NULL;

    // 定义父类
    OBJECT_STRU *pObj_animal = NULL;
    OBJECT_STRU *pObj_plant  = NULL;

    //父类函数和属性调用 
    pObj = new_Object();
    printf("[ID = %u][ ToString = %s ]\n", pObj->m_uiCID, pObj->pfToString(pObj, acStrBuf) );
    free(pObj);
    pObj = NULL;
   
    // 子类函数和属性调用 
    pAnm = new_Animal();
    strcpy(pAnm->acName,"Panda");
    printf("%s\n", pAnm->m_stBaseObj.pfToString(pAnm, acStrBuf) );
    free(pPlt);
    free(pAnm);
    pPlt = NULL;
    pAnm = NULL;

    // 子类函数和属性调用 
    pPlt = new_Plant();
    strcpy(pPlt->acName,"Tree");
    printf("%s\n", pPlt->m_stBaseObj.pfToString(pPlt, acStrBuf) );


    /************************************************************************/
    /*多态测试                                           */
    /************************************************************************/

    // 定义父类
    // OBJECT_STRU *pObj_animal = NULL;
    // OBJECT_STRU *pObj_plant  = NULL;

    // 初始化子类
    pObj_animal = (OBJECT_STRU *)new_Animal();
    pObj_plant  = (OBJECT_STRU *)new_Plant();

    // 执行相同的函数产生不同的效果
    printf("%s\n", pObj_animal->pfToString(pObj_animal, acStrBuf) );
    printf("%s\n",  pObj_plant->pfToString(pObj_plant , acStrBuf) );

    //getchar();

    free(pObj_animal);
    free(pObj_plant );

    return 0;
}
