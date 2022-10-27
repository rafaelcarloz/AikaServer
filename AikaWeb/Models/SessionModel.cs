using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using Microsoft.EntityFrameworkCore;

namespace AikaWeb.Models
{
    [Table("Sessions")]
    public class SessionModel
    {
        [Key]
        public long SessionID { get; set; }
        public int SessionStatus { get; set; }
        public long AccountID { get; set; }
        public string? CreatedByAddress { get; set; }
        public DateTime CreatedOn { get; set; }
        public DateTime UpdatedOn { get; set; }
    }


    [Table("vwSessionsActive")]
    [Keyless]
    public class SessionActiveModel
    {
        public long SessionID { get; set; }
        public int SessionStatus { get; set; }
        public long AccountID { get; set; }
        public int AccountNation { get; set; }
        public string? AccountUsername { get; set; }
        public DateTime CreatedOn { get; set; }
        public string? CreatedByAddress { get; set; }
        public DateTime UpdatedOn { get; set; }
        public string? SessionHash { get; set; }
    }
}
