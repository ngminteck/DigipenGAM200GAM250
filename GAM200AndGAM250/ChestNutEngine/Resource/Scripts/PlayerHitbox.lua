--[[

This script controls the collision between a player's hitbox and the enemy.

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components and children
	self = sceneactor
	--print("The hitbox exists")

end


-- Called every frame.
function Update()

	print("hitbox updating")

	if isCollidingvsEnemy == true then
		self:SetActive(false)
	end

end
