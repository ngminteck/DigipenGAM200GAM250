CVK_CTRL	= 0x11
CVK_SPACE	= 0x20
CVK_ESC		= 0x1B
CVK_TILDE	= 0xC0
CVK_ENTER = 0x0D

CVK_LEFT	= 0x25
CVK_UP		= 0x26
CVK_RIGHT	= 0x27
CVK_DOWN	= 0x28

CVK_0	=	0x30
CVK_1	=	0x31
CVK_2	=	0x32
CVK_3	=	0x33
CVK_4	=	0x34
CVK_5	=	0x35
CVK_6	=	0x36
CVK_7	=	0x37
CVK_8	=	0x38
CVK_9	=	0x39

CVK_A	=	0x41
CVK_B	=	0x42
CVK_C	=	0x43
CVK_D	=	0x44
CVK_E	=	0x45
CVK_F	=	0x46
CVK_G	=	0x47
CVK_H	=	0x48
CVK_I	=	0x49
CVK_J	=	0x4A
CVK_K	=	0x4B
CVK_L	=	0x4C
CVK_M	=	0x4D
CVK_N	=	0x4E
CVK_O	=	0x4F
CVK_P	=	0x50
CVK_Q	=	0x51
CVK_R	=	0x52
CVK_S	=	0x53
CVK_T	=	0x54
CVK_U	=	0x55
CVK_V	=	0x56
CVK_W	=	0x57
CVK_X	=	0x58
CVK_Y	=	0x59
CVK_Z	=	0x5A

BUTTON_A = 0
BUTTON_B = 1
BUTTON_X = 2
BUTTON_Y = 3
BUTTON_RT = 4
BUTTON_RB = 5
BUTTON_LT = 6
BUTTON_LB = 7
BUTTON_START = 8
BUTTON_BACK = 9
LSTICK = 10
LSTICK_UP = 11
LSTICK_DOWN = 12
LSTICK_LEFT = 13
LSTICK_RIGHT = 14
RSTICK = 15
DPAD_UP = 16
DPAD_DOWN = 17
DPAD_LEFT = 18
DPAD_RIGHT = 19




KEY_JUMP 	= CVK_SPACE
KEY_ALTJUMP	= CVK_W
KEY_ATTACK	= CVK_K
KEY_DASH	= CVK_J
KEY_UP		= CVK_W
KEY_LEFT	= CVK_A
KEY_DOWN	= CVK_S
KEY_RIGHT	= CVK_D







DT		= 1.0/60.0

TotalHits = 0

-- Not sorry
canDash = true
canJump = true
canDoubleJump = true
canMiracleJump = true
isMiracleJumping = false
deathAltitude = -50
respawnPoint = nil
formalRespawnPoint = nil
mod1 = 1
mod2 = 1
mod3 = 1
mod4 = 1
mod5 = 1
mod6 = 1
mod7 = 1
mod8 = 1
bossStage = false

-- For dynamic camera
cameraInFlag = false
cameraOutFlag = false

bossActionSequence = 0

gameWon = false

function Start(input, xinput, audiosystem)
i = input
xbox = xinput
audio = audiosystem
end

function Input_MoveLeft()

	if i:KeyDown(KEY_LEFT) or xbox:KeyDown(LSTICK_LEFT) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_MoveRight()

	if i:KeyDown(KEY_RIGHT) or xbox:KeyDown(LSTICK_RIGHT) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_MoveUp()

	if i:KeyDown(KEY_UP) or xbox:KeyDown(LSTICK_UP) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_MoveDown()

	if i:KeyDown(KEY_DOWN) or xbox:KeyDown(LSTICK_DOWN) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_Jump()

	if i:KeyDown(KEY_JUMP) or i:KeyDown(KEY_ALTJUMP) or xbox:KeyDown(BUTTON_A) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_Dash()

	if i:KeyDown(KEY_DASH) or xbox:KeyDown(BUTTON_LB) then
	
		return true
		
	else
	
		return false
		
	end

end

function Input_Attack()

	if i:KeyDown(KEY_ATTACK) or xbox:KeyDown(BUTTON_RB) then
	
		return true
		
	else
	
		return false
		
	end

end