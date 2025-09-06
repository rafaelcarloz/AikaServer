-- define UseEffect
CONSUME_EFFECT_ID = 666

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 3 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(8025, 1)
    controller:AddCharacterItem(1611, 1)
    controller:AddCharacterItem(10045, 1)

    bConsumeItem = true
end