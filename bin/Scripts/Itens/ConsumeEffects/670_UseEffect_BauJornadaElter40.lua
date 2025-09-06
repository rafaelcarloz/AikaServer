-- define UseEffect
CONSUME_EFFECT_ID = 670

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 4 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(7927, 1)
    controller:AddCharacterItem(4514, 50)
    controller:AddCharacterItem(8212, 20)
    controller:AddCharacterItem(10049, 1)

    bConsumeItem = true
end