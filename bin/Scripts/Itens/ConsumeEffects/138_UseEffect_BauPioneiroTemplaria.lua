-- define TypeItem 
CONSUME_EFFECT_ID = 138

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 6 then
        controller:SendMessage("inventory full!")
        return
    end

    for i = 0,3 do
        controller:AddCharacterItem((2966)+i*30, 80)
    end
    
    controller:AddCharacterItem(2526, 80)
    controller:AddCharacterItem(2816, 80)

    bConsumeItem = true
end