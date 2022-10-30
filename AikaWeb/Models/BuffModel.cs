using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("CharacterBuffs")]
    public class BuffModel
    {
        [Key]
        public long CharacterBuffID { get; set; }
        public int BuffStatus { get; set; }
        public long CharacterID { get; set; }
        public int SkillListID { get; set; }
        public DateTime? BuffDuration { get; set; }
    }
}
