#pragma once

enum TYRANNY_REGISTER_FB
{
	TYRANNY_REGISTER_FB_FAILED			= 0,
	TYRANNY_REGISTER_FB_NOTPOSSIBLE		= 1,
	TYRANNY_REGISTER_FB_INVALID_CHANNEL	= 2,
	TYRANNY_REGISTER_FB_INVALID_CHARID	= 3,
	TYRANNY_REGISTER_FB_INVALID_SCHOOL	= 4,
	TYRANNY_REGISTER_FB_LEVEL_LOW		= 5,

	TYRANNY_REGISTER_FB_ALREADY_REG		= 6,
	TYRANNY_REGISTER_FB_ALREADY_QUEUE	= 7,
	TYRANNY_REGISTER_FB_QUEUED			= 8,
	TYRANNY_REGISTER_FB_REGISTERED		= 9,
	TYRANNY_REGISTER_FB_CANCEL_REG		= 10,
	TYRANNY_REGISTER_FB_CANCEL_FAIL		= 11,
};

enum TYRANNY_REJOIN_FB
{
	TYRANNY_REJOIN_FB_FAILED			= 0,
	TYRANNY_REJOIN_FB_NOTPOSSIBLE		= 1,
	TYRANNY_REJOIN_FB_NOT_REGISTERED	= 2,
	TYRANNY_REJOIN_FB_INVALID_CHARID	= 3,
	TYRANNY_REJOIN_FB_INBATTLE			= 4,
};