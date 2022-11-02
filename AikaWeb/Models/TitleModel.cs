using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("CharacterTitles")]
    public class TitleModel
    {
        [Key]
        public long CharacterTitleID { get; set; }
        public int TitleStatus { get; set; }
        public long CharacterID { get; set; }
        public int TitleID { get; set; }
        public int TitleLevel { get; set; }
        public int TitleProgress { get; set; }
        public DateTime? CreatedOn { get; set; }
        public DateTime? UpdatedOn { get; set; }
    }
}
