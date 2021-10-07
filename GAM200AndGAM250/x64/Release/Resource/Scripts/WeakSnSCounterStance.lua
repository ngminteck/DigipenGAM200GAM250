--[[

This script controls this enemy's attack.

Possible states this script can transition to:
- Walk [DONE]
- Counter

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()

	-- Variables for managing state duration
	timePassed = 0
	stanceDuration = 3

end


-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	ReceiveAttack()
	CountdownToEnd()

end


function ReceiveAttack()

	-- if this object collides with the hitbox, move to counter state.

end


function CountdownToEnd()

	timePassed = timePassed + _G.DT

	if timePassed >= stanceDuration then
		
		timePassed = 0
		states:SetCurrLogicState("WALK")

	end

end
