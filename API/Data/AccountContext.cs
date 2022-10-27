using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using API.Models;

public class AccountContext : DbContext
{
    public AccountContext (DbContextOptions<AccountContext> options)
       : base(options)
    {
    }

    public DbSet<API.Models.Account> Accounts { get; set; } = default!;
    public DbSet<API.Models.Session> Sessions { get; set; } = default!;
}
