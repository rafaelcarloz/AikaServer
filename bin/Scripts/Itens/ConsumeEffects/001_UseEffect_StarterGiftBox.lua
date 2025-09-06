-- define UseEffect
CONSUME_EFFECT_ID = 001

bConsumeItem = false

-- init
function run(controller)
    if controller:GetAvailableSlots() < 5 then
        controller:SendMessage("inventory full!")
        return
    end

    characterClass = controller:GetCharacterClass()

    if characterClass == 0 then
        controller:AddCharacterItem(1065, 112)

        controller:AddCharacterItem(1676, 112)
        controller:AddCharacterItem(1706, 112)
        controller:AddCharacterItem(1735, 112)
        controller:AddCharacterItem(1766, 112)

    elseif characterClass == 1 then
        if controller:GetAvailableSlots() < 6 then
            controller:SendMessage("inventory full!")
            return
        end

        controller:AddCharacterItem(1306, 112)
        controller:AddCharacterItem(1031, 112)

        controller:AddCharacterItem(1796, 112)
        controller:AddCharacterItem(1826, 112)
        controller:AddCharacterItem(1855, 112)
        controller:AddCharacterItem(1886, 112)

    elseif characterClass == 2 then
        controller:AddCharacterItem(1206, 112)

        controller:AddCharacterItem(1916, 112)
        controller:AddCharacterItem(1946, 112)
        controller:AddCharacterItem(1975, 112)
        controller:AddCharacterItem(2006, 112)

    elseif characterClass == 3 then
        controller:AddCharacterItem(1171, 112)

        controller:AddCharacterItem(2036, 112)
        controller:AddCharacterItem(2066, 112)
        controller:AddCharacterItem(2095, 112)
        controller:AddCharacterItem(2126, 112)

    elseif characterClass == 4 then
        controller:AddCharacterItem(1276, 112)

        controller:AddCharacterItem(2156, 112)
        controller:AddCharacterItem(2186, 112)
        controller:AddCharacterItem(2215, 112)
        controller:AddCharacterItem(2246, 112)

    elseif characterClass == 5 then
        controller:AddCharacterItem(1241, 112)

        controller:AddCharacterItem(2276, 112)
        controller:AddCharacterItem(2306, 112)
        controller:AddCharacterItem(2335, 112)
        controller:AddCharacterItem(2366, 112)
    else 
        return
    end

    bConsumeItem = true
end