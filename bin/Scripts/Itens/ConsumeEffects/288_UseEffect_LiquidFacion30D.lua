-- define TypeItem 
CONSUME_EFFECT_ID = 288

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 1 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(8009, 1)
   
    bConsumeItem = true
end