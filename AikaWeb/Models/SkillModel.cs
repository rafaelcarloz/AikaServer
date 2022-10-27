using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("CharacterSkills")]
    public class SkillModel
    {
        [Key]
        public long CharacterSkillID { get; set; }
        public byte SkillStatus { get; set; }
        public long CharacterID { get; set; }
        public byte SkillType { get; set; }
        public byte SkillSlot { get; set; }
        public int SkillListID { get; set; }
    }
}
