-- define TypeItem 
CONSUME_FUNCTION_ID = 217

bConsumeItem = false

PREMIUM_BAG_SLOT_ID = 63

-- init
function run(controller)
    controller:MoveUsedItemToSlot(PREMIUM_BAG_SLOT_ID);
end