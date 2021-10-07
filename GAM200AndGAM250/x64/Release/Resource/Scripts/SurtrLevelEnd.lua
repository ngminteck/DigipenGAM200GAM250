local endTimeCounter = 0
local endDuration = 3
local endScaleMult = 600.5
local test

function Start(sceneactor)
	
	myActor = sceneactor
	AllComponents = sceneactor:GetPrefab()
	curtain = sceneactor:GetChild("Black Screen")
	curtainComponents = curtain:GetPrefab()
	curtainTransform = curtainComponents:GetTransformComponent()

	rb = AllComponents:GetRigidBodyComponent()

end

function Update()
	rb:SetVel(vec2.new(0, 0))

	if endTimeCounter < endDuration then
		myActor:SetSelfActive(true)
		endTimeCounter = endTimeCounter + _G.DT
		curtainTransform:SetScale(vec3.new(endTimeCounter, endTimeCounter, 0) * endScaleMult)	
	end
	if endTimeCounter>endDuration then
		myActor:SetSelfActive(false)	
	end
end