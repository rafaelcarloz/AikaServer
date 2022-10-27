using System.Numerics;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace API.Models
{
    [Table("Accounts")]
    public class Account
    {
        [Key]
        public long AccountID { get; set; }
        public int AccountStatus { get; set; }
		public string? AccountUsername { get; set; }
        public string? AccountPassword { get; set; }
		public string? AccountEmail { get; set; }
        public int AccountNation { get; set; }
        public int AccountType { get; set; }
        public long StorageGold { get; set; }
        public int AccountCash { get; set; }
        public int AccountCashBonus { get; set; }

        public string? CreatedByAddress { get; set; }
        public DateTime CreatedOn { get; set; }
        public string? UpdateByAddress { get; set; }
        public DateTime UpdateOn { get; set; }
    }
}
