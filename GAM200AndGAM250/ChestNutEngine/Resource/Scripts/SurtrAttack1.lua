--[[

This script controls the first attack of the character.

Possible states this script can transition to:
- Attack2 [DONE]
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Big Punch [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components/Children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("PlayerHitbox")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
  sprite = AllComponents:GetSpriteComponent()
	hitboxColObj = attackHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
	
	-- Variables for managing attack
	timePassed = 0
	inputAvailTime = 0.2
	attackDuration = 0.384 -- set proper number when applicable
	proceed = false
	done = false

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
	
	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	ExecuteAttack()
	CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()
	NextAttack()

end


-- Executes the first attack.
function ExecuteAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay and done == false then
		
		attackHitbox:SetActive(true)
		
		done = true
		timePassedForHitbox = -0.5

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
		
			attackHitbox:SetActive(false)
			_G.canDoubleJump = true
			break
			
		end
	
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	-- Press attack again before attack ends
	if timePassed >= inputAvailTime and timePassed < attackDuration then

		if _G.Input_Attack() then
			proceed = true
			--attackHitbox:SetActive(false)
		end

	-- Attack ends
	elseif timePassed > attackDuration and proceed == false then

		attackHitbox:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then

		timePassed = 0
		done = false
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	timePassedForHitbox = 0
	return
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
					timePassedForHitbox = 0
					done = false
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
				timePassedForHitbox = 0
				done = false
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
				timePassed = 0
				timePassedForHitbox = 0
				done = false
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end


-- Moves on to the second attack.
function NextAttack()

	if proceed == true and timePassed > attackDuration then

		timePassed = 0
		done = false
		proceed = false
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("ATTACK2")
		timePassedForHitbox = 0
    sprite:SetAnimation("ATTACK2",states)
	end

end


-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
		timePassed = 0
		done = false
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")--]]
	else
		timePassed = 0
		done = false
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end
	timePassedForHitbox = 0

end
