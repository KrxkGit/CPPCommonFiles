/******************************************************************************
Module:  Map.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

//CMap��ʹ�ö������㷨���������˲��Ҵ������ӿ������ٶ�
//CMap��ģ��Ƚϼ�ֵ�õ�< > == ���ţ������ֵ����Ϊ�ǻ����������ͣ���������Щ����
//CMap�Ḵ�Ƽ�ֵ���ݣ����Ḵ��ʵֵָ�룬Ϊ��Ч���밲ȫ����ֵ����ǻ������ͣ�ʵֵ�ڴ�������

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
	if(m_BaseMap==NULL) { //MapΪ��
		m_BaseMap=new Map;
		m_BaseMap->m_Key=Key;
		m_BaseMap->m_Value=Value;
		m_BaseMap->m_color=FALSE;//��ɫ
		m_BaseMap->m_NextLeft=NULL;
		m_BaseMap->m_NextRight=NULL;
		return TRUE;
	}
	Map* pTemp=m_BaseMap;
	Map* pBaseTemp=NULL;
	while(pTemp!=NULL) {
		pBaseTemp=pTemp;
		if(Key==pTemp->m_Key) { //��ֵ�ظ�
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