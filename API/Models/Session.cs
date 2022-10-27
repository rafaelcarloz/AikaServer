using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace API.Models
{
    [Table("Sessions")]
    public class Session
    {
        [Key]
        public long SessionID { get; set; }
        public int SessionStatus { get; set; }
        public long AccountID { get; set; }
        
        public string? CreatedByAddress { get; set; }
        public DateTime CreatedOn { get; set; }
        public DateTime UpdatedOn { get; set; }
    }
}
