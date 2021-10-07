local endTimeCounter = 0
local endDuration = 3
local endScaleMult = 600.5
local test

function Start(sceneactor)
	
	myActor = sceneactor
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	--curtain = sceneactor:GetChild("Black Screen")
	--curtainComponents = curtain:GetPrefab()
	--curtainTransform = curtainComponents:GetTransformComponent()
	word1 = sceneactor:GetChild("LevelEndWord_Level")
	word1Transform = word1:GetPrefab():GetTransformComponent()
	word2 = sceneactor:GetChild("LevelEndWord_Clear")
	word2Transform = word2:GetPrefab():GetTransformComponent()
	words = sceneactor:GetChild("LevelEndWord_LevelClear")
	wordsTransform = words:GetPrefab():GetTransformComponent()
	sprite = AllComponents:GetSpriteComponent()

	rb = AllComponents:GetRigidBodyComponent()

	scaleTimer = 0
	scaleDuration = 0.25
	endScale = vec3.new(8, 2, 1)
	word1EndLocation = vec2.new(-2, 1)
	word2EndLocation = vec2.new(2, 1)
	word1Done = false
	
	endTimer = 0
	endDuration = 2

end

function Update()
	rb:SetVel(vec2.new(0, 0))
    sprite:SetAnimation("IDLE",states)
	_G.audio:Play("Resource/Audio/Win.wav", vec3.new(0,0,0), 1.0, false)

	
	words:SetActive(true)
	
		if scaleTimer < scaleDuration then
		
			scaleMult = scaleTimer / scaleDuration
			scaleTimer = scaleTimer + _G.DT
			wordsTransform:SetScale(endScale * scaleMult)
		
		else
		
			if endTimer < endDuration then
			
				endTimer = endTimer + _G.DT
			
			else
			
				myActor:SetActive(false)
				
			end
		
		end
	
	--[[if (word1Done == false) then
	
		word1:SetActive(true)
	
		if scaleTimer < scaleDuration then
		
			scaleMult = scaleTimer / scaleDuration
			scaleTimer = scaleTimer + _G.DT
			word1Transform:SetScale(endScale * scaleMult)
		
		else
		
			word1Done = true
			scaleTimer = 0
		
		end
		
	else
	
		word2:SetActive(true)
		
		if scaleTimer < scaleDuration then
		
			scaleMult = scaleTimer / scaleDuration
			scaleTimer = scaleTimer + _G.DT
			word2Transform:SetScale(endScale * scaleMult)
		
		end
	
	end--]]

	--[[if endTimeCounter < endDuration then
		myActor:SetSelfActive(true)
		endTimeCounter = endTimeCounter + _G.DT
		curtainTransform:SetScale(vec3.new(endTimeCounter, endTimeCounter, 0) * endScaleMult)	
	end
	if endTimeCounter>endDuration then
		myActor:SetSelfActive(false)	
	end--]]
end