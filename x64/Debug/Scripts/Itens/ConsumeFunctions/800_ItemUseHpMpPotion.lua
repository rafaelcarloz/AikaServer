-- define TypeItem 
CONSUME_FUNCTION_ID = 800

bConsumeItem = false

-- init
function run(controller)
    controller:AddCharacterHealth()
    controller:AddCharacterMana()

    bConsumeItem = true
end