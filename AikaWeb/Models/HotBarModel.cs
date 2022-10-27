using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("CharacterHotBar")]
    public class HotBarModel
    {
        [Key]
        public long CharacterHotBarID { get; set; }
        public byte ItemStatus { get; set; }
        public long CharacterID { get; set; }
        public byte ItemType { get; set; }
        public byte ItemSlot { get; set; }
        public int ItemID { get; set; }
    }
}
