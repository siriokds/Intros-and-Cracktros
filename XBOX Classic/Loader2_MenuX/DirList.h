#ifndef _DIRLIST_H
#define _DIRLIST_H

#include <xtl.h>
#include <stdio.h>


struct	cDirEntry
{
	char			szName[MAX_PATH + 4];
	__int64		nLen;
	
	DWORD			Flags;
	
};


class	cDirList
{
	int					nRet;
	void				FreeList(void);
	cDirEntry*	AllocateEntries(int num);
	int					ReallocateEntries(int num);

public:
	cDirEntry*	m_pList;
	
	int		m_nListRoom;
	int		m_nListCount;

	int		GetList(CHAR *path);
	
	cDirList(void);
	~cDirList();
};



#endif	//_DirLIST_H