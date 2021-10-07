--[[

This script defines behaviour for the tutorial image trigger.

]]--

function Start(sceneactor,player)
	
	AllComponents = sceneactor:GetPrefab()
	transComp = AllComponents:GetTransformComponent()
	--myCollision = AllComponents:GetCollisionComponent()
    --colObj = myCollision:GetCollidedObject()
	
	playerTrans = player:GetPrefab():GetTransformComponent()
	playerCamera = player:GetPrefab():GetCameraComponent():GetCamera()

	prevCamPos = playerCamera:GetPositionVector()
	currCamPos = vec3.new(0, 0, 0)
	paraMove = vec3.new(0, 0, 0)
	followRate = AllComponents:GetRigidBodyComponent():GetModA()

end

function Update()

	currCamPos = playerCamera:GetPositionVector()
	
	camMoveVector = currCamPos - prevCamPos
	
	paraMove = vec3.new(transComp:GetPosition().x, transComp:GetPosition().y, 0) + camMoveVector * followRate
	
	transComp:SetPosition(vec2.new(paraMove.x, paraMove.y))
	
	prevCamPos = currCamPos

end