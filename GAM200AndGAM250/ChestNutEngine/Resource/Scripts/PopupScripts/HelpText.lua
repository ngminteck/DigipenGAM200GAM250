
function Start(sceneactor,Player)
  obj = sceneactor
    text_obj = obj:GetChild("Text")
  objComp = obj:GetPrefab()
  objComp_Col = objComp:GetCollisionComponent()
end

-- Called every frame.
function Update()

  if objComp_Col:GetColPopup()
    then text_obj:SetSelfActive(true)
  else
    text_obj:SetSelfActive(false)
  end
  
end