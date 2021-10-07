--[[

This script controls the crumbling platform.

BEHAVIOUR
- sense someone stepping on its CHILD_B'S collider component
- starts counting down
- after a few seconds, CHILD_A shakes
- after a few more seconds, child_A disappears
- after a few more seconds, child_A reappears

]]--



function Start(sceneactor,player)
	
	-- Child A: PlatformBody (the actual platform players see)
	playerBody = sceneactor:GetChild("PlatformBody")
	platformTransform = playerBody:GetPrefab():GetTransformComponent()
	
	-- Child B: PlayerSensor (the trigger box that senses the player)
	playerSensor = sceneactor:GetChild("PlayerSensor")
	sensorCollision = playerSensor:GetPrefab():GetCollisionComponent()
	
	-- Variables for timing platform fall
	timePassed = 0
	shakeStartTime = 2
	fallTime = 3
	respawnTime = 6

end


function Update()

	for i = 1, #colObj do
	
		--print(i, colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType())
		--if colObj[i]:
	
	end

end