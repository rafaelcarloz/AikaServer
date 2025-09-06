-- define UseEffect
CONSUME_EFFECT_ID = 668

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 5 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(13528, 1)
    controller:AddCharacterItem(1614, 1)
    controller:AddCharacterItem(4514, 50)
    controller:AddCharacterItem(8189, 50)
    controller:AddCharacterItem(10047, 1)

    bConsumeItem = true
end