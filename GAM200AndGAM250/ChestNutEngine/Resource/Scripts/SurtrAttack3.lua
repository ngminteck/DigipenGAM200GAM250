--[[

This script controls the second attack of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Big Punch [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	myCollision = AllComponents:GetCollisionComponent()
	colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("PlayerHitbox")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
  sprite = AllComponents:GetSpriteComponent()
	hitboxColObj = attackHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
	
	-- Variables for managing attack
	timePassed = 0
	attackDuration = 0.576

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.2 * 0
	
	spearID = "Spear "
	spearEnemyID = "SpearThrowEnemy"
	landEnemyID = "SmallLandEnemy"
	sawProjID = "Saw_Projectile"
	damagingNames = { spearID, sawProjID }

end


-- Called every frame.
function Update()

	ExecuteAttack()
	--CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()

end


-- Executes the third attack.
function ExecuteAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay then

		attackHitbox:SetActive(true)
		timePassedForHitbox = 0

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()
		
	--[[
	if attackHitbox:GetPrefab():GetCollisionComponent():GetColPlayerHitbox() == true then
		attackHitbox:SetActive(false)
	end
	--]]
	
	for i = 1, #hitboxColObj do
	
		--print(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
		if string.sub(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"), 1, string.len(spearID)) == spearID then
		
			rb:SetVel(vec2.new(rb:GetVel().x, spearHitBoost))
			attackHitbox:SetActive(false)
			_G.canDoubleJump = true
			break
			
		end
	
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	if timePassed >= attackDuration then

		timePassed = 0
		attackHitbox:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then
	
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("FALL")
		sprite:SetAnimation("FALL",states)
	end
	
	CheckCollision()

end

-- Checks if Surtr is hurt
function CheckCollision()

	if myStats:GetCooldown("INVULN") <= 0.0 then
		
		validHit = false
		invulnEffect:SetActive(false)
	
		for i = 1, #colObj do
		
			for j = 1, #damagingNames do
			
				--print(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
				if string.sub(colObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"), 1, string.len(damagingNames[j])) == damagingNames[j] then
					
					print(colObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
					timePassed = 0
					attackHitbox:SetActive(false)
					states:SetCurrLogicState("HURT")
					sprite:SetAnimation("HURT",states)
					validHit = true
					break
					
				end
				
			end
			
			if validHit then break end
			
			if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "RESPAWNBOX" then
				_G.respawnPoint = colObj[i]:GetPrefab():GetTransformComponent():GetPosition()
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
				timePassed = 0
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
				timePassed = 0
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end


-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")--]]
	else
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end

end
