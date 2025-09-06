-- define TypeItem 
CONSUME_EFFECT_ID = 1600

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 5 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(8009, 1)
    controller:AddCharacterItem(4271, 1)
    controller:AddCharacterItem(11451, 1)
    controller:AddCharacterItem(4480, 1)
    controller:AddCharacterItem(4481, 1)

    bConsumeItem = true
end