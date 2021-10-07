--[[

This script controls the collision between a player's hitbox and the enemy.

]]--


-- Called at first frame.
function Start(sceneactor,player)

	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
    sprite = AllComponents:GetSpriteComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	
	doorPassed = false
	
	glowFX = sceneactor:GetChild("EndOfLevelGlow")
	
	animDuration = (0.025 * 12) - 0.01
	animTimer = 0

end


-- Called every frame.
function Update()

	for i = 1, #colObj do
	
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PLAYER" then
		
			sprite:SetAnimation("DOORWAY_ON",states)
			doorPassed = true
			break
		
		end
	
	end
	
	if doorPassed then
	
		print("DOOR GLOWING")
		glowFX:SetActive(true)
		animTimer = animTimer + _G.DT
		
		if animTimer >= animDuration then
		
			print("DOOR NOT GLOWING")
			glowFX:SetActive(false)
		
		end
	
	end

end
