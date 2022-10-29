-- define UseEffect
CONSUME_EFFECT_ID = 669

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 5 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(7930, 1)
    controller:AddCharacterItem(1613, 1)
    controller:AddCharacterItem(4514, 50)
    controller:AddCharacterItem(8212, 20)
    controller:AddCharacterItem(10048, 1)

    bConsumeItem = true
end