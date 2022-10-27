using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace AikaWeb.Models
{
    [Table("Characters")]
    public class CharacterModel
    {
        [Key]
		public long CharacterID { get; set; }
        public byte CharacterStatus { get; set; }
        public long AccountID { get; set; }
        public string? CharacterName { get; set; }
        public byte CharacterSlot { get; set; }
        public string? NumericToken { get; set; }
        public byte NumericTokenError { get; set; }
        public int CharacterLastRoration { get; set; }
        public int LoggedHour { get; set; }
        public byte PlayerKillActive { get; set; }

        public int CharacterClass { get; set; }

        public int CharacterFirstLogin { get; set; }

        public int CharacterLevel { get; set; }


        public int? AddinitionalStrength { get; set; }
        public int? AddinitionalAgility { get; set; }
        public int? AddinitionalIntelligence { get; set; }
        public int? AddinitionalConstituion { get; set; }
        public int? AddinitionalLuck { get; set; }

        public int? CurrentHelthPoints { get; set; }
        public int? CurrentManaPoints { get; set; }

        public int CurrentHonorPoints { get; set; }
        public int AllHonorPoints { get; set; }

        public int CurrentKillPoints { get; set; }
        public int AllKillPoints { get; set; }

        public int InfamiaStatus { get; set; }

        public int CurrentSkillPoints { get; set; }
        public long CurrentExperience { get; set; }


        public int? CharacterGuildID { get; set; }

        public long CharacterGold { get; set; }


        public int CharacterPositionX { get; set; }
        public int CharacterPositionY { get; set; }

        public int? CharacterActiveTitleID { get; set; }
        public int? CharacterActiveAction { get; set; }


        public DateTime? ScheduledDeleteOn { get; set; }

        public DateTime CreatedOn { get; set; }
        public string? CreatedByAddress { get; set; }
        public DateTime? LoggedOn { get; set; }
        public string? LoggedByAddress { get; set; }

        public int SavedPositionX { get; set; }
        public int SavedPositionY { get; set; }
    }
}
