-- define TypeItem 
CONSUME_FUNCTION_ID = 208

bConsumeItem = false

-- init
function run(controller)
    if controller:GetCharacterNation() > 0 then
        if controller:GetCharacterNation() ~= controller:GetServerNation() then
            controller:SendMessage("can be used only in the nation of origin")
            return
        end
    end

    controller:TeleportScroll();

    bConsumeItem = true
end