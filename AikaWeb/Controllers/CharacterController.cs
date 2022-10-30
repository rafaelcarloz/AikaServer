using AikaWeb.Data;
using AikaWeb.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient.Server;
using Microsoft.EntityFrameworkCore;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.Json;

namespace AikaWeb.Controllers
{
    namespace AikaWeb.Controllers
    {
        [ApiController]
        public class CharacterController : ControllerBase
        {
            private readonly ILogger<CharacterController> _logger;
            private readonly EntityHandler _context;

            public CharacterController(ILogger<CharacterController> logger, EntityHandler context)
            {
                _logger = logger;
                _context = context;
            }

            #region "Character"

            [HttpGet("/characters/{accountId}/all")]
            public ActionResult<IEnumerable<CharacterModel>> GetCharactersAll(int accountId)
            {
                return _context.Characters.Where(character => (character.AccountID == accountId) && (character.CharacterStatus > 0)).OrderBy(c => c.CharacterSlot).ToList();
            }

            [HttpGet("/characters/{characterId}")]
            public ActionResult<CharacterModel> GetCharacter(int characterId)
            {
                try
                {
                    return _context.Characters.Where(character => (character.CharacterID == characterId) && (character.CharacterStatus > 0)).First();
                } catch (Exception e)
                {
                    return NotFound();
                }
            }


            public class CharacterCreateResponse_Data
            {
                public long characterID { get; set; }
            }
            public class CharacterCreateResponse{
                public bool success { get; set; }
                public CharacterCreateResponse_Data? data { get; set; }
                public string? message { get; set; }
            }

            [HttpPost("/characters/add")]
            public ActionResult<CharacterCreateResponse> PostCreateCharacter([FromBody]CharacterModel character)
            {
                character.CharacterStatus = 1;
                character.CreatedOn = DateTime.Now;
                character.CreatedByAddress = HttpContext.Connection.RemoteIpAddress?.ToString();

                if (character.CharacterName == null || character.CharacterName == "")
                {
                    return new CharacterCreateResponse() { success = false, message = "nome de personagem invalido" };
                }

                if (_context.Characters.Any(c => character.CharacterName.Equals(c.CharacterName)  && c.CharacterStatus > 0))
                {
                    return new CharacterCreateResponse() { success = false, message = "nome de personagem em uso" };
                }

                if (_context.Characters.Any(c => c.AccountID == character.AccountID && c.CharacterSlot == character.CharacterSlot && c.CharacterStatus > 0))
                {
                    return new CharacterCreateResponse() { success = false, message = "slot ja utilizado" };
                }

                _context.Characters.Add(character);
                _context.SaveChanges();

                return new CharacterCreateResponse() { 
                    success = true, 
                    data = new CharacterCreateResponse_Data(){ characterID = character .CharacterID}, 
                    message = "" 
                };
            }

            [HttpPost("/characters/{characterId}")]
            public ActionResult<CharacterCreateResponse> PostCreateCharacter(long characterId, [FromBody] CharacterModel character)
            {
                if (characterId == 0)
                {
                    return NotFound();
                }

                var currentCharacter = _context.Characters.Find(characterId);

                if (currentCharacter == null)
                {
                    return NotFound();
                }

                character.AccountID = currentCharacter.AccountID;
                character.CharacterStatus = currentCharacter.CharacterStatus;
                character.CharacterName = currentCharacter.CharacterName;
                character.CharacterSlot = currentCharacter.CharacterSlot;
                character.NumericToken = currentCharacter.NumericToken;

                character.CreatedOn = currentCharacter.CreatedOn;
                character.CreatedByAddress = currentCharacter.CreatedByAddress;

                _context.Entry(currentCharacter).State = EntityState.Detached;

                _context.Entry(character).State = EntityState.Modified;
                _context.SaveChanges();

                return new CharacterCreateResponse(){ success = true };
            }

            public class DeletionData
            {   
                public long CharacterID { get; set; }
                public int DeleteStatus { get; set; }
                public DateTime? DaleteTime { get; set; }
            }

            [HttpPost("/characters/{characterID}/deletion")]
            public ActionResult<CharacterCreateResponse> PostDeletionCharacter([FromBody] DeletionData data)
            {
                var character = _context.Characters.Find(data.CharacterID);

                if (character == null)
                {
                    return NotFound();
                }

                switch (data.DeleteStatus)
                {
                    case 0:
                        {
                            character.ScheduledDeleteOn = null;
                            break;
                        }
                    case 1:
                        {
                            character.ScheduledDeleteOn = data.DaleteTime;
                            break;
                        }
                    case 2:
                        {
                            character.CharacterStatus = 0;
                            break;
                        }
                }

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            public class TokenData
            {
                public long CharacterID { get; set; }
                public string? NumericToken { get; set; }
                public byte NumericErrors { get; set; }
            }

            [HttpPost("/characters/{characterID}/token")]
            public ActionResult<CharacterCreateResponse> PostTokenCharacter([FromBody] TokenData data)
            {
                var character = _context.Characters.Find(data.CharacterID);

                if (character == null)
                {
                    return NotFound();
                }

                if (data.NumericToken != null)
                {
                    character.NumericToken = data.NumericToken;
                }

                character.NumericTokenError = data.NumericErrors;

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            #endregion
            #region "Items"

            [HttpGet("/characters/{characterID}/items")]
            public ActionResult<IEnumerable<ItemModel>> GetCharacterItemsAll(long characterID)
            {
                return _context.Items.Where(item => (item.CharacterID == characterID) && (item.ItemStatus > 0)).OrderBy(item => item.InventorySlot).ToList();
            }

            [HttpGet("/characters/{characterID}/items/{inventoryType}")]
            public ActionResult<IEnumerable<ItemModel>> GetCharacterItemsByInventoryType(long characterID, int inventoryType)
            {
                return _context.Items.Where(item => (item.CharacterID == characterID) && (item.ItemStatus > 0) && item.InventoryType == inventoryType).OrderBy(item => item.InventorySlot).ToList();
            }

            [HttpPost("/characters/{characterID}/items")]
            public ActionResult<CharacterCreateResponse> PostCharacterItemsAll(long characterID, [FromBody]IEnumerable<ItemModel> items)
            {
                foreach (var item in items)
                {
                    _context.Entry(item).State = (item.CharacterItemID == 0) ? EntityState.Added : EntityState.Modified;
                }

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            #endregion
            #region "Skills"

            [HttpGet("/characters/{characterID}/skills")]
            public ActionResult<IEnumerable<SkillModel>> GetCharacterSkillsAll(long characterID)
            {
                return _context.Skills.Where(item => (item.CharacterID == characterID) && (item.SkillStatus > 0)).OrderBy(item => item.SkillSlot).ToList();
            }

            [HttpPost("/characters/{characterID}/skills")]
            public ActionResult<CharacterCreateResponse> PostCharacterSkillsAll(long characterID, [FromBody] IEnumerable<SkillModel> items)
            {
                foreach (var skill in items)
                {
                    _context.Entry(skill).State = (skill.CharacterSkillID == 0) ? EntityState.Added : EntityState.Modified;
                }

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            #endregion
            #region "HotBar"

            [HttpGet("/characters/{characterID}/hotbar")]
            public ActionResult<IEnumerable<HotBarModel>> GetCharacterHotBarAll(long characterID)
            {
                return _context.Hotbars.Where(item => (item.CharacterID == characterID) && (item.ItemStatus > 0)).OrderBy(item => item.ItemSlot).ToList();
            }

            [HttpPost("/characters/{characterID}/hotbar")]
            public ActionResult<CharacterCreateResponse> PostCharacterHotBarAll(long characterID, [FromBody] IEnumerable<HotBarModel> items)
            {
                foreach (var item in items)
                {
                    _context.Entry(item).State = (item.CharacterHotBarID == 0) ? EntityState.Added : EntityState.Modified;
                }

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            #endregion
            #region "Buffs"

            [HttpGet("/characters/{characterID}/buffs")]
            public ActionResult<IEnumerable<BuffModel>> GetCharacterBuffs(long characterID)
            {
                return _context.Buffs.Where(item => (item.CharacterID == characterID) && (item.BuffStatus > 0)).OrderByDescending(item => item.BuffDuration).ToList();
            }

            [HttpPost("/characters/{characterID}/buffs")]
            public ActionResult<CharacterCreateResponse> PostCharacterBuffs(long characterID, [FromBody] IEnumerable<BuffModel> items)
            {
                foreach (var buff in items)
                {
                    _context.Entry(buff).State = (buff.CharacterBuffID == 0) ? EntityState.Added : EntityState.Modified;
                }

                _context.SaveChanges();

                return new CharacterCreateResponse() { success = true };
            }

            #endregion
        }
    }
}
