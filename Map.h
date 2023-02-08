/******************************************************************************
Module:  Map.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

//CMap类使用二叉树算法，大大减少了查找次数，加快运行速度
//CMap类模板比较键值用到< > == 符号，如果键值类型为非基础数据类型，请重载这些符号
//CMap会复制键值内容，仅会复制实值指针，为了效率与安全，键值最好是基础类型，实值内存必须存在

#pragma once

template<class KEY,class VALUE>
class  CMap {
public:
	BOOL Insert(KEY Key,VALUE& Value);
	BOOL Query(KEY Key,VALUE& Value);
protected:
	struct Map {
		KEY m_Key;
		VALUE m_Value;
		Map* m_NextLeft;
		Map* m_NextRight;
		Map* m_Base;
		BOOL m_color;//Red is True,Black if False
	};
	Map* m_BaseMap;
public:
	CMap():m_BaseMap(NULL) {}
};



template<class KEY,class VALUE>
BOOL CMap<KEY,VALUE>::Insert(KEY Key,VALUE& Value)
{
	if(m_BaseMap==NULL) { //Map为空
		m_BaseMap=new Map;
		m_BaseMap->m_Key=Key;
		m_BaseMap->m_Value=Value;
		m_BaseMap->m_color=FALSE;//黑色
		m_BaseMap->m_NextLeft=NULL;
		m_BaseMap->m_NextRight=NULL;
		return TRUE;
	}
	Map* pTemp=m_BaseMap;
	Map* pBaseTemp=NULL;
	while(pTemp!=NULL) {
		pBaseTemp=pTemp;
		if(Key==pTemp->m_Key) { //键值重复
			return FALSE;
		}
		else {
			if(Key<pTemp->m_Key) {
				pTemp=pTemp->m_NextLeft;
			}
			else {
				pTemp=pTemp->m_NextRight;
			}
		}
	}
	pTemp=new Map;
	pTemp->m_Key=Key;
	pTemp->m_Value=Value;
	pTemp->m_NextLeft=NULL;
	pTemp->m_NextRight=NULL;
	pTemp->m_Base=pBaseTemp;

	if(Key<pBaseTemp->m_Key) pBaseTemp->m_NextLeft=pTemp;
	else pBaseTemp->m_NextRight=pTemp;

	return TRUE;
}

template<class KEY,class VALUE>
BOOL CMap<KEY,VALUE>::Query(KEY Key,VALUE& Value)
{
	Map* pTemp=m_BaseMap;
	while(pTemp!=NULL) {
		if(Key==pTemp->m_Key) {
			Value=pTemp->m_Value;
			return TRUE;
		}
		else {
			pTemp=(Key<pTemp->m_Key)? pTemp->m_NextLeft:pTemp->m_NextRight;
		}
	}
	return FALSE;
}