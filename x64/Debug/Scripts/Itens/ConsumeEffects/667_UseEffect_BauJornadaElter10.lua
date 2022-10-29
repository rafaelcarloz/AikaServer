-- define UseEffect
CONSUME_EFFECT_ID = 667

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 6 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(8027, 1)
    controller:AddCharacterItem(1612, 1)
    controller:AddCharacterItem(4514, 50)
    controller:AddCharacterItem(8189, 50)
    controller:AddCharacterItem(4438, 1)
    controller:AddCharacterItem(10046, 1)

    bConsumeItem = true
end