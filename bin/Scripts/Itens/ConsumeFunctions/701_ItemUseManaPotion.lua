-- define TypeItem 
CONSUME_FUNCTION_ID = 701

bConsumeItem = false

-- init
function run(controller)
    controller:AddCharacterMana()

    bConsumeItem = true
end