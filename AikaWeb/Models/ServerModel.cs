using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace AikaWeb.Models
{
    [Table("Servers")]
    public class ServerModel
    {
        [Key]
        public int ServerID { get; set; }
        public int ServerType { get; set; }
        public int ServerStatus { get; set; }
        public int NationID { get; set; }
        public string? ServerAddress { get; set; }
        public int ServerPort { get; set; }

        public DateTime CreatedOn { get; set; }
        public DateTime? UpdatedOn { get; set; }
    }
}
