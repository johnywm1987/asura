#ifndef _DX_SKELETON_MANAGER_H_
#define _DX_SKELETON_MANAGER_H_

#include <boost/unordered_map.hpp>

#include "../../ResourceManagerCacheAlgorithm.h"

//#include "../../../../RanThirdParty/DirectX/Include/rmxftmpl.h"
#include "../../../../RanThirdParty/DirectX/Include/rmxfguid.h"
#include "../../../../RanThirdParty/DirectX/Include/dxfile.h"
//#include "../../Common/DxStdAfx_Min.h"
#include "../../../SigmaCore/File/FileFind.h"
#include "../../../SigmaCore/gassert.h"
#include "../../../InternalCommonLib/dxstdafx.h"
#include "../../DxCommon/xrmxftmpl.h"
#include "./DxSkeletonManagerDef.h"
#include "../DxAniKeys.h"

class DxBoneTransOptimize;

#define	USE_ANI_ADD						0x0001	// �ִϸ��̼� ��ġ �߰�.
#define	USE_ANI_BLENDING				0x0002	// �ִϸ��̼� ����
#define	USE_ANI_BIP01_SPINE				0x0004	// Bone - BIP01_Spine
#define	USE_ANI_BIP01_SPINE1			0x0008	// Bone - BIP01_Spine1
#define	USE_ANI_UPDATE					0x0010	// Enable Update
#define	USE_ANI_JIGGLE_BONE				0x0020	// JiggleBone ������̴�.
#define	USE_ANI_INVERSE_SCALE_ROOT		0x0100	// Inverse Scale Root
#define	USE_ANI_INVERSE_SCALE_ROOT_CAL	0x0400	// Inverse Scale Root �������.. 
#define	USE_ANI_INVERSE_SCALE_CHILE		0x0200	// Inverse Scale Chile
#define	USE_ANI_INVERSE_SCALE_CHILE_CAL	0x0800	// Inverse Scale Chile. USE_ANI_INVERSE_SCALE_ROOT_CAL ���� ���� ����.

struct DxBoneTrans
{
	//TCHAR*			szName;					// �� �̸�.
	TSTRING         m_strName;
	EMBODY			m_emBody;				// ��ü�� ��ġ
	D3DXMATRIXA16	matRot, matRotOrig;		// Ʈ������ ��Ʈ����.
	D3DXMATRIXA16	matCombined;
	D3DXVECTOR3		vBoneScale;				// ������ �� ����� �ٲ۴�.
	float			fScaleLink;				// ������ �� ����� �ٲ۴�. ������ Link ��.

	DWORD			m_dwFlag;				// ���� define
	float			m_fScale;				// ��ü������ ������ �Ѵ�.
	SQuatPos		m_pQuatPosORIG;			// ���̳ʽ� �������� ������ ��.
	SQuatPos		m_pQuatPosCUR;			// ���ʹϾ� ��
	SQuatPos		m_pQuatPosPREV;
	D3DXQUATERNION	m_vQuatMIX;				// Ÿ�ݽ� �ͽ� �ȴ�.

	DxBoneTrans		*pBoneTransParent;		//	�θ� ���.
	DxBoneTrans		*pBoneTransSibling;		//	���� ����.
	DxBoneTrans		*pBoneTransFirstChild;	//	�ڽ� ����.

	DxBoneTrans ()
		: m_emBody(EMBODY_ROOT)
		//, szName(NULL)
		, m_dwFlag(0L)
		, vBoneScale(1.f,1.f,1.f)
		, fScaleLink(1.f)
		, m_fScale(1.f)
		, m_vQuatMIX(0.f,0.f,0.f,1.f)
		, pBoneTransParent(NULL)
		, pBoneTransSibling(NULL)
		, pBoneTransFirstChild(NULL)
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixIdentity(&matRotOrig);

		D3DXMatrixIdentity(&matCombined);
	}

	~DxBoneTrans()
	{
		//SAFE_DELETE_ARRAY(szName);
		SAFE_DELETE(pBoneTransSibling);
		SAFE_DELETE(pBoneTransFirstChild);
	}

	void AddBoneTrans ( DxBoneTrans *pBoneTrans )
	{
		if ( pBoneTransFirstChild == NULL )
		{
			pBoneTransFirstChild = pBoneTrans;
			pBoneTrans->pBoneTransParent = this;	// �θ���
		}
		else
		{
			pBoneTrans->pBoneTransSibling = pBoneTransFirstChild->pBoneTransSibling;
			pBoneTransFirstChild->pBoneTransSibling = pBoneTrans;

			pBoneTransFirstChild->pBoneTransSibling->pBoneTransParent = pBoneTrans;
			pBoneTrans->pBoneTransParent = this;	// �θ���
		}
	}

	void SetFlag_JiggleBone( BOOL bUse );

	void ResetBone_PureThread ();

	void SetName( char* _szname );
	void SetName( wchar_t* _szname );

	//! �� �̸�
	inline const TSTRING& GetName() const { return m_strName; }
};
typedef DxBoneTrans*	PDXBONE_DATA;

class BoneTransMap
{
public:
	//typedef std::map<size_t,DxBoneTrans*>	BONE_TRANS_MAP;
    typedef boost::unordered_map<TSTRING, DxBoneTrans*> BONE_TRANS_MAP;
	typedef BONE_TRANS_MAP::iterator	    BONE_TRANS_MAP_ITER;
	typedef BONE_TRANS_MAP::const_iterator	BONE_TRANS_MAP_CITER;
    typedef BONE_TRANS_MAP::value_type      BONE_TRANS_VALUE;

protected:
	BONE_TRANS_MAP m_BoneMap;
	//boost::hash<TSTRING> m_StringHash;

public:
	BONE_TRANS_MAP_ITER begin()
	{
		return m_BoneMap.begin();
	}

	BONE_TRANS_MAP_ITER end()
	{
		return m_BoneMap.end();
	}

	BONE_TRANS_MAP_CITER cbegin() const
	{
		return m_BoneMap.cbegin();
	}

	BONE_TRANS_MAP_CITER cend() const
	{
		return m_BoneMap.cend();
	}

	void insert(DxBoneTrans* pBoneTrans)
	{
		//size_t HashValue = m_StringHash(pBoneTrans->m_strName);
		//m_BoneMap.insert(BONE_TRANS_VALUE(HashValue, pBoneTrans));        
        m_BoneMap.insert(BONE_TRANS_VALUE(pBoneTrans->m_strName, pBoneTrans));
	};

	BONE_TRANS_MAP_ITER find(const TSTRING& strName)
	{
		//size_t HashValue = m_StringHash(strName);
		//return m_BoneMap.find(HashValue);
        return m_BoneMap.find(strName);
	}

	// 	DxBoneTrans* findData( const WCHAR* szName )
	// 	{
	// 		return findData( TSTRING(szName) );
	// 	}

	DxBoneTrans* findData(const TSTRING& strName) const
	{	
		//size_t HashValue = m_StringHash(strName);
		//BONE_TRANS_MAP_ITER iterFind = m_BoneMap.find(HashValue);        
        BONE_TRANS_MAP_CITER iterFind = m_BoneMap.find(strName);
		if (iterFind != m_BoneMap.end())
			return iterFind->second;
		else
			return NULL;
	}
};

//--------------------------------------------------------------------------------- 
//		CPU ������ŭ DxSkeletonPart�� �����ȴ�.
struct DxSkeletonPart
{
	DxBoneTrans*	pBoneRoot;
	BoneTransMap	BoneMap;

	BOOL			m_bUpBody;
	D3DXMATRIX		m_matTemp;				// �ӽõ�����
	D3DXMATRIX		m_matScale;				// �ӽõ�����
	D3DXMATRIX		m_matBip01_Spine;
	D3DXMATRIX		m_matUpBody_Bip01_Spine;
	D3DXMATRIX		m_matBip01_Spine_Mix;
	D3DXQUATERNION	m_quatUpBody_Bip01_Spine;

	DxBoneTrans* FindBone( const TCHAR *szName ) const
	{
		return FindBone( TSTRING(szName) );
	}

	DxBoneTrans* FindBone( const TSTRING& strName ) const
	{
		return BoneMap.findData( strName );
	}

	const DxBoneTrans* FindBone_const( const TSTRING& strName ) const
	{
		return BoneMap.findData( strName );
	}

	void CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TSTRING& strName );

	void ResetBone_PureThread();

	DxSkeletonPart()
		: pBoneRoot(NULL)
		, m_bUpBody(FALSE)
	{
	}
	~DxSkeletonPart()
	{
		SAFE_DELETE(pBoneRoot);
	}
};

struct DxSkeleton
{
	TSTRING			m_xFileName;
	DxSkeletonPart	m_skeletonPart;

	// ������ ������ �ֵ��� �Ѵ�.
	// Bone �� �ѹ� ��������� Tree �� ������ �ʾƼ� vector �� ������ �ؼ� ������ �˻��� �ϵ��� �غ���.
	std::vector<DxBoneTrans*>	m_vecReferenceBoneTrans;	// ��������.

	void DataExchange(DxSkeleton& rhs);
	HRESULT LoadBoneTrans( PDXBONE_DATA &pBone, BoneTransMap* pBoneMap, LPDIRECTXFILEDATA pxofobjCur, EMBODY emBody, BOOL bMainChar, BOOL bHaveParentBip01 );
	HRESULT LoadFileFromBuffer(const TSTRING& fileName,void* pBuffer,SIZE_T bufferSize, LPDIRECT3DDEVICEQ pd3dDevice );

	DxBoneTrans* FindBone_t( const TCHAR *szName )  const
	{
		return FindBone_t( TSTRING(szName) );
	}

	DxBoneTrans* FindBone_t( const TSTRING& strName ) const;
	const DxBoneTrans* FindBone_const_t( const TSTRING& strName) const;

	//void MultiplyScale( const TCHAR* cName, const D3DXVECTOR3& vScale, float fScaleLink );
	void MultiplyScale( const TSTRING& strName, const D3DXVECTOR3& vScale, float fScaleLink );
	//void MultiplyScaleWeight( const TCHAR* cName, float fScale );
	void MultiplyScaleWeight( const TSTRING& strName, float fScale );

	//void GetMatrixBoneBip01( DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur );
	void GetMatrixBone_Bip01_Spine_UpBody( DxSkeletonPart* pSkeletonPart, DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur );
	void GetMatrixBone_Bip01_Spine_Mix( DxSkeletonPart* pSkeletonPart, DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur );

	//void CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TCHAR* szName );
	void CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TSTRING& strName );
	void EditLineSphere( LPDIRECT3DDEVICEQ pd3dDevice );
	void EditLineSphere( LPDIRECT3DDEVICEQ pd3dDevice, DxBoneTrans *pBoneCur );

	BOOL IsCollision( D3DXVECTOR3& vFrom, D3DXVECTOR3& vLook, TSTRING& strName, const float fLength ) const;
	void CheckCollision( D3DXVECTOR3& vFrom, D3DXVECTOR3& vLook, TSTRING& strName, const float fLength, float& fBestDis, DxBoneTrans *pBoneCur ) const;

	void SetFlag_JiggleBoneAllFalse( DxBoneTrans *pBoneCur );



	//////////////////////////////////////////////////////////////////////////
	//					���� Thread ��� ȣ��Ǵ� �Լ�
	//		��� Thread ������ �� ��찡 �ְ� Main Thread ���� �� ��찡 �ִ�.
	//						���̴� �� ���´�.
	//////////////////////////////////////////////////////////////////////////

	// DxJiggleBone::Import �� ����� ��� Thread ������ �۵��ȴ�.
	// DxJiggleBone::Import ���� ��� Thread �� ������ �ʱ� ������ Thread Safe �ϴ�.
	// bMirror - Max �� Scale ���� Mirror �� �Ǿ� �ִ� xyz ���� ���̳ʽ�. �װ� �����Ѵ�.
	void UpdateBones_PureThread( DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur, float fWeight, float fParentScaleX );	//, BOOL bMirror );
	void ResetBone_PureThread();

	//////////////////////////////////////////////////////////////////////////



	// return -1 �� ���� ������.
	// �ӵ��� ����. ���� ���� ����ϱ� ���� ����.
	int GetReferenceIndex_t( const TSTRING& strName ) const;

	const DxBoneTrans* FindBone_Index_Skin( const DWORD arrayBoneIndex[], int nBoneIDInSkin ) const;	// Skin ���� Bone Index (nBoneIDInSkin) �� �������� ��� ��.
	const DxBoneTrans* FindBone_Index_Bone_const( int nBoneID ) const;									// GetReferenceIndex_t ���°��� ���� ��.
	DxBoneTrans* FindBone_Index_Bone( int nBoneID ) const;												// GetReferenceIndex_t ���°��� ���� ��.

	DxSkeleton ()
	{
	}
	~DxSkeleton ()
	{
	}
};


class DxSkeletonManager : public OnceRefAndEnableAlgorithm<DxSkeleton,EMPTY_ADD_DATA,OnceRefNTime>
{
	DxSkeletonManager();
public:
	struct Resource : public ResourceManager::Resource
	{
		Resource(const ResourceManager::Resource& res) : ResourceManager::Resource(res) {}
		Resource() {}

		DxSkeleton* get() const
		{
			MIN_ASSERT(m_spCompletedData.get());
			return m_spCompletedData.get(); 
		}
	};

public:
	static DxSkeletonManager& GetInstance();
	
	void SetPath(const TSTRING& path);
	const TSTRING& GetPath() { return m_path; }
	
	void SetDevice( LPDIRECT3DDEVICEQ pd3dDevice )	{ m_pd3dDevice = pd3dDevice; }

	Resource LoadSkeleton(const TSTRING& fileName, BOOL bThread);
	Resource LoadSkeletonAndBackUp(const TSTRING& fileName);

	bool ReLoadSkeleton(const TSTRING& fileName, Resource& res);
	bool ReLoadData(const TSTRING& fileName, bool bMustDirectory, const EMPTY_ADD_DATA& addData, Resource& res);

	BOOL ExistFile( const TSTRING& strName );

public:
    void Archive(boost::function<void (const TCHAR*)> fun);

private:
	virtual void ProcessCompleteData(
		SP_QUEUE_DATA_VECTOR& dataVec,
		NAME_N_SP_COMPLETED_DATA_VECTOR* pCompletedDataVec,
		DWORD dwPrevTick, 
		DWORD nWorkTime );

private:
	TSTRING		m_path;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	sc::CFileFindTree	m_sFileTree;

};

typedef DxSkeletonManager::Resource DxSkeletonResource;










#endif