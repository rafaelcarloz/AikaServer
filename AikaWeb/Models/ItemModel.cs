using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
	[Table("CharacterItems")]
    public class ItemModel
    {
		[Key]
		public long CharacterItemID { get; set; }
		public byte ItemStatus { get; set; }
        public long CharacterID { get; set; }
        public byte InventoryType { get; set; }
        public byte InventorySlot { get; set; }
		public int ItemListID { get; set; }
        public int ItemAppearanceID { get; set; }
        public int ItemIdentication { get; set; }
        public int ItemEffect1_ID { get; set; }
        public int ItemEffect1_Value { get; set; }
        public int ItemEffect2_ID { get; set; }
        public int ItemEffect2_Value { get; set; }
        public int ItemEffect3_ID { get; set; }
        public int ItemEffect3_Value { get; set; }
        public byte ItemDurabilityMin { get; set; }
        public byte ItemDurabilityMax { get; set; }
        public int ItemAmount { get; set; }
        public int ItemExpireTime { get; set; }
    }
}
