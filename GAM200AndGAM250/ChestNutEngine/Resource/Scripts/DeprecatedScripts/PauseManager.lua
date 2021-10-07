
function Start(sceneactor)
  self = sceneactor
  pause_status = self:GetActive()
  count = 0
  
  resume_game_button = sceneactor:GetChild("Pause Resume Game Button")
  how_to_play_button = sceneactor:GetChild("Pause How To Play Button")
  quit_game_button = sceneactor:GetChild("Pause Quit Game Button")
  
  resume_game_hover_button = sceneactor:GetChild("Pause Resume Game Hover Button")
  how_to_play_hover_button = sceneactor:GetChild("Pause How To Play Hover Button")
  quit_game_hover_button = sceneactor:GetChild("Pause Quit Game Hover Button")
  
  how_to_play_menu = sceneactor:GetChild("How To Play Menu")
  back_button = sceneactor:GetChild("Back Button")
  
  in_how_to_play_menu = false
end

function Update()
  
  if _G.i:KeyDown(CVK_ESC) then
    if pause_status = true then
      self:SetActive(false)
    else
      self:SetActive(true)
      how_to_play_menu:SetSelfActive(false)
      back_button:SetSelfActive(false)
  
      --hover set to false
    end
	end
  
  if pause_status = true then
    ButtonCommand()
  end

end

function ButtonCommand()
  -- this chekc is for is how to play menu selected
  if in_how_to_play_menu == true then
     if _G.i:KeyDown(CVK_ENTER) then
      how_to_play_menu:SetSelfActive(false)
      back_button:SetSelfActive(false)
      in_how_to_play_menu = false
     end
  else
    if _G.i:KeyDown(CVK_UP) then
      if count == 0 then
        count = 2
      else
        count = count - 1
      end
    
    elseif M_G.i:KeyDown(CVK_DOWN) then
      if count == 2 then
        count = 0
      else
        count = count + 1
      end
    
    elseif M_G.i:KeyDown(CVK_ENTER) then
      if count == 0 then
        self:SetActive(false)
      elseif count == 1 then
        how_to_play_menu:SetSelfActive(true)
        back_button:SetSelfActive(true)
        in_how_to_play_menu = true
      else
    
      end
    end
    
    if count == 0 then
      resume_game_button:SetSelfActive(false)
      resume_game_hover_button:SetSelfActive(true)
      how_to_play_button:SetSelfActive(true)
      how_to_play_hover_button:SetSelfActive(false)
      quit_game_button:SetSelfActive(true)
      quit_game_hover_button:SetSelfActive(false)
      
    else if count == 1 then
      resume_game_button:SetSelfActive(true)
      resume_game_hover_button:SetSelfActive(false)
      how_to_play_button:SetSelfActive(false)
      how_to_play_hover_button:SetSelfActive(true)
      quit_game_button:SetSelfActive(true)
      quit_game_hover_button:SetSelfActive(false)
      
    else if count == 2 then
      resume_game_button:SetSelfActive(true)
      resume_game_hover_button:SetSelfActive(false)
      how_to_play_button:SetSelfActive(true)
      how_to_play_hover_button:SetSelfActive(false)
      quit_game_button:SetSelfActive(false)
      quit_game_hover_button:SetSelfActive(true)
    
    end
  end
