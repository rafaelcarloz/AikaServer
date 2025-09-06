-- define TypeItem 
CONSUME_EFFECT_ID = 139

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 5 then
        controller:SendMessage("inventory full!")
        return
    end

    for i = 0,3 do
        controller:AddCharacterItem((3086)+i*30, 80)
    end
    
    controller:AddCharacterItem(2701, 80)

    bConsumeItem = true
end