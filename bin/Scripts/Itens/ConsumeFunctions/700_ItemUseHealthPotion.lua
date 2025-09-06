-- define TypeItem 
CONSUME_FUNCTION_ID = 700

bConsumeItem = false

-- init
function run(controller)
    controller:AddCharacterHealth()

    bConsumeItem = true
end