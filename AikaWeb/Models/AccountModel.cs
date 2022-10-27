using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("Accounts")]
    public class AccountModel
    {
        [Key]
        public long AccountID { get; set; }
        public int AccountStatus { get; set; }
        public string? AccountUsername { get; set; }
        public string? AccountPassword { get; set; }
    }
}
