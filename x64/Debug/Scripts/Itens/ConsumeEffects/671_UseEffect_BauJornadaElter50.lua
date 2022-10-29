-- define UseEffect
CONSUME_EFFECT_ID = 671

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 8 then
        controller:SendMessage("inventory full!")
        return
    end

    controller:AddCharacterItem(8199, 100)
    controller:AddCharacterItem(8253, 100)
    controller:AddCharacterItem(8186, 4)
    controller:AddCharacterItem(8188, 4)
    controller:AddCharacterItem(8207, 2)
    controller:AddCharacterItem(8210, 2)
    controller:AddCharacterItem(4483, 1)
    controller:AddCharacterItem(4487, 1)

    bConsumeItem = true
end