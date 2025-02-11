#pragma once
#include "stdafx.h"

#include "CMemPool.h"
#include "dxeffsingle.h"
#include "dxeffectmesh.h"
#include "dxsimplemeshman.h"


#define	USESCALE			0x00000001	// 什追析
#define	USEROTATE			0x00000004	// 骨是 噺穿 端滴
//#define	USEMATRIAL			0x00000008	// 古闘軒杖 旋遂 巴掘益
#define	USEDIRECTION		0x00000100	// 遁走澗 号狽戚 赤倉..
#define	USETEXSCALE			0x00000200	// 努什団 潅戚奄
#define	USETEXROTATE		0x00000400	// 努什団 噺穿
#define	USETEXMOVE			0x00000800	// 努什団 噺穿
#define	USEBLUR				0x00001000	// 接雌 蓄亜
#define	USEBLENDMESH		0x00002000	// 痕発 五習 旋遂
#define	USEGOTOCENTER		0x00004000	// 湿斗 号狽生稽 戚疑 獣鉄
#define	USEOTHERTEX			0x00008000	// 陥献 努什団
#define	USESEQUENCE			0x00010000	// 獣碇什 反引 諮脊
#define	USESEQUENCELOOP		0x00020000	// 獣碇什 反引 欠覗 宜奄
//#define	USELIGHTING			0x00100000	// 笹聖 紫遂 拝 依昔亜, 源 依昔亜 ?
#define	USECULLNONE			0x00200000	// Cull Mode研 NONE稽 拝 依昔亜, 源 依昔亜 ?
#define	USESIZEXYZ			0x00400000	// size xyz 3亜走 号狽生稽 崇送析 呪 赤陥.
#define	USEHEIGHT_MESH		0x00800000	// 株戚 繕箭戚 亜管馬陥.
#define USENORMAL2			0x04000000	// 葛菰
#define USEGROUNDTEX		0x08000000	// 走檎 TEX 紫遂

struct MESH_PROPERTY : public EFF_PROPERTY
{
	enum { STRING_NUM = 256, };
	enum { TYPEID = EFFSINGLE_MESH, };
	static const char	NAME[];
	const static DWORD	VERSION;

	virtual DWORD GetTypeID() { return TYPEID; }
	virtual const char *GetName() { return NAME; }

	struct PROPERTY				// Ver. 104 ~ 105
	{
		DWORD		m_dwFlag;

		int			m_nBlurObject;		//  Note : 切重引 旭精 接雌 害奄奄 判呪 Ver.101

		int			m_nCol;			// 楳			Ver.102
		int			m_nRow;			// 伸			Ver.102
		float		m_fAniTime;		// 竺舛 獣娃	Ver.102

		char		m_szMeshFile[STRING_NUM];		//	Note: 五習 督析 戚硯
		char		m_szMeshFile1[STRING_NUM];		//	Note: 五習 督析 戚硯
		char		m_szMeshFile2[STRING_NUM];		//	Note: 五習 督析 戚硯
		char		m_szTexture[STRING_NUM];		//	Note: 努什団 戚硯		Ver.102
		int			m_nRotationType;		//	Note: 噺穿 莫殿 竺舛 
		int			m_nBlend;				//	Note: 鷺兄漁 採歳
		int			m_nPower;				//	Note: 笹税 室奄			Ver.104

		float		m_fMorphRoopNum;

		float		m_fSizeRate1;
		float		m_fSizeRate2;

		float		m_fSizeStart;
		float		m_fSizeMid1;
		float		m_fSizeMid2;
		float		m_fSizeEnd;	

		D3DXVECTOR3	m_vSizeXYZ;			// Note : 紫戚綜研 XYZ 号狽生稽 ~!		Ver.103

		float		m_fAlphaRate1;
		float		m_fAlphaRate2;

		float		m_fAlphaStart;
		float		m_fAlphaMid1;
		float		m_fAlphaMid2;
		float		m_fAlphaEnd;

		D3DXVECTOR2	m_vHeight;			// x 坦製 y 蟹掻
			
		float		m_fRotationAngle;	// Note : 噺穿 唖亀 

		float		m_fTexRotateAngel;	// Note : 努什団 噺穿
		int			m_nTexRotateType;	// Note : 努什団 噺穿 莫殿
		D3DXVECTOR3	m_vTexCenter;		// Note : 努什団 掻宿
		
		D3DXVECTOR3	m_vTexVel;			// Note : 努什団 斐牽澗 号狽

		float		m_fTexScaleStart;	// Note : 努什団 什追析 坦製葵
		float		m_fTexScaleEnd;		// Note : 努什団 什追析 魁葵

		float       m_fMaterial[4];		// Note : 袴銅軒杖 事雌 舛左 4亜走
		float       m_fMaterialRatio0;	// Note : 袴銅軒杖 硝督 旋遂 搾晴 0
		float       m_fMaterialRatio1;	// Note : 袴銅軒杖 硝督 旋遂 搾晴 1
		
		D3DXCOLOR	m_clrStart;			// Note : 袴銅軒杖 巨濃綜 鎮君税 窒降事雌	
		D3DXCOLOR	m_clrEnd;			// Note : 袴銅軒杖 巨濃綜 鎮君税 魁事雌

		PROPERTY()
		{
			memset( m_szMeshFile, 0, sizeof(char)*STRING_NUM );
			memset( m_szMeshFile1, 0, sizeof(char)*STRING_NUM );
			memset( m_szMeshFile2, 0, sizeof(char)*STRING_NUM );
			memset( m_szTexture, 0, sizeof(char)*STRING_NUM );
		};
	};
	
	union
	{
		struct
		{
			PROPERTY	m_Property;
		};

		struct
		{				 
			DWORD		m_dwFlag;

			int			m_nBlurObject;		//  Note : 切重引 旭精 接雌 害奄奄 判呪 Ver.101

			int			m_nCol;			// 楳			Ver.102
			int			m_nRow;			// 伸			Ver.102
			float		m_fAniTime;		// 竺舛 獣娃	Ver.102

			char		m_szMeshFile[STRING_NUM];		//	Note: 五習 督析 戚硯
			char		m_szMeshFile1[STRING_NUM];		//	Note: 五習 督析 戚硯
			char		m_szMeshFile2[STRING_NUM];		//	Note: 五習 督析 戚硯
			char		m_szTexture[STRING_NUM];		//	Note: 努什団 戚硯		Ver.102
			int			m_nRotationType;		//	Note: 噺穿 莫殿 竺舛 
			int			m_nBlend;				//	Note: 鷺兄漁 採歳
			int			m_nPower;				//	Note: 笹税 室奄			Ver.104

			float		m_fMorphRoopNum;

			float		m_fSizeRate1;
			float		m_fSizeRate2;

			float		m_fSizeStart;
			float		m_fSizeMid1;
			float		m_fSizeMid2;
			float		m_fSizeEnd;	

			D3DXVECTOR3	m_vSizeXYZ;			// Note : 紫戚綜研 XYZ 号狽生稽 ~!		Ver.103

			float		m_fAlphaRate1;
			float		m_fAlphaRate2;

			float		m_fAlphaStart;
			float		m_fAlphaMid1;
			float		m_fAlphaMid2;
			float		m_fAlphaEnd;

			D3DXVECTOR2	m_vHeight;			// x 坦製 y 蟹掻
				
			float		m_fRotationAngle;	// Note : 噺穿 唖亀 

			float		m_fTexRotateAngel;	// Note : 努什団 噺穿
			int			m_nTexRotateType;	// Note : 努什団 噺穿 莫殿
			D3DXVECTOR3	m_vTexCenter;		// Note : 努什団 掻宿
			
			D3DXVECTOR3	m_vTexVel;			// Note : 努什団 斐牽澗 号狽

			float		m_fTexScaleStart;	// Note : 努什団 什追析 坦製葵
			float		m_fTexScaleEnd;		// Note : 努什団 什追析 魁葵

			float       m_fMaterial[4];		// Note : 袴銅軒杖 事雌 舛左 4亜走
			float       m_fMaterialRatio0;	// Note : 袴銅軒杖 硝督 旋遂 搾晴 0
			float       m_fMaterialRatio1;	// Note : 袴銅軒杖 硝督 旋遂 搾晴 1
			
			D3DXCOLOR	m_clrStart;			// Note : 袴銅軒杖 巨濃綜 鎮君税 窒降事雌	
			D3DXCOLOR	m_clrEnd;			// Note : 袴銅軒杖 巨濃綜 鎮君税 魁事雌
		};
	};

public:
	DxSimMesh*			m_pMesh;
	LPDIRECT3DTEXTUREQ	m_pTexture;
		
	virtual DxEffSingle* NEWOBJ ();
	virtual HRESULT LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT SaveFile ( CSerialFile &SFile );

	virtual HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects();

	MESH_PROPERTY () : EFF_PROPERTY (),	
		m_nBlurObject(0),

		m_nCol(4),			// 楳
		m_nRow(4),			// 伸
		m_fAniTime(0.03f),		// 竺舛 獣娃

		m_nRotationType(0),
		m_fRotationAngle(3.0f),

		m_fMorphRoopNum(4.f),

		m_fSizeRate1(25.f),
		m_fSizeRate2(75.f),

		m_fSizeStart(0.f),
		m_fSizeMid1(3.f),
		m_fSizeMid2(2.f),
		m_fSizeEnd(5.f),

		m_vSizeXYZ(2.f,1.f,2.f),

		m_fAlphaRate1(25.0f),
		m_fAlphaRate2(75.0f),

		m_fAlphaStart(0.0f),
		m_fAlphaMid1(1.0f),
		m_fAlphaMid2(1.0f),
		m_fAlphaEnd(0.0f),

		m_vHeight(0.f, 5.f),

		m_fTexRotateAngel(2.f),
		m_nTexRotateType(0),
		m_vTexCenter(D3DXVECTOR3(0.f,0.f,0.f)),

		m_vTexVel(D3DXVECTOR3(0.f,0.f,0.f)),

		m_fTexScaleStart(1.f),
		m_fTexScaleEnd(0.f),

		m_pTexture( NULL ),
		m_pMesh( NULL )
	{	
		m_dwFlag = 0;

		//m_dwFlag |= USELIGHTING;

		m_nPower	= 0;
		m_nBlend	= 0;
		StringCchCopy( m_szMeshFile,	STRING_NUM, "sd_stone.X" );
		StringCchCopy( m_szMeshFile1,	STRING_NUM, "Dolphin2.x" );
		StringCchCopy( m_szMeshFile2,	STRING_NUM, "Dolphin3.x" );
		StringCchCopy( m_szTexture,		STRING_NUM, "1d_Lighting.bmp" );
		
		m_clrStart = D3DXCOLOR( 1.0f,1.0f,1.0f,1.0f);
		m_clrEnd   = D3DXCOLOR( 1.0f,1.0f,1.0f,1.0f);
	};

};

//----------------------------------- 姥疑 五習 獣拙 ---------------------------------------//
class DxEffectMesh : public DxEffSingle
{
private:
	float					m_fScale0;		//	Note: 砧鯵税 什追析 舛左 煽舌
	float					m_fScale1;		//  Note: 雌疑 
	float					m_fTime0;		//	Note: 什追析拭 魚献 獣娃 搾晴 
	float					m_fTime1;		//	Note: 什追析拭 魚献 獣娃 搾晴
	float					m_fAngle;		//	Note: 唖亀 
	D3DXVECTOR3				m_vDistance;	//	Note: 暗軒

public:
	const static DWORD TYPEID;
	const static DWORD FLAG;
	const static char  NAME[];


	float			m_fElapsedTime;			//せせせせせせせせせせせ
	D3DXMATRIX		m_matWorld;

public:	
	
	int			m_nBlend;					//	Note: 鷺兄漁 採歳
	int			m_nPower;					//	Note: 笹税 室奄
    DWORD		m_dwFlag;

	int			m_nBlurObject;

	int			m_iAllSprite;				// 穿端 什覗虞戚闘 姐呪
	int			m_nCol;						// 楳	Note : Ver.1.02
	int			m_nRow;						// 伸
	float		m_fAniTime;					// 竺舛 獣娃

	int			m_nRotationType;			//	Note: 噺穿 莫殿 衣舛
	int			m_nDelta;					//	Note: 什追析引 獣娃 戚疑 昔畿什 
	int			m_nMaterialDelta;			//	Note: 袴銅軒杖 痕鉢 痕是

	float		m_fRotationAngle;			//	Note: 噺穿 唖亀 

	float		m_fMorphRoopNum;

	float		m_fSizeStart;
	float		m_fAlphaStart;

	float		m_fSize;

	float		m_fSizeTime1;
	float		m_fSizeTime2;

	float		m_fSizeDelta1;
	float		m_fSizeDelta2;
	float		m_fSizeDelta3;

	D3DXVECTOR3	m_vSizeXYZ;
	D3DXVECTOR3	m_vSizeXYZEnd;

	float		m_fAlpha;

	float		m_fAlphaTime1;
	float		m_fAlphaTime2;

	float		m_fAlphaDelta1;
	float		m_fAlphaDelta2;
	float		m_fAlphaDelta3;

	float		m_fHeight;			// Note : 株戚拭 淫廃. ^^;
	float		m_fHeightStart;
	float		m_fHeightDelta;

	float		m_fTexRotateAngel;	// Note: 努什団 噺穿
	int			m_nTexRotateType;	// Note: 努什団 噺穿 莫殿
	D3DXVECTOR3	m_vTexCenter;		// Note: 努什団 掻宿

	D3DXVECTOR3	m_vTexVel;			// Note: 努什団 斐牽澗 号狽
	D3DXVECTOR3	m_vTexVelSum;		// Note: 努什団 斐牽澗 紗亀 置曽 杯

	float		m_fTexScale;		// Note: 
	float		m_fTexScaleDelta;	// Note: 

	float       m_fMaterial[4];				//	Note: 袴銅軒杖 獣娃 遁湿闘 搾晴 
	float		m_fMaterialTime[4];			//	Note: 袴銅軒杖 獣娃 舛左 

	D3DXCOLOR	m_clrStart;					//	Note: 袴銅軒杖 巨濃綜 鎮君税 窒降事雌	
	D3DXCOLOR	m_clrEnd;					//	Note: 袴銅軒杖 巨濃綜 鎮君税 魁事雌
	D3DXCOLOR	m_cColorVar;

	static LPDIRECT3DSTATEBLOCK9	m_pSavedRenderStats;
	static LPDIRECT3DSTATEBLOCK9	m_pDrawRenderStats;	

	std::string			m_strTexture;
	LPDIRECT3DTEXTUREQ	m_pTexture;					//	Note : Ver.1.02
	DxSimMesh*			m_pMesh;					//	Note: 兄希元 五習 煽舌 採歳 

	std::string			m_szCollTex;
	LPDIRECT3DTEXTUREQ	m_pCollTEX;		// 中宜吉 努什団.

public:
	virtual DWORD GetTypeID() { return TYPEID; }
	virtual DWORD GetFlag()	  { return FLAG; }
	virtual const char* GetName() { return NAME; }

protected:
	D3DXVECTOR2			m_pSequenceUV[4];
	DWORD				m_dwVertices;

	int			m_iNowSprite;				// 走榎 橿汐税 什覗虞戚闘 腰硲
	float		m_fTimeSum;
	BOOL		m_bSequenceRoop;			// 獣碇什 欠覗

public:
	static HRESULT CreateDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	static HRESULT ReleaseDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	static void OnInitDevice_STATIC();
	static void OnDeleteDevice_STATIC();

public:
	DxEffectMesh(void);
	~DxEffectMesh(void);

public:
	virtual void ReStartEff ();
	virtual void CheckAABBBox( D3DXVECTOR3& vMax, D3DXVECTOR3& vMin );

public:
	HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT InvalidateDeviceObjects ();

public:
	virtual HRESULT FrameMove ( float fTime, float fElapsedTime );
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice, D3DXMATRIX &matComb );
	virtual HRESULT FinalCleanup ();

public:
	typedef CMemPool<DxEffectMesh>	MESHPOOL;
    static MESHPOOL*				m_pPool;
};
