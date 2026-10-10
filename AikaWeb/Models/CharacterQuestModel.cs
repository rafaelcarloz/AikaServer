using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("CharacterQuests")]
    public class CharacterQuestModel
    {
        [Key]
        public long CharacterQuestID { get; set; }
        public int IsActive { get; set; }
		public string? QuestStatus { get; set; }
        public long CharacterID { get; set; }

        public int QuestID { get; set; }

        public int GoalCompleted_1 { get; set; }
        public int GoalCompleted_2 { get; set; }
        public int GoalCompleted_3 { get; set; }
        public int GoalCompleted_4 { get; set; }
        public int GoalCompleted_5 { get; set; }

        public DateTime? AcceptedOn { get; set; }
        public DateTime? CompletedOn { get; set; }
		public DateTime? UpdatedOn { get; set; }
        
    }
}