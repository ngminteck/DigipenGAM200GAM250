
-- Called at first frame.
function Start(sceneactor,Player)
  obj = sceneactor
  flag = 0
end

-- Called every frame.
function Update()
  
  if flag == 1 then
    obj:SetSelfActive(false)
  end
  
  flag = 1
end