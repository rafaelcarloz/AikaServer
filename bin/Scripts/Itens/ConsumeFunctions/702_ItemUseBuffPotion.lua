-- define TypeItem 
CONSUME_FUNCTION_ID = 702

bConsumeItem = false

-- init
function run(controller)
    controller:AddCharacterBuff()

    bConsumeItem = true
end