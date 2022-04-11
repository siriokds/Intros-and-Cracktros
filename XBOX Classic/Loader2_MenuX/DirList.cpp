#include "DirList.h"


/*------------------------------------------------------*/
cDirEntry*	cDirList::AllocateEntries(int num)
/*------------------------------------------------------*/
{
cDirEntry*	pList = new cDirEntry[num];

	if (pList) ZeroMemory(pList, sizeof(cDirEntry)*num);
	
	return pList;
}


/*------------------------------------------------------*/
int		cDirList::ReallocateEntries(int num)
/*------------------------------------------------------*/
{
		if (num <= m_nListRoom) return -2;

cDirEntry*	dList = AllocateEntries(num);

		if (dList == 0) return -1;
		
		CopyMemory( dList, m_pList, sizeof(cDirEntry) * m_nListCount );
		
		m_nListRoom = num;
		
		delete [] m_pList;
		
		m_pList = dList;
		
		return 0;
}


/*------------------------------------------------------*/
cDirList::cDirList(void)
/*------------------------------------------------------*/
{
		m_pList = AllocateEntries(1024);
		m_nListRoom = 1024;
		m_nListCount = 0;
		
		
}


/*------------------------------------------------------*/
void	cDirList::FreeList(void)
/*------------------------------------------------------*/
{
		if (m_pList != 0) 
		{
			delete [] m_pList;
			m_pList = 0;
		}
		
		m_nListRoom = 0;
		m_nListCount = 0;
};




/*------------------------------------------------------*/
cDirList::~cDirList()
/*------------------------------------------------------*/
{
	FreeList();
}



/*-------------------------------------------------------------*/
static int namecompare( const void *arg1, const void *arg2 )
/*-------------------------------------------------------------*/
{
  cDirEntry* a = (cDirEntry*)arg1;
  cDirEntry* b = (cDirEntry*)arg2;

  return _stricmp(a->szName,b->szName);
}


/*------------------------------------------------------*/
int	cDirList::GetList(CHAR *path)
/*------------------------------------------------------*/
{
		if ((m_pList == 0) || (m_nListRoom == 0)) return 1;

	
char 	t_szPath[MAX_PATH + 1];

		m_nListCount = 0;
		
		lstrcpy(t_szPath, path);
		lstrcat(t_szPath, "\\*");


WIN32_FIND_DATAA oFindData;
	
	HANDLE	hFind = FindFirstFile(t_szPath, &oFindData);


	if (hFind == INVALID_HANDLE_VALUE) return GetLastError();
	
	do
	{
		if ((oFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0) 
		{
			strcpy( m_pList[m_nListCount].szName, "F");
			m_pList[m_nListCount].nLen = (oFindData.nFileSizeHigh * (MAXDWORD+1)) + oFindData.nFileSizeLow;
		}
		else
		{
			strcpy( m_pList[m_nListCount].szName, "D");
			m_pList[m_nListCount].nLen = 0;
		}
		
		strncat( m_pList[m_nListCount].szName, oFindData.cFileName, MAX_PATH - 4);
		
		if (++m_nListCount >= m_nListRoom) 
		{
			nRet = ReallocateEntries(m_nListRoom * 2);
			if (nRet != 0) break;
		}

	} while (FindNextFile(hFind, &oFindData));
	
	FindClose(hFind);

	qsort( (void *)m_pList, (size_t)m_nListCount, sizeof( cDirEntry ), namecompare);

	return 0;
}




/*-----------------------------------------------------------------------------------*/
int	cDirList::LoadFile(unsigned char *dst, int nEntry, int nMaxLen)
/*-----------------------------------------------------------------------------------*/
{
	if (nEntry >= m_nListCount) return 10000;
	if (m_pList[nEntry].nLen > nMaxLen) return 10001;

	lstrcpy(t_szDirs,szDirs);
	lstrcat(t_szDirs,"\\");
	lstrcat(t_szDirs,m_pList[nEntry].szName);


	FILE *fp = fopen(t_szDirs,"rb");

	if (fp == NULL) return 10002;

	m_nReaded = fread(dst,1,m_pList[nEntry].nLen,fp);
	fclose(fp);
	
	if (m_nReaded != m_pList[nEntry].nLen) return 10003;

	return 0;
}
