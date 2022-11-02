using Microsoft.EntityFrameworkCore;
using AikaWeb.Models;

namespace AikaWeb.Data
{
    public class EntityHandler : DbContext
    {
        public EntityHandler(DbContextOptions<EntityHandler> options)
            : base(options)
        {

        }

        public DbSet<AccountModel> Accounts { get; set; } = default!;
        public DbSet<SessionModel> Sessions { get; set; } = default!;
        public DbSet<SessionActiveModel> SessionsActive { get; set; } = default!;

        public DbSet<CharacterModel> Characters { get; set; } = default!;
        public DbSet<ItemModel> Items { get; set; } = default!;
        public DbSet<SkillModel> Skills { get; set; } = default!;
        public DbSet<HotBarModel> Hotbars { get; set; } = default!; 
        public DbSet<BuffModel> Buffs { get; set; } = default!;
        public DbSet<TitleModel> Titles { get; set; } = default!;

        public DbSet<ServerModel> Servers { get; set; } = default!;
        public DbSet<NationModel> Nations { get; set; } = default!;
    }
}
