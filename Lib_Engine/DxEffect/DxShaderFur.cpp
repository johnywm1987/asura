#include "stdafx.h"

#include "./DxEffectFur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	Note : 정적 변수 생성.
//
DWORD	DxEffectFur::m_dwEffect;

/*
char DxEffectFur::m_strEffect[] =
{
	"vs.1.0 \n" //

	//	단위 벡터로 만드는 작업
	"mov r0, v3			\n"
	"dp3 r0.w, r0, r0	\n" 
	"rsq r0.w, r0.w		\n" 
	"mul r0, r0, r0.w	\n" 

	//;compute final vert pos = (norm*offset + vert)
	"mov r4, v0			\n"
	"mul r2, r0, c29.x	\n"
	"add r1, r4, r2		\n"
	"mov r1.w, v0.w		\n"

	//;--------------------------------------------------------------
	//;	Vertex 가 World*View*Projection 에 적용, 뿌려짐

	//;// transform position
	"m4x4 oPos, r1, c22	\n"

	//;--------------------------------------------------------------------  
	//;	Normal 벡터를 얻어와서 단위 벡터로 만듬	------ r0 에 등록

	//	; Normal 벡터를 World 행렬로 변환
	"m3x3 r0, v3, c10	;\n"

	//	; 변환된 것을 단위 벡터로 만드는 작업
	"dp3 r0.w, r0, r0	\n" 
	"rsq r0.w, r0.w		\n" 
	"mul r0, r0, r0.w	\n" 

	//;-------------------------------------------------------------------- 
	//;		Diffuse 칼라를 생성

	//; The directional light source.
	"dp3 r4.x, r0, -c0		\n" //

	"mul r5, r4.x, c1		\n" //	빛 디퓨즈 와 각도
	"mul r1, r5, c26		\n" //	빛 디퓨즈 와 각도 & 매트리얼 디퓨즈
	"max r1.xyz, r1.xyz, c28.x	\n"
	"add r1.xyz, r1.xyz, c27		\n" //	빛 디퓨즈 와 각도 & 매트리얼 디퓨즈 & 엠뷰언트

	"mov oD0, r1	\n" //

	//;Here we set the diffuse alpha component
	"mov oD0.w, c29.y		\n"


	//; Set texture coord
	"mul oT0.xy, v7.xy, c29.zw	\n"
};
*/

DWORD	DxEffectFur::dwFurVertexShader[] = {
	0xfffe0100, 0x0003fffe, 0x454c4946, 0x2e727566,
	0x00687376, 0x0002fffe, 0x454e494c, 0x00000001,
	0x0002fffe, 0x454e494c, 0x00000002, 0x0002fffe,
	0x454e494c, 0x00000003, 0x0002fffe, 0x454e494c,
	0x00000003, 0x00000001, 0x800f0000, 0x90e40003,
	0x0002fffe, 0x454e494c, 0x00000004, 0x0002fffe,
	0x454e494c, 0x00000004, 0x00000008, 0x80080000,
	0x80e40000, 0x80e40000, 0x0002fffe, 0x454e494c,
	0x00000005, 0x0002fffe, 0x454e494c, 0x00000005,
	0x00000007, 0x80080000, 0x80ff0000, 0x0002fffe,
	0x454e494c, 0x00000006, 0x0002fffe, 0x454e494c,
	0x00000006, 0x00000005, 0x800f0000, 0x80e40000,
	0x80ff0000, 0x0002fffe, 0x454e494c, 0x00000007,
	0x0002fffe, 0x454e494c, 0x00000008, 0x0002fffe,
	0x454e494c, 0x00000009, 0x0002fffe, 0x454e494c,
	0x00000009, 0x00000001, 0x800f0004, 0x90e40000,
	0x0002fffe, 0x454e494c, 0x0000000a, 0x0002fffe,
	0x454e494c, 0x0000000a, 0x00000005, 0x800f0002,
	0x80e40000, 0xa000001d, 0x0002fffe, 0x454e494c,
	0x0000000b, 0x0002fffe, 0x454e494c, 0x0000000b,
	0x00000002, 0x800f0001, 0x80e40004, 0x80e40002,
	0x0002fffe, 0x454e494c, 0x0000000c, 0x0002fffe,
	0x454e494c, 0x0000000c, 0x00000001, 0x80080001,
	0x90ff0000, 0x0002fffe, 0x454e494c, 0x0000000d,
	0x0002fffe, 0x454e494c, 0x0000000e, 0x0002fffe,
	0x454e494c, 0x0000000f, 0x0002fffe, 0x454e494c,
	0x0000000f, 0x00000014, 0xc00f0000, 0x80e40001,
	0xa0e40016, 0x0002fffe, 0x454e494c, 0x00000010,
	0x0002fffe, 0x454e494c, 0x00000011, 0x0002fffe,
	0x454e494c, 0x00000011, 0x00000017, 0x800f0000,
	0x90e40003, 0xa0e4000a, 0x0002fffe, 0x454e494c,
	0x00000012, 0x0002fffe, 0x454e494c, 0x00000013,
	0x0002fffe, 0x454e494c, 0x00000014, 0x0002fffe,
	0x454e494c, 0x00000014, 0x00000008, 0x80080000,
	0x80e40000, 0x80e40000, 0x0002fffe, 0x454e494c,
	0x00000015, 0x0002fffe, 0x454e494c, 0x00000015,
	0x00000007, 0x80080000, 0x80ff0000, 0x0002fffe,
	0x454e494c, 0x00000016, 0x0002fffe, 0x454e494c,
	0x00000016, 0x00000005, 0x800f0000, 0x80e40000,
	0x80ff0000, 0x0002fffe, 0x454e494c, 0x00000017,
	0x0002fffe, 0x454e494c, 0x00000018, 0x0002fffe,
	0x454e494c, 0x00000018, 0x00000008, 0x80010004,
	0x80e40000, 0xa1e40000, 0x0002fffe, 0x454e494c,
	0x00000019, 0x0002fffe, 0x454e494c, 0x0000001a,
	0x0002fffe, 0x454e494c, 0x0000001a, 0x00000005,
	0x800f0005, 0x80000004, 0xa0e40001, 0x0002fffe,
	0x454e494c, 0x0000001b, 0x0002fffe, 0x454e494c,
	0x0000001b, 0x00000005, 0x800f0001, 0x80e40005,
	0xa0e4001a, 0x0002fffe, 0x454e494c, 0x0000001c,
	0x0002fffe, 0x454e494c, 0x0000001c, 0x0000000b,
	0x80070001, 0x80a40001, 0xa000001c, 0x0002fffe,
	0x454e494c, 0x0000001d, 0x0002fffe, 0x454e494c,
	0x0000001d, 0x00000002, 0x80070001, 0x80a40001,
	0xa0e4001b, 0x0002fffe, 0x454e494c, 0x0000001e,
	0x0002fffe, 0x454e494c, 0x0000001f, 0x0002fffe,
	0x454e494c, 0x0000001f, 0x00000001, 0xd00f0000,
	0x80e40001, 0x0002fffe, 0x454e494c, 0x00000020,
	0x0002fffe, 0x454e494c, 0x00000021, 0x0002fffe,
	0x454e494c, 0x00000021, 0x00000001, 0xd0080000,
	0xa055001d, 0x0002fffe, 0x454e494c, 0x00000022,
	0x0002fffe, 0x454e494c, 0x00000023, 0x0002fffe,
	0x454e494c, 0x00000024, 0x0002fffe, 0x454e494c,
	0x00000024, 0x00000005, 0xe0030000, 0x90540007,
	0xa0fe001d, 0x0000ffff
};
