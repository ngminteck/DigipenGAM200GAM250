--[[

This script controls the checkpoint behaviour (torch version).

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Basic init
	AllComponents = sceneactor:GetPrefab()
	thisActor = sceneactor
	
	-- Collision
	myCollision = AllComponents:GetCollisionComponent()
	myColObj = myCollision:GetCollidedObject()
	
	-- Sprite
	sprite = AllComponents:GetSpriteComponent()
	sprite:SetSprite("Torch_Top_Off")
	
	-- States / movement component
	states = AllComponents:GetMovementComponent()
	
	-- Transform
	myTransform = AllComponents:GetTransformComponent()
	
	-- Positions to set (A = x, B = y) and rigidbody
	rb = AllComponents:GetRigidBodyComponent()
	xPos = rb:GetModA()
	yPos = rb:GetModB()
	
end


-- Called every frame.
function Update()

	for i = 1, #myColObj do
	
		if (myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PLAYER") and (thisInstanceActivated == false) then
			
			ShiftToPosition()
			
		end
		
	end

end


-- Update the current checkpoint
function ShiftToPosition()

	-- PUT ON HOLD

end