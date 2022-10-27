using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("Nations")]
    public class NationModel
    {
        [Key]
        public int NationID { get; set; }
        public string? NationName { get; set; }
        public int NationRank { get; set; }
        public int NationStatus { get; set; }
        public long? MarshalGuildID { get; set; }
        public int TaxCitizens { get; set; }
        public int TaxVisitors { get; set; }
        public int TaxSettlement { get; set; }
        public int? NationAlliedID { get; set; }
        public DateTime? NationAlliedDate { get; set; }
        public long NationGold { get; set; }
        public DateTime CreatedOn { get; set; }
        public DateTime? UpdatedOn { get; set; }
    }
}
