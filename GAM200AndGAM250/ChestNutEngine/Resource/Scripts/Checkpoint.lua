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
	
	-- States / movement component
	states = AllComponents:GetMovementComponent()
	
	-- Sprite
	sprite = AllComponents:GetSpriteComponent()
	sprite:SetAnimation("Checkpoint", states)
	
	-- Transform
	myTransform = AllComponents:GetTransformComponent()
	
	-- Variable for this instance of checkpoint
	thisInstanceActivated = false
	
end


-- Called every frame.
function Update()

	UpdateCheckpoint()

end


-- Update the current checkpoint
function UpdateCheckpoint()

	for i = 1, #myColObj do
	
		if (myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PLAYER") and (thisInstanceActivated == false) then
			
			--sprite:SetSprite("Torch_Top_On")
			states:SetCurrLogicState("ACTIVATED")
			thisInstanceActivated = true
			_G.formalRespawnPoint = myTransform:GetPosition()
						
		end
		
	end

end