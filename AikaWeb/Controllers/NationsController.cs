using AikaWeb.Data;
using AikaWeb.Models;
using Microsoft.AspNetCore.Mvc;
using static AikaWeb.Controllers.AuthController;

namespace AikaWeb.Controllers
{
    [ApiController]
    public class NationsController : ControllerBase
    {
        private readonly ILogger<NationsController> _logger;
        private readonly EntityHandler _context;

        public NationsController(ILogger<NationsController> logger, EntityHandler context)
        {
            _logger = logger;
            _context = context;
        }

        [HttpGet("/nations/all")]
        public ActionResult<IEnumerable<NationModel>> GetNations()
        {
            return _context.Nations.Where(nation => nation.NationStatus > 0).ToList();
        }

        [HttpGet("/nations/{id}")]
        public ActionResult<NationModel> GetNation(int? id)
        {
            NationModel? nation;

            if (id == null)
            {
                return BadRequest();
            }

            try
            {
                nation = _context.Nations.Find(id);
            } catch (Exception exception)
            {
                Console.WriteLine(exception.Message);
                return NotFound();
            }

            if (nation == null)
            {
                return NoContent();
            }

            return nation;
        }
    }
}
