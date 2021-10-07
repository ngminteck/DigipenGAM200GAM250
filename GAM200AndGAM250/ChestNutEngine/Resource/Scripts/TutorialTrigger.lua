--[[

This script defines behaviour for the tutorial image trigger.

]]--

function Start(sceneactor,player)
	
	AllComponents = sceneactor:GetPrefab()
	transComp = AllComponents:GetTransformComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	tutImage = sceneactor:GetChild("TutorialImage")
	tutImageTrans = tutImage:GetPrefab():GetTransformComponent()
	tutImageAABB = tutImage:GetPrefab():GetAABBComponent()
	
	playerTrans = player:GetPrefab():GetTransformComponent()

	showImage = false
	fullImageScale = vec3.new(2, 1, 1)
	imageElevation = 1.5
	scalePerFrame = _G.DT * 2
	
	tutImageTrans:SetScale(vec3.new(0, 0, 0))

end

function Update()

	--print(tutImageAABB:GetOffset())
	ShowImage()

end

function ShowImage()

	--print("TutorialTrigger exists")
	for i = 1, #colObj do
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PLAYER" then
			showImage = true
			ScaleImageUp()
			--tutImageAABB:SetOffset(playerTrans:GetPosition() - transComp:GetPosition() + vec2.new(0, imageElevation))
			return
		end
	end
	
	showImage = false
	ScaleImageDown()

end

function ScaleImageUp()
	
	tutImage:SetActive(true)
	currentScale = tutImageTrans:GetScale()
	
	if currentScale.x < fullImageScale.x then
	
		tutImageTrans:SetScale(currentScale + fullImageScale * scalePerFrame)
		tutImageAABB:SetOffset(tutImageAABB:GetOffset() + vec2.new(0, scalePerFrame))
	
	end
	
end

function ScaleImageDown()

	currentScale = tutImageTrans:GetScale()
	
	if currentScale.x > 0 then
	
		tutImageTrans:SetScale(currentScale - fullImageScale * scalePerFrame)
		--tutImageAABB:SetOffset(playerTrans:GetPosition() - transComp:GetPosition() + vec2.new(0, imageElevation))
		tutImageAABB:SetOffset(tutImageAABB:GetOffset() - vec2.new(0, scalePerFrame))
	
	else
	
		tutImage:SetActive(false)
		--tutImageAABB:SetOffset(vec2.new(0, 0))
	
	end

end