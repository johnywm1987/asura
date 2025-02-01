#include "stdafx.h"

#include "../Common/StringUtils.h"

#include "EditMeshs.h"
#include "Collision.h"
#include "DxViewPort.h"

#include "DxSkeletonManager.h"

#define __ROOT	_T("__ROOT")

extern DWORD UPDATEBONECOUNT;

//float			DxSkeleton::g_fWeight = 1.f;

void DxBoneTrans::SetName( char* _szname )
{
	if ( _szname )
	{
		// 			WCHAR szValueW[256];
		// 			MultiByteToWideChar( CP_ACP, 0, _szname, -1, szValueW, 256 );
		// 			szValueW[256-1] = 0;
		// 
		// 			size_t nStrLen = _tcslen(szValueW)+1;
		// 			szName = new TCHAR[ nStrLen ];
		// 			StringCchCopy( szName, nStrLen, szValueW );
        m_strName = _szname;
	}
	else
	{
		// 			size_t nStrLen = _tcslen( _T("[Unnamed]") )+1;
		// 			szName = new TCHAR[ nStrLen ];
		// 			StringCchCopy( szName, nStrLen, _T("[Unnamed]") );
		m_strName = _T("[Unnamed]");
	}
}

void DxBoneTrans::SetName( wchar_t* _szname )
{
	if ( _szname )
	{
		// 			size_t nStrLen = _tcslen(_szname)+1;
		// 			szName = new TCHAR[ nStrLen ];
		// 			StringCchCopy( szName, nStrLen, _szname );
		m_strName = sc::string::unicodeToAnsi( _szname );
	}
	else
	{
		// 			size_t nStrLen = _tcslen( _T("[Unnamed]") )+1;
		// 			szName = new TCHAR[ nStrLen ];
		// 			StringCchCopy( szName, nStrLen, _T("[Unnamed]") );
		m_strName = _T("[Unnamed]");
	}
}

void DxBoneTrans::SetFlag_JiggleBone( BOOL bUse )
{
	m_dwFlag |= USE_ANI_JIGGLE_BONE;
}

void DxBoneTrans::ResetBone_PureThread ()
{
#ifdef USE_ANI_QUATERNION
	m_dwFlag &= ~USE_ANI_ADD;
	m_dwFlag &= ~USE_ANI_BLENDING;
	//m_dwFlag &= ~USE_ANI_JIGGLE_BONE;
	//m_dwFlag &= ~USE_ANI_UPDATE;
	vBoneScale = D3DXVECTOR3( 1.f, 1.f, 1.f );
	m_fScale = 1.f;
	fScaleLink = 1.f;
	D3DXQuaternionIdentity( &m_vQuatMIX );
	m_pQuatPosCUR = m_pQuatPosORIG;
	m_pQuatPosPREV = m_pQuatPosORIG;

#else
	matRot = matRotOrig;

#endif

	if ( pBoneTransFirstChild != NULL )
		pBoneTransFirstChild->ResetBone_PureThread ();
	if ( pBoneTransSibling != NULL )
		pBoneTransSibling->ResetBone_PureThread ();
}

void DxSkeletonPart::CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TSTRING& strName )
{
	DxBoneTrans* pBoneTran = FindBone( strName );
	if( !pBoneTran )	return;

	D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );
	vPos.x = pBoneTran->matCombined._41;
	vPos.y = pBoneTran->matCombined._42;
	vPos.z = pBoneTran->matCombined._43;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	EDITMESHS::RENDERSPHERE( pd3dDevice, vPos, 0.006f*DxViewPort::GetInstance().GetDistance(), NULL, 0xffff0000 );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	TRUE );
}

void DxSkeletonPart::ResetBone_PureThread()
{
	if( pBoneRoot )
	{
		pBoneRoot->ResetBone_PureThread();
	}
}

// void DxSkeleton::MultiplyScale( const TCHAR* cName, const D3DXVECTOR3& vScale, float fScaleLink )
// {
// 	MultiplyScale( TSTRING(cName), vScale, fScaleLink );
// }

void DxSkeleton::MultiplyScale( const TSTRING& strName, const D3DXVECTOR3& vScale, float fScaleLink )
{
	DxBoneTrans* pBone = FindBone_t( strName );
	if ( pBone )
	{
		pBone->vBoneScale = vScale;
		pBone->fScaleLink = fScaleLink;
	}
}

// void DxSkeleton::MultiplyScaleWeight( const TCHAR* cName, float fScale )
// {
// 	MultiplyScaleWeight( TSTRING(cName) );
// }

void DxSkeleton::MultiplyScaleWeight( const TSTRING& strName, float fScale )
{
	DxBoneTrans* pBone = FindBone_t( strName );
	if ( pBone )
	{
		pBone->vBoneScale.y *= fScale;
		pBone->vBoneScale.z *= fScale;
	}
}

void DxSkeleton::UpdateBones_PureThread( DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur, float fWeight, float fParentScaleX )	//, BOOL bMirror )
{
	if( !pBoneCur )
		return;

	//// Note : Base
	//DWORD dwFlag = pBoneCur->m_dwFlag;
	//// Note : ������Ʈ �� �ʿ䰡 ���� ����.
	//if( !(dwFlag&USE_ANI_UPDATE) )
	//{
	//	return;
	//}

	// Jiggle Bone �̶� ��� �� �ʿ䰡 ����.
	if( pBoneCur->m_dwFlag & USE_ANI_JIGGLE_BONE )
		return;

	++UPDATEBONECOUNT;

#ifndef USE_ANI_QUATERNION

	//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
	D3DXMatrixMultiply( &pBoneCur->matCombined, &pBoneCur->matRot, &matCur );

	//	Note : �ڽ� �������� ��Ʈ���� ��� ���.
	//
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while( pBoneChild )
	{
		UpdateBones_PureThread( pBoneChild, pBoneCur->matCombined, fWeight, fParentScaleX );

		pBoneChild = pBoneChild->pBoneTransSibling;
	}

#else
	// Note : Base
	SQuatPos* pQuatPos = &pBoneCur->m_pQuatPosCUR;
	DWORD dwFlag = pBoneCur->m_dwFlag;

	// Note : Animation Blending
	if( dwFlag & USE_ANI_BLENDING )
	{
		SQuatPos* pQuatPosPREV = &pBoneCur->m_pQuatPosPREV;

		// Note : ���� �۾�.
		D3DXVec3Lerp( &pQuatPos->m_vPos, &pQuatPosPREV->m_vPos, &pQuatPos->m_vPos, fWeight );
		D3DXVec3Lerp( &pQuatPos->m_vScale, &pQuatPosPREV->m_vScale, &pQuatPos->m_vScale, fWeight );
		D3DXQuaternionSlerp( &pQuatPos->m_vQuat, &pQuatPosPREV->m_vQuat, &pQuatPos->m_vQuat, fWeight );
	}

	// Note : Ani �� ���ߴٸ�..~!!
	if( dwFlag & USE_ANI_ADD )
	{
		D3DXQuaternionNormalize( &pQuatPos->m_vQuat, &pQuatPos->m_vQuat );
		D3DXQuaternionNormalize( &pBoneCur->m_vQuatMIX , &pBoneCur->m_vQuatMIX  );
		//D3DXQuaternionMultiply( &pQuatPos->m_vQuat, &pQuatPos->m_vQuat, &pBoneCur->m_vQuatMIX );
		D3DXQuaternionMultiply( &pQuatPos->m_vQuat, &pBoneCur->m_vQuatMIX, &pQuatPos->m_vQuat );
	}

	D3DXMatrixRotationQuaternion( &m_skeletonPart.m_matTemp, &pQuatPos->m_vQuat );

	float ScaleX = pQuatPos->m_vScale.x * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleY = pQuatPos->m_vScale.y * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleZ = pQuatPos->m_vScale.z * pBoneCur->m_fScale * pBoneCur->fScaleLink;

	//// [shhan][2014.09.23] Max ���� Scale ���̳ʽ� �� ����ִ� ������ �ذ��ϱ� ���� �۾�.
	////						Bone�̳� �ִϸ��̼� �� ���� Export ���� �� �������� �� �� �ִ�.
	//if ( bMirror )
	//{
	//	pQuatPos->m_vPos = -pQuatPos->m_vPos;
	//}
	//else if ( pQuatPos->m_vScale.x < 0.f )
	//{
	//	bMirror = TRUE;

	//	ScaleX = -ScaleX;
	//	ScaleY = -ScaleY;
	//	ScaleZ = -ScaleZ;
	//}

	// [shhan][2016.04.01] InverseScale �� ���� ���� �۾�.
	if ( pBoneCur->m_dwFlag & USE_ANI_INVERSE_SCALE_CHILE )
	{
		if ( (pQuatPos->m_vPos.x > 0.f && pBoneCur->m_pQuatPosORIG.m_vPos.x < 0.f) ||
			(pQuatPos->m_vPos.x < 0.f && pBoneCur->m_pQuatPosORIG.m_vPos.x > 0.f) )
		{
			pQuatPos->m_vPos = -pQuatPos->m_vPos;
		}
	}

	m_skeletonPart.m_matTemp._11 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._12 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._13 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._21 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._22 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._23 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._31 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._32 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._33 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	m_skeletonPart.m_matTemp._41 = pQuatPos->m_vPos.x * fParentScaleX;	// * pBoneCur->vBoneScale.x;
	m_skeletonPart.m_matTemp._42 = pQuatPos->m_vPos.y;
	m_skeletonPart.m_matTemp._43 = pQuatPos->m_vPos.z;

	//BYTE* pByte01 = (BYTE*)&pQuatPos->m_vScale;
	///BYTE* pByte02 = (BYTE*)&m_skeletonPart.m_matTemp;

	//__asm
	//{
	//	mov		esi,	   pByte01	   	   // esi <- 
	//	mov		edi,	   pByte02		   	   	   // edi <- ���� �����Ͱ� ����Ǿ��� ������ ������ ����

	//	movss	xmm0,	  [esi]	  	   	   	   	   // xmm0 �� ���ؽ�x~���� ����
	//	movss	xmm1,	  [esi+0x4]	  	   	   	   // xmm1 �� ���ؽ�y~�� ����
	//	movss	xmm2,	  [esi+0x8]	  	   	   	   // xmm2 �� ���ؽ�z~�� ����
	//	shufps	xmm0,	  xmm0, 0x0	  	   	   	   // xmm0�� 4ĭ��, ���ؽ�x �� ä��
	//	shufps	xmm1,	  xmm1, 0x0	  	   	   	   // xmm1�� 4ĭ��, ���ؽ�y �� ä��
	//	shufps	xmm2,	  xmm2, 0x0	  	   	   	   // xmm2�� 4ĭ��, ���ؽ�z �� ä��

	//	movaps	xmm4,	  [edi]
	//	movaps	xmm5,	  [edi+0x10]	 	   // xmm4 �� ��Ʈ���� 21, 22, 23, 24 ����
	//	movaps	xmm6,	  [edi+0x20]	 	   // xmm4 �� ��Ʈ���� 31, 32, 33, 34 ����

	//	mulps	xmm4,	  xmm0	   	   	   	   // ���ؽ� X�� ��Ʈ���� 11, 12, 13, 14 ����
	//	mulps	xmm5,	  xmm1	   	   	   	   // ���ؽ� Y�� ��Ʈ���� 21, 22, 23, 34 ����
	//	mulps	xmm6,	  xmm2	   	   	   	   // ���ؽ� Z�� ��Ʈ���� 31, 32, 33, 34 ����

	//	movaps	[edi],		xmm4
	//	movaps	[edi+0x10],	xmm5
	//	movaps	[edi+0x20],	xmm6

	//	mov		esi,		pByte01
	//	mov		eax,		[esi]
	//	mov		[edi+0x30],	eax
	//	mov		eax,		[esi+0x4]
	//	mov		[edi+0x34],	eax
	//	mov		eax,		[esi+0x8]
	//	mov		[edi+0x38],	eax
	//}	


	if( dwFlag & USE_ANI_BIP01_SPINE1 )		// Note : ��ü Bone
	{
		// Note : ��ü ������ �ִ°�
		if( m_skeletonPart.m_bUpBody )
		{
			D3DXMATRIX matUpBody_Bip01_Spine;
			matUpBody_Bip01_Spine._11 = m_skeletonPart.m_matUpBody_Bip01_Spine._11;
			matUpBody_Bip01_Spine._12 = m_skeletonPart.m_matUpBody_Bip01_Spine._12;
			matUpBody_Bip01_Spine._13 = m_skeletonPart.m_matUpBody_Bip01_Spine._13;
			matUpBody_Bip01_Spine._14 = m_skeletonPart.m_matUpBody_Bip01_Spine._14;
			matUpBody_Bip01_Spine._21 = m_skeletonPart.m_matUpBody_Bip01_Spine._21;
			matUpBody_Bip01_Spine._22 = m_skeletonPart.m_matUpBody_Bip01_Spine._22;
			matUpBody_Bip01_Spine._23 = m_skeletonPart.m_matUpBody_Bip01_Spine._23;
			matUpBody_Bip01_Spine._24 = m_skeletonPart.m_matUpBody_Bip01_Spine._24;
			matUpBody_Bip01_Spine._31 = m_skeletonPart.m_matUpBody_Bip01_Spine._31;
			matUpBody_Bip01_Spine._32 = m_skeletonPart.m_matUpBody_Bip01_Spine._32;
			matUpBody_Bip01_Spine._33 = m_skeletonPart.m_matUpBody_Bip01_Spine._33;
			matUpBody_Bip01_Spine._34 = m_skeletonPart.m_matUpBody_Bip01_Spine._34;
			matUpBody_Bip01_Spine._44 = m_skeletonPart.m_matUpBody_Bip01_Spine._44;
			matUpBody_Bip01_Spine._41 = matCur._41;
			matUpBody_Bip01_Spine._42 = matCur._42;
			matUpBody_Bip01_Spine._43 = matCur._43;

			//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
			D3DXMatrixMultiply( &pBoneCur->matCombined, &m_skeletonPart.m_matTemp, &matUpBody_Bip01_Spine );
		}
		else
		{
			//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
			D3DXMatrixMultiply( &pBoneCur->matCombined, &m_skeletonPart.m_matTemp, &matCur );
		}
	}
	else
	{
		//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
		D3DXMatrixMultiply( &pBoneCur->matCombined, &m_skeletonPart.m_matTemp, &matCur );
	}


	//	Note : �ڽ� �������� ��Ʈ���� ��� ���.
	//
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while( pBoneChild )
	{
		UpdateBones_PureThread( pBoneChild, pBoneCur->matCombined, fWeight, pBoneCur->vBoneScale.x );	//, bMirror );

		pBoneChild = pBoneChild->pBoneTransSibling;
	}

	D3DXMatrixScaling( &m_skeletonPart.m_matScale, pBoneCur->vBoneScale.x, pBoneCur->vBoneScale.y, pBoneCur->vBoneScale.z );
	D3DXMatrixMultiply( &pBoneCur->matCombined, &m_skeletonPart.m_matScale, &pBoneCur->matCombined );

	if( dwFlag & USE_ANI_BIP01_SPINE )		// Note : ��ü Bone	
	{
		// Note : ��ü ������ �ִ°�
		if( m_skeletonPart.m_bUpBody )
		{
			//pBoneCur->matCombined = DxSkeleton::m_matBip01_Spine_Mix;
			pBoneCur->matCombined._11 = m_skeletonPart.m_matUpBody_Bip01_Spine._11;
			pBoneCur->matCombined._12 = m_skeletonPart.m_matUpBody_Bip01_Spine._12;
			pBoneCur->matCombined._13 = m_skeletonPart.m_matUpBody_Bip01_Spine._13;
			pBoneCur->matCombined._14 = m_skeletonPart.m_matUpBody_Bip01_Spine._14;
			pBoneCur->matCombined._21 = m_skeletonPart.m_matUpBody_Bip01_Spine._21;
			pBoneCur->matCombined._22 = m_skeletonPart.m_matUpBody_Bip01_Spine._22;
			pBoneCur->matCombined._23 = m_skeletonPart.m_matUpBody_Bip01_Spine._23;
			pBoneCur->matCombined._24 = m_skeletonPart.m_matUpBody_Bip01_Spine._24;
			pBoneCur->matCombined._31 = m_skeletonPart.m_matUpBody_Bip01_Spine._31;
			pBoneCur->matCombined._32 = m_skeletonPart.m_matUpBody_Bip01_Spine._32;
			pBoneCur->matCombined._33 = m_skeletonPart.m_matUpBody_Bip01_Spine._33;
			pBoneCur->matCombined._34 = m_skeletonPart.m_matUpBody_Bip01_Spine._34;
			pBoneCur->matCombined._44 = m_skeletonPart.m_matUpBody_Bip01_Spine._44;

			m_skeletonPart.m_bUpBody = FALSE;
		}
	}

#endif
}

//void GetMatrixBone_Bip01_Spine_UpBody( DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur );
//void GetMatrixBone_Bip01_Spine_Mix( DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur );
void DxSkeleton::GetMatrixBone_Bip01_Spine_UpBody( DxSkeletonPart* pSkeletonPart, DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur )
{
	if( !pBoneCur )
		return;

	++UPDATEBONECOUNT;

	// Note : Base
	SQuatPos* pQuatPos = &pBoneCur->m_pQuatPosCUR;
	DWORD dwFlag = pBoneCur->m_dwFlag;
	D3DXMatrixRotationQuaternion( &pSkeletonPart->m_matTemp, &pQuatPos->m_vQuat );

	float ScaleX = pQuatPos->m_vScale.x * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleY = pQuatPos->m_vScale.y * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleZ = pQuatPos->m_vScale.z * pBoneCur->m_fScale * pBoneCur->fScaleLink;

	//float ScaleX = pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	//float ScaleY = pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	//float ScaleZ = pQuatPos->m_vScale.z * pBoneCur->m_fScale;

	pSkeletonPart->m_matTemp._11 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._12 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._13 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._21 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._22 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._23 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._31 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._32 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._33 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._41 = pQuatPos->m_vPos.x;
	pSkeletonPart->m_matTemp._42 = pQuatPos->m_vPos.y;
	pSkeletonPart->m_matTemp._43 = pQuatPos->m_vPos.z;

	//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
	D3DXMatrixMultiply( &pBoneCur->matCombined, &pSkeletonPart->m_matTemp, &matCur );

	// ���� BoneName�� Bip01��� ���⼭ ����.
	if( dwFlag & USE_ANI_BIP01_SPINE )
	{
		pSkeletonPart->m_bUpBody = TRUE;
		pSkeletonPart->m_matUpBody_Bip01_Spine = pBoneCur->matCombined;
		return;
	}

	//	Note : �ڽ� �������� ��Ʈ���� ��� ���.
	//
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while( pBoneChild )
	{
		GetMatrixBone_Bip01_Spine_UpBody( pSkeletonPart, pBoneChild, pBoneCur->matCombined );

		pBoneChild = pBoneChild->pBoneTransSibling;
	}
}

void DxSkeleton::GetMatrixBone_Bip01_Spine_Mix( DxSkeletonPart* pSkeletonPart, DxBoneTrans *pBoneCur, const D3DXMATRIX &matCur )
{
	if( !pBoneCur )
		return;

	++UPDATEBONECOUNT;

	// Note : Base
	SQuatPos* pQuatPos = &pBoneCur->m_pQuatPosCUR;
	DWORD dwFlag = pBoneCur->m_dwFlag;

	//// Note : Animation Blending
	//if( dwFlag & USE_ANI_BLENDING )
	//{
	//	SQuatPos* pQuatPosPREV = &pBoneCur->m_pQuatPosPREV;

	//	// Note : ���� �۾�.
	//	D3DXVec3Lerp( &pQuatPos->m_vPos, &pQuatPosPREV->m_vPos, &pQuatPos->m_vPos, g_fWeight );
	//	D3DXVec3Lerp( &pQuatPos->m_vScale, &pQuatPosPREV->m_vScale, &pQuatPos->m_vScale, g_fWeight );
	//	D3DXQuaternionSlerp( &pQuatPos->m_vQuat, &pQuatPosPREV->m_vQuat, &pQuatPos->m_vQuat, g_fWeight );
	//}

	D3DXMatrixRotationQuaternion( &pSkeletonPart->m_matTemp, &pQuatPos->m_vQuat );

	float ScaleX = pQuatPos->m_vScale.x * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleY = pQuatPos->m_vScale.y * pBoneCur->m_fScale * pBoneCur->fScaleLink;
	float ScaleZ = pQuatPos->m_vScale.z * pBoneCur->m_fScale * pBoneCur->fScaleLink;

	//float ScaleX = pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	//float ScaleY = pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	//float ScaleZ = pQuatPos->m_vScale.z * pBoneCur->m_fScale;

	pSkeletonPart->m_matTemp._11 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._12 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._13 *= ScaleX; // pQuatPos->m_vScale.x * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._21 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._22 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._23 *= ScaleY; // pQuatPos->m_vScale.y * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._31 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._32 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._33 *= ScaleZ; // pQuatPos->m_vScale.z * pBoneCur->m_fScale;
	pSkeletonPart->m_matTemp._41 = pQuatPos->m_vPos.x;
	pSkeletonPart->m_matTemp._42 = pQuatPos->m_vPos.y;
	pSkeletonPart->m_matTemp._43 = pQuatPos->m_vPos.z;

	//	Note : pBoneCur�� ��Ʈ������ ���. matCombined = matRot * matParent * matTrans
	D3DXMatrixMultiply( &pBoneCur->matCombined, &pSkeletonPart->m_matTemp, &matCur );

	// ���� BoneName�� Bip01��� ���⼭ ����.
	if( dwFlag & USE_ANI_BIP01_SPINE )
	{
		pSkeletonPart->m_bUpBody = TRUE;
		pSkeletonPart->m_matBip01_Spine_Mix = pBoneCur->matCombined;
		return;
	}

	//	Note : �ڽ� �������� ��Ʈ���� ��� ���.
	//
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while( pBoneChild )
	{
		GetMatrixBone_Bip01_Spine_Mix( pSkeletonPart, pBoneChild, pBoneCur->matCombined );

		pBoneChild = pBoneChild->pBoneTransSibling;
	}
}

// void DxSkeleton::CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TCHAR* szName )
// {
// 	CheckSphere( TSTRING(szName) );
// }

DxBoneTrans* DxSkeleton::FindBone_t( const TSTRING& strName) const
{
    return m_skeletonPart.FindBone(strName);
}

const DxBoneTrans* DxSkeleton::FindBone_const_t( const TSTRING& strName) const
{
	return m_skeletonPart.FindBone(strName);
}

void DxSkeleton::CheckSphere( LPDIRECT3DDEVICEQ pd3dDevice, const TSTRING& strName )
{
	DxBoneTrans* pBoneTran = FindBone_t( strName );
	if( !pBoneTran )	return;

	D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );
	vPos.x = pBoneTran->matCombined._41;
	vPos.y = pBoneTran->matCombined._42;
	vPos.z = pBoneTran->matCombined._43;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	EDITMESHS::RENDERSPHERE( pd3dDevice, vPos, 0.006f*DxViewPort::GetInstance().GetDistance(), NULL, 0xffff0000 );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	TRUE );
}

void DxSkeleton::EditLineSphere( LPDIRECT3DDEVICEQ pd3dDevice )
{
	EditLineSphere( pd3dDevice, m_skeletonPart.pBoneRoot );
}

void DxSkeleton::EditLineSphere( LPDIRECT3DDEVICEQ pd3dDevice, DxBoneTrans *pBoneCur )
{
	// 	D3DXVECTOR3 vPos(0.f,0.f,0.f);
	// 	vPos.x = pBoneCur->matCombined._41;
	// 	vPos.y = pBoneCur->matCombined._42;
	// 	vPos.z = pBoneCur->matCombined._43;
	D3DXVECTOR3 vPos(
		pBoneCur->matCombined._41,
		pBoneCur->matCombined._42,
		pBoneCur->matCombined._43 );

	EDITMESHS::RENDERSPHERE( pd3dDevice, vPos, 0.003f*DxViewPort::GetInstance().GetDistance() );

	//	Note : �ڽ� �������� ��Ʈ���� ��� ���.
	D3DXVECTOR3 vPosChild(0.f,0.f,0.f);
	D3DXVECTOR3 vDir(0.f,0.f,0.f);
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while ( pBoneChild != NULL )
	{		
		vPosChild.x = pBoneChild->matCombined._41;
		vPosChild.y = pBoneChild->matCombined._42;
		vPosChild.z = pBoneChild->matCombined._43;

		EDITMESHS::RENDERLINE( pd3dDevice, vPos, vPosChild );

		EditLineSphere( pd3dDevice, pBoneChild );

		D3DXVECTOR3 vDirX(1.f,0.f,0.f);
		D3DXVECTOR3 vDirY(0.f,1.f,0.f);
		D3DXVECTOR3 vDirZ(0.f,0.f,1.f);
		D3DXVec3TransformNormal( &vDirX, &vDirX, &pBoneChild->matCombined );
		D3DXVec3TransformNormal( &vDirY, &vDirY, &pBoneChild->matCombined );
		D3DXVec3TransformNormal( &vDirZ, &vDirZ, &pBoneChild->matCombined );

		vDir = vPosChild + vDirX*0.3f;
		EDITMESHS::RENDERLINE( pd3dDevice, vPosChild, vDir, 0xff0000 );

		vDir = vPosChild + vDirY*0.3f;
		EDITMESHS::RENDERLINE( pd3dDevice, vPosChild, vDir, 0x00ff00 );

		vDir = vPosChild + vDirZ*0.3f;
		EDITMESHS::RENDERLINE( pd3dDevice, vPosChild, vDir, 0x0000ff );

		pBoneChild = pBoneChild->pBoneTransSibling;
	}
}

BOOL DxSkeleton::IsCollision( D3DXVECTOR3& vFrom, D3DXVECTOR3& vLook, TSTRING& strName, const float fLength ) const
{
	float fBestDis = FLT_MAX;
	CheckCollision( vFrom, vLook, strName, fLength, fBestDis, m_skeletonPart.pBoneRoot );

	if( strName.size() > 0 )	return TRUE;
	else						return FALSE;
}

void DxSkeleton::CheckCollision( 
    D3DXVECTOR3& vFrom, D3DXVECTOR3& vLook, TSTRING& strName, const float fLength, float& fBestDis, DxBoneTrans *pBoneCur ) const
{
	// 	D3DXVECTOR3 vPos(0.f,0.f,0.f);
	// 	vPos.x = pBoneCur->matCombined._41;
	// 	vPos.y = pBoneCur->matCombined._42;
	// 	vPos.z = pBoneCur->matCombined._43;

	D3DXVECTOR3 vPos(
		pBoneCur->matCombined._41,
		pBoneCur->matCombined._42,
		pBoneCur->matCombined._43 );

	// ���� ��ŷ �ߴ��� üũ
	if ( COLLISION::IsCollisionLineToSphere_Limit( vFrom, vLook, vPos, fLength ) )
	{
		const D3DXVECTOR3 vDis( vFrom - vPos );
		const float fDis = D3DXVec3Length( &vDis );

		if ( strName != pBoneCur->m_strName )
		{
			if ( fDis < fBestDis )
			{
				strName = pBoneCur->m_strName;
				fBestDis = fDis;
			}	
		}
	}

	// �ڽ� �������� ��Ʈ���� ��� ���.
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while ( pBoneChild != NULL )
	{
		CheckCollision( vFrom, vLook, strName, fLength, fBestDis, pBoneChild );	
		pBoneChild = pBoneChild->pBoneTransSibling;
	}
}

void DxSkeleton::SetFlag_JiggleBoneAllFalse( DxBoneTrans *pBoneCur )
{
	pBoneCur->m_dwFlag &= ~USE_ANI_JIGGLE_BONE;

	// �ڽ� �������� ��Ʈ���� ��� ���.
	DxBoneTrans *pBoneChild = pBoneCur->pBoneTransFirstChild;
	while ( pBoneChild != NULL )
	{
		SetFlag_JiggleBoneAllFalse( pBoneChild );	
		pBoneChild = pBoneChild->pBoneTransSibling;
	}
}

void DxSkeleton::ResetBone_PureThread()
{
    m_skeletonPart.ResetBone_PureThread();
}

void DxSkeleton::DataExchange(DxSkeleton& rhs)
{
	if (this != &rhs)
	{
		std::swap(m_skeletonPart.pBoneRoot, rhs.m_skeletonPart.pBoneRoot);
		std::swap(m_skeletonPart.BoneMap, rhs.m_skeletonPart.BoneMap);

		m_skeletonPart.m_bUpBody = rhs.m_skeletonPart.m_bUpBody;
		m_skeletonPart.m_matTemp = rhs.m_skeletonPart.m_matTemp;
		m_skeletonPart.m_matScale = rhs.m_skeletonPart.m_matScale;
		m_skeletonPart.m_matBip01_Spine = rhs.m_skeletonPart.m_matBip01_Spine;
		m_skeletonPart.m_matUpBody_Bip01_Spine = rhs.m_skeletonPart.m_matUpBody_Bip01_Spine;
		m_skeletonPart.m_matBip01_Spine_Mix = rhs.m_skeletonPart.m_matBip01_Spine_Mix;
		m_skeletonPart.m_quatUpBody_Bip01_Spine = rhs.m_skeletonPart.m_quatUpBody_Bip01_Spine;
	}
}

HRESULT DxSkeleton::LoadBoneTrans ( PDXBONE_DATA &pBone, BoneTransMap* pBoneMap, LPDIRECTXFILEDATA pxofobjCur, EMBODY emBody, BOOL bMainChar, BOOL bHaveParentBip01 )
{
	//---------------------------------------------------//
	NSLoadingDirect::FrameMoveRender( FALSE );
	//---------------------------------------------------//

	HRESULT hr = S_OK;
	LPDIRECTXFILEDATA pxofobjChild = NULL;
	LPDIRECTXFILEOBJECT pxofChild = NULL;
	const GUID *type;
	DWORD cbSize;
	D3DXMATRIX *pmatNew;
	PDXBONE_DATA pBoneTransCur;
	DWORD cchName;

	//	Note : ������Ʈ�� Ÿ��.
	//
	hr = pxofobjCur->GetType(&type);
	if (FAILED(hr))	goto e_Exit;

	//	Note : Ʈ������.
	//
	if (*type == TID_D3DRMFrameTransformMatrix)
	{
		hr = pxofobjCur->GetData ( NULL, &cbSize, (PVOID*)&pmatNew );
		if (FAILED(hr))	goto e_Exit;

		//// Normal ���� �������ͷ� ������Ű�� ���� �۾�
		//D3DXMATRIX	sM = *pmatNew;
		//float fLength = sM._11*(sM._22*sM._33-sM._23*sM._32) - sM._12*(sM._21*sM._33-sM._23*sM._31) + sM._13*(sM._21*sM._32-sM._22*sM._31);
		//fLength = 1.f/fLength;
		//*pmatNew *= fLength;

		//	update the parents matrix with the new one
		pBone->matRot = pBone->matRotOrig = *pmatNew;
		D3DXMatrixToSQT( pBone->m_pQuatPosORIG.m_vScale, pBone->m_pQuatPosORIG.m_vQuat, pBone->m_pQuatPosORIG.m_vPos, pBone->matRot );

		if ( pBone->pBoneTransParent && ( (pBone->pBoneTransParent->m_dwFlag&USE_ANI_INVERSE_SCALE_ROOT_CAL) || (pBone->pBoneTransParent->m_dwFlag&USE_ANI_INVERSE_SCALE_CHILE_CAL) ) )
		{
			pBone->m_pQuatPosORIG.m_vPos = -pBone->m_pQuatPosORIG.m_vPos;
			pBone->m_dwFlag |= USE_ANI_INVERSE_SCALE_CHILE;		// �ٸ� ������ ���� ���.
			pBone->m_dwFlag |= USE_ANI_INVERSE_SCALE_CHILE_CAL;	// �̰����� ������ ������ ���İ� ������.
		}
		else if ( pBone->m_pQuatPosORIG.m_vScale.x < 0.f )
		{
			pBone->m_pQuatPosORIG.m_vScale = -pBone->m_pQuatPosORIG.m_vScale;
			pBone->m_dwFlag |= USE_ANI_INVERSE_SCALE_ROOT;		// �ٸ� ������ ���� ���.
			pBone->m_dwFlag |= USE_ANI_INVERSE_SCALE_ROOT_CAL;	// �̰����� ������ ������ ���İ� ������.
		}

		pBone->m_pQuatPosPREV = pBone->m_pQuatPosCUR = pBone->m_pQuatPosORIG;
	}
	//	Note : ���� ������.
	//
	else if (*type == TID_D3DRMFrame)
	{
		char* szFrameName = NULL;

		//	Note : �������� �̸��� �ִ��� �˻�.
		//
		hr = pxofobjCur->GetName(NULL, &cchName);
		if ( SUCCEEDED(hr) && cchName!=0 )
		{
			szFrameName = (char*) _alloca(sizeof(char)*(cchName+1));
			hr = pxofobjCur->GetName ( szFrameName, &cchName );
			if (FAILED(hr))	goto e_Exit;
		}

		// Note : �̸��� ���ٸ� �۾� �� ����.
		if( !szFrameName )	goto e_Exit;

		//	Note : �� ������ ����.
		//
		pBoneTransCur = new DxBoneTrans();
		if (pBoneTransCur == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		pBoneTransCur->SetName ( szFrameName );

		// ��ü���� ��ü���� �����Ѵ�. ( �θ� ��ü�� �ڽĵ� ��ü�̴�. )
		switch( emBody )
		{
		case EMBODY_ROOT:
			if( strcmp(szFrameName,"Bip01_Spine1")==0 )
			{
				pBoneTransCur->m_emBody = EMBODY_UPBODY;
			}
			else if( strcmp(szFrameName,"Bip01_R_Thigh")==0 )
			{
				pBoneTransCur->m_emBody = EMBODY_DOWNBODY;
			}
			else if( strcmp(szFrameName,"Bip01_L_Thigh")==0 )
			{
				pBoneTransCur->m_emBody = EMBODY_DOWNBODY;
			}
			else if( strcmp(szFrameName,"Bip01")==0 )
			{
				bHaveParentBip01 = true;
			}
			else if( bHaveParentBip01 )
			{
				pBoneTransCur->m_emBody = EMBODY_DOWNBODY;
			}

			break;

		case EMBODY_UPBODY:
			pBoneTransCur->m_emBody = EMBODY_UPBODY;
			break;

		case EMBODY_DOWNBODY:
			pBoneTransCur->m_emBody = EMBODY_DOWNBODY;
			break;
		};

		// Note : Bip01 �� Ȯ���Ѵ�.
		if( strcmp(szFrameName,"Bip01_Spine")==0 )
		{
			pBoneTransCur->m_dwFlag |= USE_ANI_BIP01_SPINE;
		}
		else if( strcmp(szFrameName,"Bip01_Spine1")==0 )
		{
			pBoneTransCur->m_dwFlag |= USE_ANI_BIP01_SPINE1;
		}

		if ( bMainChar )
		{
			if( strcmp(szFrameName,"whal")==0 ||
				strcmp(szFrameName,"Bip01_R_Finger0Nub")==0 ||
				strcmp(szFrameName,"Bip01_R_Finger1Nub")==0 ||
				strcmp(szFrameName,"Bip01_R_Finger2Nub")==0 ||
				strcmp(szFrameName,"Bip01_R_Finger3Nub")==0 ||
				strcmp(szFrameName,"Bip01_R_Finger4Nub")==0 ||
				strcmp(szFrameName,"whal_slot")==0 ||
				strcmp(szFrameName,"Bip01_L_Toe0Nub")==0 ||
				strcmp(szFrameName,"Bone04")==0 ||
				strcmp(szFrameName,"Bone18")==0 ||
				strcmp(szFrameName,"Bone37")==0 ||
				strcmp(szFrameName,"Bone30")==0 ||
				strcmp(szFrameName,"Bone24")==0 ||
				strcmp(szFrameName,"Bone27")==0 )
			{
				pBoneTransCur->m_dwFlag |= USE_ANI_INVERSE_SCALE_ROOT;
			}
			else if( strcmp(szFrameName,"Bone05")==0 ||
				strcmp(szFrameName,"Bone06")==0 ||
				strcmp(szFrameName,"Bone19")==0 ||
				strcmp(szFrameName,"Bone20")==0 ||
				strcmp(szFrameName,"Bone21")==0 ||
				strcmp(szFrameName,"Bone38")==0 ||
				strcmp(szFrameName,"Bone31")==0 ||
				strcmp(szFrameName,"Bone32")==0 ||
				strcmp(szFrameName,"Bone25")==0 ||
				strcmp(szFrameName,"Bone26")==0 ||
				strcmp(szFrameName,"Bone28")==0 ||
				strcmp(szFrameName,"Bone29")==0 )
			{
				pBoneTransCur->m_dwFlag |= USE_ANI_INVERSE_SCALE_CHILE;
			}
		}

		// Note : �ʿ���� ����
		BOOL bEraseBone(FALSE);
		if( strcmp(szFrameName,"bookbone01")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"bookbone02")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"bookbone03")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"bookbone04")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"notebook")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"notebook01")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"notebook02")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"notebook03")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}
		else if( strcmp(szFrameName,"notebook04")==0 )
		{
			SAFE_DELETE( pBoneTransCur );
			goto e_Exit;
		}

		pBoneMap->insert( pBoneTransCur );
		pBone->AddBoneTrans(pBoneTransCur);

		//	Note : ���������� Ž���ϸ鼭 ���ϸ��̼��� �о�´�.
		//		QueryInterface()�� ����Ͽ� ������Ʈ�� Ÿ���� Ȯ����.
		//
		while ( SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)) )
		{
			hr = pxofChild->QueryInterface(IID_IDirectXFileData,
				(LPVOID *)&pxofobjChild);

			if (SUCCEEDED(hr))
			{
				hr = LoadBoneTrans ( pBoneTransCur, pBoneMap, pxofobjChild, pBoneTransCur->m_emBody, bMainChar, bHaveParentBip01 );
				if (FAILED(hr))
				{
					break;
				}

				GXRELEASE(pxofobjChild);
			}

			GXRELEASE(pxofChild);
		}
	}

e_Exit:
	GXRELEASE(pxofobjChild);
	GXRELEASE(pxofChild);

	return S_OK;
}

HRESULT DxSkeleton::LoadFileFromBuffer(const TSTRING& fileName,void* pBuffer,SIZE_T bufferSize, LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	LPDIRECTXFILE pxofapi = NULL;
	LPDIRECTXFILEENUMOBJECT pxofenum = NULL;
	LPDIRECTXFILEDATA pxofobjCur = NULL;
	BoneTransMap::BONE_TRANS_MAP_CITER citer;

	m_xFileName = fileName;

	// Inverse Bone �� Update �Ǵ� ���� ���ܼ� �߰���.
	BOOL bMainChar(FALSE);
	if ( fileName == "b_m.x" || fileName == "b_w.x" || fileName == "b_m.mxf" || fileName == "b_w.mxf" )
		bMainChar = TRUE;

	//	xFile ��ü�� ����.
	hr = DirectXFileCreate(&pxofapi);
	if (FAILED(hr))
		goto e_Exit;

	//	xFile ���ø�Ʈ ���.
	hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
		D3DRM_XTEMPLATE_BYTES);
	if (FAILED(hr))
		goto e_Exit;

	//	Note : Path xFile ���.
	//

	D3DXF_FILELOADMEMORY flm;
	flm.lpMemory = pBuffer;
	flm.dSize    = bufferSize;

	//---------------------------------------------------//
	NSLoadingDirect::FrameMoveRender( FALSE );
	//---------------------------------------------------//

	//	Note : �ֻ��� ������Ʈ�� �о��.
	{
		hr = pxofapi->CreateEnumObject(
			&flm,
			DXFILELOAD_FROMMEMORY,
			&pxofenum );	
		if (FAILED(hr))
		{
			// File System �޸� ��ȯ
			goto e_Exit;
		}
		// File System �޸� ��ȯ

		//---------------------------------------------------//
		NSLoadingDirect::FrameMoveRender( FALSE );
		//---------------------------------------------------//

		m_skeletonPart.pBoneRoot = new DxBoneTrans;

		m_skeletonPart.pBoneRoot->m_strName = __ROOT;
		while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
		{
			const GUID *type;

			hr = pxofobjCur->GetType(&type);
			if (FAILED(hr))	{
				goto e_Exit;
			}

			//	Note : ���� �����´�.
			//
			if ( *type == TID_D3DRMFrame )
			{
				// ���� �κ��� �� �̻� ������ �κ��� ����.
				//---------------------------------------------------//
				NSLoadingDirect::FrameMoveRender( FALSE, FALSE );
				//---------------------------------------------------//

				BOOL bHaveParentBip01(FALSE);
				hr = LoadBoneTrans( m_skeletonPart.pBoneRoot, &m_skeletonPart.BoneMap, pxofobjCur, EMBODY_ROOT,  bMainChar, bHaveParentBip01 );
				if (FAILED(hr))	{
					goto e_Exit;
				}
			}

			GXRELEASE(pxofobjCur);
		}

		GXRELEASE(pxofenum);
	}

	// ���⼭ vector �������ε� ���� �� �� �ֵ��� �۾��Ѵ�.
	m_vecReferenceBoneTrans.clear();
	for ( citer = m_skeletonPart.BoneMap.begin(); citer!=m_skeletonPart.BoneMap.end(); ++citer )
	{
		m_vecReferenceBoneTrans.push_back( (*citer).second );
	}

e_Exit:
	GXRELEASE(pxofobjCur);
	GXRELEASE(pxofenum);
	GXRELEASE(pxofapi);

	return hr;
}

// return -1 �� ���� ������.
// �ӵ��� ����. ���� ���� ����ϱ� ���� ����.
int DxSkeleton::GetReferenceIndex_t( const TSTRING& strName ) const
{
	for ( DWORD i=0; i<m_vecReferenceBoneTrans.size(); ++i )
	{
		if ( m_vecReferenceBoneTrans[i]->m_strName == strName )
			return static_cast<int>(i);
	}

	return -1;
}

const DxBoneTrans* DxSkeleton::FindBone_Index_Skin( const DWORD arrayBoneIndex[], int nBoneID ) const
{
	int nIndex = arrayBoneIndex[nBoneID];
	if ( nIndex >= static_cast<int>( m_vecReferenceBoneTrans.size() ) )
		return NULL;

	return m_vecReferenceBoneTrans[nIndex];
}

const DxBoneTrans* DxSkeleton::FindBone_Index_Bone_const( int nIndex ) const
{
	if ( nIndex >= static_cast<int>( m_vecReferenceBoneTrans.size() ) )
		return NULL;

	return m_vecReferenceBoneTrans[nIndex];
}

DxBoneTrans* DxSkeleton::FindBone_Index_Bone( int nIndex ) const
{
	if ( nIndex >= static_cast<int>( m_vecReferenceBoneTrans.size() ) )
		return NULL;

	return m_vecReferenceBoneTrans[nIndex];
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


DxSkeletonManager::DxSkeletonManager()
{
	m_pd3dDevice = NULL;
}

DxSkeletonManager& DxSkeletonManager::GetInstance()
{
	static DxSkeletonManager sm; return sm;
}

void DxSkeletonManager::SetPath(const TSTRING& path)
{ 
	m_path = path; 
	m_sFileTree.CreateTree( path );
}

DxSkeletonManager::Resource DxSkeletonManager::LoadSkeleton( const TSTRING& fileName, BOOL bThread )
{
	// [ NOTE shhan ]
	//		���� ������ Bone�� Thread Loading�� �� ���� ����. ������ FALSE�� �ٲ۴�.
	bThread = FALSE;

	CString strFileName(fileName.c_str());	// �ҹ��ڷ� ����
	strFileName.MakeLower();

	// mxf �� ���� �Ѵٸ� �װ����� �̸��� �ٲ۴�.
	{
		CString strEncName;
		STRUTIL::ChangeExt( strFileName, strEncName, _T( ".mxf" ) );

		if ( ExistFile( TSTRING(strEncName) ) )
		{
			strFileName = strEncName;
		}
	}

	return LoadData(GetPath() + strFileName.GetString(),false,EMPTY_ADD_DATA(),bThread?true:false);
}

DxSkeletonManager::Resource DxSkeletonManager::LoadSkeletonAndBackUp( const TSTRING& fileName )
{
	Resource res = LoadSkeleton(fileName,FALSE);
	BackUpCache(res);
	return res;
}

bool DxSkeletonManager::ReLoadSkeleton(const TSTRING& fileName, Resource& res)
{
	if (res.IsValid() == false)
	{
		return false;
	}

	return ReLoadData(fileName, false, EMPTY_ADD_DATA(), res);
}

bool DxSkeletonManager::ReLoadData(const TSTRING& fileName,bool bMustDirectory,const EMPTY_ADD_DATA& addData, Resource& res)
{
	UINT nDataSize = 0;
	CTypeSmartPtr spCTypeRawData = CFileSystem::getInstance().getFile( GetPath() + fileName, nDataSize );

	COMPLTED_DATA_MAP::iterator it = m_completedDataMap.find( fileName );

	if( m_completedDataMap.end() == it )
	{
		res.get()->DataExchange( *LoadSkeleton( fileName, FALSE ).get() );
		return true;
	}

	if( spCTypeRawData != NULL )
	{
		SP_RAW_DATA& spRawData = spCTypeRawData.GetSPMemory();		

		SP_QUEUE_DATA spQueueData( new QUEUE_DATA( fileName, bMustDirectory, addData ) );
		spQueueData->m_rawDataSize = nDataSize;
		spQueueData->m_spRawData = spRawData;

		ResourceQueue<EMPTY_ADD_DATA>::SP_QUEUE_DATA_VECTOR completedRawDataVec;
		completedRawDataVec.push_back( spQueueData );

		DWORD nWorkTime = ULONG_MAX;
		NAME_N_SP_COMPLETED_DATA_VECTOR completedDataVec; 
		completedDataVec.reserve( completedRawDataVec.size() );

		ProcessCompleteData( completedRawDataVec, &completedDataVec, 0, nWorkTime );

		if (completedDataVec.empty() == false)
		{
			res.get()->DataExchange( *(completedDataVec[0].second.get()) );
			return true;
		}
	}

	return false;
}

void DxSkeletonManager::ProcessCompleteData(
	SP_QUEUE_DATA_VECTOR& dataVec,
	NAME_N_SP_COMPLETED_DATA_VECTOR* pCompletedDataVec,
	DWORD dwPrevTick, 
	DWORD nWorkTime )
{
    SP_QUEUE_DATA_VECTOR::iterator it = dataVec.begin();
    while( it != dataVec.end() )
    {
		DWORD dwTick( timeGetTime() );

        const SP_QUEUE_DATA& spResData = *it;
        ++it;

        MIN_ASSERT(!spResData->m_fileName.empty());
        MIN_ASSERT(spResData->m_spRawData != NULL);
        MIN_ASSERT(m_pd3dDevice);

        SP_COMPLETED_DATA spCompletedData( new DxSkeleton ); 
        spCompletedData->LoadFileFromBuffer(spResData->m_fileName,
            spResData->m_spRawData.get(),spResData->m_rawDataSize,m_pd3dDevice);

        pCompletedDataVec->push_back( std::make_pair(spResData->m_fileName,spCompletedData) );

#ifdef _RELEASED
		if ( NSLoadingLagTester::g_DxSkeletonManager_ProcessCompleteData < timeGetTime() - dwTick )
		{
			NSLoadingLagTester::g_DxSkeletonManager_ProcessCompleteData = timeGetTime() - dwTick;
			NSLoadingLagTester::g_strDxSkeletonManager_ProcessCompleteData = spResData->m_fileName;
		}
#endif

		//// Log�� ���ܺ���.
		//if ( dwTick > 10 )
		//	sc::writeLogInfo( sc::string::format("DxSkeletonManager Bone Load %1% - %2%", spResData->m_fileName, dwTick ) );

		if ( timeGetTime() - dwPrevTick > nWorkTime )
		{
			break;
		}
    }

    if( !dataVec.empty() )
        dataVec.erase( dataVec.begin(), it );
}

BOOL DxSkeletonManager::ExistFile( const TSTRING& strName )
{
	TSTRING strFullPath = m_sFileTree.FindPathName( strName );
	if ( strFullPath.empty() )
		return FALSE;
	else
		return TRUE;
}

void DxSkeletonManager::Archive( boost::function<void (const TCHAR*)> fun )
{
    BOOST_FOREACH(COMPLTED_DATA_MAP::value_type& it, m_completedDataMap)
    {
        fun( (m_path + it.first).c_str() );
    }

    BOOST_FOREACH(COMPLTED_DATA_MAP::value_type& it, m_backupCacheCompletedDataMap)
    {
        fun( (m_path + it.first).c_str() );
    }
}
