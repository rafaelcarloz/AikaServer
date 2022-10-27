using AikaWeb.Data;
using AikaWeb.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.VisualBasic;
using System;
using System.ComponentModel.DataAnnotations;
using System.Net;
using System.Security.Cryptography;
using static AikaWeb.Controllers.AuthController;

namespace AikaWeb.Controllers
{
    [ApiController]
    public class AuthController : ControllerBase
    {
        private readonly ILogger<AuthController> _logger;
        private readonly EntityHandler _context;

        const string RESP_CODE_ACCOUNT_NOT_FOUND = "0";
        const string RESP_CODE_ACCOUNT_INCORRECT_PW = "-1";

        public AuthController(ILogger<AuthController> logger, EntityHandler context)
        {
            _logger = logger;
            _context = context;
        }

        public class TokenRequest
        {
            [Required]
            [MaxLength(140)]
            public string? id { get; set; }
            [Required]
            public string? pw { get; set; }
        }

        [HttpPost("member/aika_get_token.asp")]
        [Consumes("application/x-www-form-urlencoded")]
        public async Task<ActionResult<string>> GetToken([FromForm] TokenRequest tokenRequest)
        {
            string username = tokenRequest.id ?? "";

            if (!_context.Accounts.Any(acc => username.Equals(acc.AccountUsername)))
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            var account = _context.Accounts.Single(e => username.Equals(e.AccountUsername));

            if (account.AccountPassword != tokenRequest.pw)
            {
                return RESP_CODE_ACCOUNT_INCORRECT_PW;
            }

            SessionModel session = this.RegisterSession(account);

            await _context.SaveChangesAsync();

            return CreateMD5(session.SessionID.ToString());
        }

        private SessionModel RegisterSession(AccountModel account)
        {
            SessionModel session = new SessionModel();

            session.AccountID = account.AccountID;
            session.CreatedByAddress = this.GetClientAddress();
            session.CreatedOn = DateTime.Now;
            session.SessionStatus = 1;
            session.UpdatedOn = DateTime.Now;

            _context.Sessions.Add(session);

            return session;
        }
        

        [HttpPost("servers/aika_get_chrcnt.asp")]
        [Consumes("application/x-www-form-urlencoded")]
        public ActionResult<string> GetCharacterCount([FromForm] TokenRequest tokenRequest)
        {
            int characterCount = 0;
            int accountNationId = 0;

            string username = tokenRequest.id ?? "";
            string tokenString = tokenRequest.pw ?? "";

            SessionActiveModel sessionActive = new SessionActiveModel();

            try
            {
                sessionActive = _context.SessionsActive.OrderByDescending(s=> s.UpdatedOn).Where(s => username.Equals(s.AccountUsername)).Take(1).First();
            } catch (Exception)
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            if (!username.Equals(sessionActive.AccountUsername))
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            if (!tokenString.Equals(sessionActive.SessionHash))
            {
                return RESP_CODE_ACCOUNT_INCORRECT_PW;
            }

            accountNationId = sessionActive.AccountNation;

            characterCount = _context.Characters.Where(c => c.AccountID == sessionActive.AccountID && c.CharacterStatus > 0).Count();

            return String.Format("CNT {0} 0 0 0<br>{1} 0 0 0", characterCount, accountNationId);
        }


        [HttpPost("servers/aika_reset_flag.asp")]
        [Consumes("application/x-www-form-urlencoded")]
        public ActionResult<string> PostResetFlag([FromForm] TokenRequest tokenRequest)
        {
            string username = tokenRequest.id ?? "";
            string tokenString = tokenRequest.pw ?? "";

            SessionActiveModel sessionActive = new SessionActiveModel();

            try
            {
                sessionActive = _context.SessionsActive.Single(s => username.Equals(tokenRequest.id));
            }
            catch (Exception)
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            if (!username.Equals(sessionActive.AccountUsername))
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            if (!tokenString.Equals(sessionActive.SessionHash))
            {
                return RESP_CODE_ACCOUNT_INCORRECT_PW;
            }

            var session = _context.Sessions.Find(sessionActive.SessionID);

            if (session == null)
            {
                return RESP_CODE_ACCOUNT_INCORRECT_PW;
            }

            session.UpdatedOn = DateTime.Now;
            _context.SaveChanges();

            return sessionActive.SessionHash;
        }


        [HttpPost("/servers/serv0{serverId}.asp")]
        public ActionResult<string> GetServersPlayers(int serverId)
        {
            return "1 0";
        }


        [HttpPost("/session/data")]
        public ActionResult<SessionActiveModel> GetSessionDataByToken([FromBody] TokenRequest tokenRequest)
        {
            string user = tokenRequest.id ?? "";
            string tokenString = tokenRequest.pw ?? "";

            SessionActiveModel sessionActive = new SessionActiveModel();

            try
            {
                sessionActive = _context.SessionsActive.OrderByDescending(s => s.UpdatedOn).Where(s => user.Equals(s.AccountUsername)).Take(1).First();
            }
            catch (Exception)
            {
                return NotFound();
            }

            if (!user.Equals(sessionActive.AccountUsername))
            {
                return NotFound();
            }

            if (!tokenString.Equals(sessionActive.SessionHash))
            {
                return Unauthorized();
            }

            var session = _context.Sessions.Find(sessionActive.SessionID);

            if (session == null)
            {
                return Unauthorized();
            }

            session.UpdatedOn = DateTime.Now;
            session.SessionStatus = 2;
            _context.SaveChanges();

            return sessionActive;
        }

        public static string CreateMD5(string input)
        {
            using (MD5 md5 = MD5.Create())
            {
                byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
                byte[] hashBytes = md5.ComputeHash(inputBytes);

                return Convert.ToHexString(hashBytes).ToLower();
            }
        }

        private string? GetClientAddress()
        {
            return HttpContext.Connection.RemoteIpAddress?.ToString();
        }
    }
}
