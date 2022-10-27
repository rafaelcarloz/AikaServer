using Microsoft.AspNetCore.Mvc;
using System;
using Microsoft.EntityFrameworkCore;
using System.Security.Cryptography;

using API.Models;

namespace API.Controllers
{
    [ApiController]
    public class AccountController : ControllerBase
    {
        private readonly AccountContext _context;

        const string RESP_CODE_ACCOUNT_NOT_FOUND = "0";
        const string RESP_CODE_ACCOUNT_INCORRECT_PW = "-1";

        public AccountController(AccountContext context)
        {
            _context = context;
        }

        [HttpGet("acccount/token")]
        public async Task<ActionResult<string>> GetToken(string id, string pw)
        {
            if (!_context.Accounts.Any(acc => id.Equals(acc.AccountUsername)))
            {
                return RESP_CODE_ACCOUNT_NOT_FOUND;
            }

            var account = _context.Accounts.Single(e => id.Equals(e.AccountUsername));

            if (account.AccountPassword != pw)
            {
                return RESP_CODE_ACCOUNT_INCORRECT_PW;
            }

            Session session = new Session();

            session.AccountID = account.AccountID;
            session.CreatedByAddress = this.GetClientAddress();
            session.CreatedOn = DateTime.Now;
            session.SessionStatus = 1;

            _context.Sessions.Add(session);
            await _context.SaveChangesAsync();

            return CreateMD5(session.SessionID.ToString());
        }

        public static string CreateMD5(string input)
        {
            using (MD5 md5 = MD5.Create())
            {
                byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);
                byte[] hashBytes = md5.ComputeHash(inputBytes);

                return Convert.ToHexString(hashBytes);
            }
        }


        private string? GetClientAddress()
        {
           return HttpContext.Connection.RemoteIpAddress?.ToString();
        }
    }
}
