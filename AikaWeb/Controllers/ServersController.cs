using AikaWeb.Data;
using AikaWeb.Models;
using Microsoft.AspNetCore.Mvc;

namespace AikaWeb.Controllers
{
    [ApiController]
    public class ServersController : ControllerBase
    {
        private readonly ILogger<ServersController> _logger;
        private readonly EntityHandler _context;

        public ServersController(ILogger<ServersController> logger, EntityHandler context)
        {
            _logger = logger;
            _context = context;
        }

        [HttpGet("/servers/all")]
        public ActionResult<IEnumerable<ServerModel>> GetServers()
        {
            return _context.Servers.Where(server => server.ServerStatus > 0).ToList();
        }
    }
}
