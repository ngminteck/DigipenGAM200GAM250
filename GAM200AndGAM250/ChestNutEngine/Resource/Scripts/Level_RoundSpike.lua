--[[

This script animates the saw aka round spike.

]]--



function Start(sceneactor,player)
	
	AllComponents = sceneactor:GetPrefab()
	sprite = AllComponents:GetSpriteComponent()
	sprite:SetAnimation("SPIN",states)

end

function Update()
end