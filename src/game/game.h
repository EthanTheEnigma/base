#ifndef CPP_GAME_HEADER
#define CPP_GAME_HEADER

#include "engine.h"

#define VERSION_GAMEID "fps"
#define VERSION_GAME 268
#define VERSION_DEMOMAGIC "RED_ECLIPSE_DEMO"

#define MAXAI 256
#define MAXPLAYERS (MAXCLIENTS+MAXAI*2)
#define MAXPARAMS 256

// network quantization scale
#define DMF 16.0f // for world locations
#define DNF 1000.0f // for normalized vectors
#define DVELF 1.0f // for playerspeed based velocity vectors

#define FOOTSTEP_DIST 1.5f // Threshold, below which a footstep will be registered

enum
{
    S_JUMP = S_GAMESPECIFIC, S_IMPULSE, S_LAND, S_FOOTSTEP_L, S_FOOTSTEP_R, S_SWIMSTEP, S_PAIN, S_DEATH,
    S_SPLASH1, S_SPLASH2, S_SPLOSH, S_DEBRIS, S_BURNLAVA,
    S_EXTINGUISH, S_SHELL, S_ITEMUSE, S_ITEMSPAWN,
    S_REGEN_BEGIN, S_REGEN, S_CRITICAL, S_DAMAGE, S_DAMAGE2, S_DAMAGE3, S_DAMAGE4, S_DAMAGE5, S_DAMAGE6, S_DAMAGE7, S_DAMAGE8,
    S_BURNED, S_BLEED, S_SHOCK, S_RESPAWN, S_CHAT, S_ERROR, S_ALARM, S_CATCH, S_DROP, S_BOUNCE,
    S_V_FLAGSECURED, S_V_FLAGOVERTHROWN, S_V_FLAGPICKUP, S_V_FLAGDROP, S_V_FLAGRETURN, S_V_FLAGSCORE, S_V_FLAGRESET,
    S_V_BOMBSTART, S_V_BOMBDUEL, S_V_BOMBPICKUP, S_V_BOMBDROP, S_V_BOMBSCORE, S_V_BOMBRESET,
    S_V_NOTIFY, S_V_FIGHT, S_V_SCORE, S_V_START, S_V_CHECKPOINT, S_V_COMPLETE, S_V_OVERTIME, S_V_ONEMINUTE, S_V_HEADSHOT,
    S_V_SPREE, S_V_SPREE2, S_V_SPREE3, S_V_SPREE4, S_V_MULTI, S_V_MULTI2, S_V_MULTI3,
    S_V_REVENGE, S_V_DOMINATE, S_V_FIRSTBLOOD, S_V_BREAKER,
    S_V_YOUWIN, S_V_YOULOSE, S_V_DRAW, S_V_FRAGGED, S_V_BALWARN, S_V_BALALERT,
    S_GAME
};

enum                                // entity types
{
    NOTUSED = ET_EMPTY, LIGHT = ET_LIGHT, MAPMODEL = ET_MAPMODEL, PLAYERSTART = ET_PLAYERSTART, ENVMAP = ET_ENVMAP, PARTICLES = ET_PARTICLES,
    MAPSOUND = ET_SOUND, LIGHTFX = ET_LIGHTFX, DECAL = ET_DECAL, WIND = ET_WIND, MAPUI = ET_MAPUI, SOUNDENV = ET_SOUNDENV, WEAPON = ET_GAMESPECIFIC,
    TELEPORT, ACTOR, TRIGGER, PUSHER, AFFINITY, CHECKPOINT,
    ROUTE, RAIL, CAMERA,
    MAXENTTYPES
};

enum { EU_NONE = 0, EU_ITEM, EU_AUTO, EU_ACT, EU_MAX };

enum { TR_TOGGLE = 0, TR_LINK, TR_SCRIPT, TR_ONCE, TR_EXIT, TR_MAX };
enum { TA_MANUAL = 0, TA_AUTO, TA_ACTION, TA_MAX };
enum { RAIL_YAW = 0, RAIL_PITCH, RAIL_SEEK, RAIL_SPLINE, RAIL_SPEED, RAIL_PREV, RAIL_NEXT, RAIL_MAX, RAIL_ALL = (1<<RAIL_YAW)|(1<<RAIL_PITCH)|(1<<RAIL_SEEK)|(1<<RAIL_SPLINE)|(1<<RAIL_SPEED)|(1<<RAIL_PREV)|(1<<RAIL_NEXT) };
enum { CAMERA_NORMAL = 0, CAMERA_MAPSHOT, CAMERA_MAX };
enum { CAMERA_F_STATIC = 0, CAMERA_F_MAX, CAMERA_F_ALL = (1<<CAMERA_F_STATIC)};

#define TRIGGERIDS      16
#define TRIGSTATE(a,b)  (b%2 ? !a : a)
#define TRIGGERTIME     1000
#define TRIGGERDELAY    250

enum { CP_RESPAWN = 0, CP_START, CP_FINISH, CP_LAST, CP_MAX, CP_ALL = (1<<CP_RESPAWN)|(1<<CP_START)|(1<<CP_FINISH)|(1<<CP_LAST) };

enum { TELE_NOAFFIN = 0, TELE_MAX };
enum { MDLF_HIDE = 0, MDLF_NOCLIP, MDLF_NOSHADOW, MDLF_MAX };

struct enttypes
{
    int type,           priority, links,    radius, usetype,    numattrs,   palattr,    modesattr,  idattr, mvattr, fxattr,
            canlink, reclink, canuse;
    bool    noisy,  syncs,  resyncs,    syncpos,    synckin;
    const char *name,           *displayname,       *attrs[MAXENTATTRS];
};
#ifdef CPP_GAME_SERVER
extern const enttypes enttype[] = {
    {
        NOTUSED,        -1,         0,      0,      EU_NONE,    0,          -1,         -1,         -1,     -1,     -1,
            0, 0, 0,
            true,   false,  false,      false,      false,
                "none",         "NONE",             { "" }
    },
    {
        LIGHT,          1,          59,     0,      EU_NONE,    13,         7,          9,          -1,     11,      12,
            (1<<LIGHTFX), (1<<LIGHTFX), 0,
            false,  false,  false,      false,      false,
                "light",        "Light",            { "radius", "red", "green", "blue", "flare", "fscale", "flags", "palette", "palindex", "modes", "muts", "variant", "fxlevel"  }
    },
    {
        MAPMODEL,       1,          58,     0,      EU_NONE,    22,         8,          13,         -1,     15,     16,
            (1<<TRIGGER)|(1<<RAIL), (1<<TRIGGER), 0,
            false,  false,  false,      false,      false,
                "mapmodel",     "Map Model",        { "type", "yaw", "pitch", "roll", "blend", "scale", "flags", "colour", "palette", "palindex", "spinyaw", "spinpitch", "spinroll", "modes", "muts", "variant", "fxlevel", "lodoff", "anim", "aspeed", "aoffset", "shadowdist" }
    },
    {
        PLAYERSTART,    1,          59,     0,      EU_NONE,    7,          -1,         3,          5,      6,      -1,
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            0,
            false,  true,  false,      false,      false,
                "playerstart",  "Player Start",     { "team", "yaw", "pitch", "modes", "muts", "id", "variant" }
    },
    {
        ENVMAP,         1,          260,    0,      EU_NONE,    3,          -1,         -1,         -1,     -1,     -1,
            (1<<TELEPORT), (1<<TELEPORT), 0,
            false,  false,  false,      false,      false,
                "envmap",       "Envrionment Map",  { "radius", "size", "blur" }
    },
    {
        PARTICLES,      1,          59,     0,      EU_NONE,    19,         -1,         13,         -1,     15,     16,
            (1<<TELEPORT)|(1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT)|(1<<RAIL),
            (1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT),
            0,
            false,  false,  false,      false,      false,
                "particles",    "Particles",        { "type", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "millis", "modes", "muts", "variant", "fxlevel", "hintcolour", "hintblend" }
    },
    {
        MAPSOUND,       1,          58,     0,      EU_NONE,    12,         -1,         7,          -1,     9,      -1,
            (1<<TELEPORT)|(1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT)|(1<<WIND)|(1<<RAIL),
            (1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT)|(1<<WIND),
            0,
            false,  false,  false,      false,      false,
                "sound",        "Sound",            { "type", "gain", "pitch", "rolloff", "refdist", "maxdist", "flags", "modes", "muts", "variant", "offset", "groupid" }
    },
    {
        LIGHTFX,        1,          1,      0,      EU_NONE,    9,          -1,         5,          -1,     7,      8,
            (1<<LIGHT)|(1<<TELEPORT)|(1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT)|(1<<RAIL),
            (1<<LIGHT)|(1<<TRIGGER)|(1<<PUSHER)|(1<<PLAYERSTART)|(1<<CHECKPOINT),
            0,
            false,  false,  false,      false,      false,
                "lightfx",      "Light FX",         { "type", "mod", "min", "max", "flags", "modes", "muts", "variant", "fxlevel" }
    },
    {
        DECAL,          1,          0,      0,      EU_NONE,    13,         7,          9,          -1,     11,      12,
            0, 0, 0,
            false,  false,  false,      false,      false,
                "decal",        "Decal",            { "type", "yaw", "pitch", "roll", "scale", "blend", "colour", "palette", "palindex", "modes", "muts", "variant", "fxlevel" }
    },
    {
        WIND,           1,          239,    0,      EU_NONE,    11,          -1,         7,         -1,     9,      10,
            (1<<MAPSOUND),
            (1<<MAPSOUND),
            0,
            false,  false,  false,      false,      false,
                "wind",        "Wind",              { "mode", "yaw", "speed", "radius", "atten", "interval", "implen", "modes", "muts", "variant", "fxlevel" }
    },
    {
        MAPUI,          1,          241,    0,      EU_NONE,    14,          -1,         10,          -1,     12,      13,
            0, 0,
            0,
            false,  false,  false,      false,      false,
                "mapui",       "Map UI",            { "type", "flags", "yaw", "pitch", "radius", "scale", "yawdet", "pitchdet", "colour" "blend", "modes", "muts", "variant", "fxlevel" }
    },
    {
        SOUNDENV,        1,          0,     0,      EU_NONE,    8,           -1,          -1,        -1,     -1,      -1,
            0, 0, 0,
            false,  false,  false,      false,      false,
                "soundenv",     "Sound Env",        { "type", "width", "length", "height", "fadevals1", "fadevals2", "fadevals3", "fadevals4" }
    },
    {
        WEAPON,         2,          59,     16,     EU_ITEM,    6,          -1,         2,          4,      5,      -1,
            0, 0,
            (1<<ENT_PLAYER)|(1<<ENT_AI),
            false,  true,   true,      false,      false,
                "weapon",       "Weapon",           { "type", "flags", "modes", "muts", "id", "variant" }
    },
    {
        TELEPORT,       1,          50,     16,     EU_AUTO,    17,         6,          9,          -1,     11,      -1,
            (1<<ENVMAP)|(1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX)|(1<<TELEPORT),
            (1<<ENVMAP)|(1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<ENT_PLAYER)|(1<<ENT_AI)|(1<<ENT_PROJ),
            false,  false,  false,      false,      false,
                "teleport",     "Teleport",         { "yaw", "pitch", "push", "radius", "colour", "type", "palette", "palindex", "flags", "modes", "muts", "variant", "blend", "size", "envblend", "hintcolour", "hintblend" }
    },
    {
        ACTOR,          1,          59,     0,      EU_NONE,    11,         -1,         3,          5,      10,     -1,
            0, 0, 0,
            false,  true,   false,      true,       false,
                "actor",        "Actor",            { "type", "yaw", "pitch", "modes", "muts", "id", "weap", "health", "speed", "scale", "variant" }
    },
    {
        TRIGGER,        1,          58,     16,     EU_AUTO,    8,          -1,         5,          -1,     7,      -1,
            (1<<MAPMODEL)|(1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<MAPMODEL)|(1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<ENT_PLAYER)|(1<<ENT_AI),
            false,  true,   true,       false,      true,
                "trigger",      "Trigger",          { "id", "type", "action", "radius", "state", "modes", "muts", "variant" }
    },
    {
        PUSHER,         1,          58,     16,     EU_AUTO,    11,         -1,         6,          -1,     9,      -1,
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX)|(1<<PUSHER),
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<ENT_PLAYER)|(1<<ENT_AI)|(1<<ENT_PROJ),
            false,  false,  false,      false,      false,
                "pusher",       "Pusher",           { "yaw", "pitch", "force", "maxrad", "minrad", "type", "modes", "muts", "id", "variant", "sdelay" }
    },
    {
        AFFINITY,       1,          48,     32,     EU_NONE,    7,          -1,         3,          5,      6,      -1,
            0, 0, 0,
            false,  false,  false,      false,      false,
                "affinity",     "Affinity",         { "team", "yaw", "pitch", "modes", "muts", "id", "variant" }
    },
    {
        CHECKPOINT,     1,          48,     16,     EU_AUTO,    8,          -1,         3,          5,      7,      -1,
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<MAPSOUND)|(1<<PARTICLES)|(1<<LIGHTFX),
            (1<<ENT_PLAYER)|(1<<ENT_AI),
            false,  true,   false,      true,       false,
                "checkpoint",   "Check Point",      { "radius", "yaw", "pitch", "modes", "muts", "id", "type", "variant" }
    },
    {
        ROUTE,          1,          224,    16,     EU_NONE,    6,          -1,         -1,         -1,     -1,     -1,
            (1<<ROUTE), 0, 0,
            false,   false,  false,      false,      false,
                "route",         "Route",           { "num", "yaw", "pitch", "move", "strafe", "action" }
    },
    {
        RAIL,           -1,         228,    0,      EU_NONE,    13,         -1,         10,         -1,     12,     -1,
            (1<<LIGHT)|(1<<MAPMODEL)|(1<<PLAYERSTART)|(1<<PARTICLES)|(1<<MAPSOUND)|(1<<MAPUI)|(1<<LIGHTFX)|(1<<WEAPON)|(1<<TELEPORT)|(1<<ACTOR)|(1<<TRIGGER)|(1<<PUSHER)|(1<<RAIL)|(1<<CAMERA), 0, 0,
            false,   false,  false,      false,      false,
                "rail",         "Rail",             { "time", "flags", "yaw", "pitch", "rotlen", "rotwait", "collide", "anim", "aspeed", "aoffset", "modes", "muts", "variant" }
    },
    {
        CAMERA,         -1,         252,    0,      EU_NONE,    12,         -1,         7,          9,      10,      -1,
            0, 0, 0,
            false,   false,  false,      false,      false,
                "camera",       "Camera",           { "type", "flags", "yaw", "pitch", "maxdist", "mindist", "delay", "modes", "muts", "id", "variant", "fov" }
    }
};
#else
extern const enttypes enttype[];
namespace entities
{
    extern vector<extentity *> ents;
    extern void removepassenger(physent *d);
}
#endif
#define isent(a) (a >= NOTUSED && a < MAXENTTYPES)

#define MAXNAMELEN 24
#define SAY_ENUM(pr, en) en(pr, Message, MESSAGE, 1<<0) en(pr, Action, ACTION, 1<<1) en(pr, Team, TEAM, 1<<2) en(pr, Whisper, WHISPER, 1<<3)
ENUMNI(SAY, SAY_ENUM);

enum
{
    PRIV_NONE = 0, PRIV_PLAYER, PRIV_SUPPORTER, PRIV_MODERATOR, PRIV_ADMINISTRATOR, PRIV_DEVELOPER, PRIV_CREATOR, PRIV_MAX,
    PRIV_START = PRIV_PLAYER, PRIV_ELEVATED = PRIV_MODERATOR, PRIV_LAST = PRIV_CREATOR, PRIV_TYPE = 0xFF, PRIV_LOCAL = 1<<8
};

#define MM_MODE 0xF
#define MM_AUTOAPPROVE 0x1000
#define MM_FREESERV (MM_AUTOAPPROVE|MM_MODE)
#define MM_VETOSERV ((1<<MM_OPEN)|(1<<MM_VETO))
#define MM_COOPSERV (MM_AUTOAPPROVE|MM_VETOSERV|(1<<MM_LOCKED))
#define MM_OPENSERV (MM_AUTOAPPROVE|(1<<MM_OPEN))

enum { MM_OPEN = 0, MM_VETO, MM_LOCKED, MM_PRIVATE, MM_PASSWORD };
enum { SINFO_NONE = 0, SINFO_STATUS, SINFO_NAME, SINFO_PORT, SINFO_QPORT, SINFO_DESC, SINFO_MODE, SINFO_MUTS, SINFO_MAP, SINFO_TIME, SINFO_NUMPLRS, SINFO_MAXPLRS, SINFO_PING, SINFO_PRIO, SINFO_MAX };
enum { SSTAT_OPEN = 0, SSTAT_LOCKED, SSTAT_PRIVATE, SSTAT_FULL, SSTAT_UNKNOWN, SSTAT_MAX };

enum
{
    AC_PRIMARY = 0, AC_SECONDARY, AC_RELOAD, AC_USE, AC_JUMP, AC_WALK, AC_CROUCH, AC_SPECIAL, AC_DROP, AC_AFFINITY, AC_DASH, AC_MAX,
    AC_ALL = (1<<AC_PRIMARY)|(1<<AC_SECONDARY)|(1<<AC_RELOAD)|(1<<AC_USE)|(1<<AC_JUMP)|(1<<AC_WALK)|(1<<AC_CROUCH)|(1<<AC_SPECIAL)|(1<<AC_DROP)|(1<<AC_AFFINITY)
};

enum { IM_METER = 0, IM_COUNT, IM_TYPE, IM_REGEN, IM_SLIP, IM_FLING, IM_PUSHER, IM_COLLECT_METER, IM_LASTCOL_METER, IM_COLLECT_COUNT, IM_LASTCOL_COUNT, IM_MAX };

enum
{
    IM_T_JUMP = 0, IM_T_BOOST, IM_T_DASH, IM_T_SLIDE, IM_T_LAUNCH, IM_T_MELEE, IM_T_KICK, IM_T_GRAB, IM_T_PARKOUR, IM_T_VAULT, IM_T_POUND, IM_T_AFTER, IM_T_PUSHER, IM_T_MAX,
    IM_T_ALL    = (1<<IM_T_JUMP)|(1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH)|(1<<IM_T_MELEE)|(1<<IM_T_KICK)|(1<<IM_T_GRAB)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT)|(1<<IM_T_POUND)|(1<<IM_T_AFTER)|(1<<IM_T_PUSHER),
    IM_T_ACTION = (1<<IM_T_JUMP)|(1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH)|(1<<IM_T_MELEE)|(1<<IM_T_KICK)|(1<<IM_T_GRAB)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT)|(1<<IM_T_POUND),
    IM_T_COUNT  = (1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT)|(1<<IM_T_POUND),
    IM_T_CHECK  = (1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT)|(1<<IM_T_POUND),
    IM_T_TOUCH  = (1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH), IM_T_NOTOUCH = (1<<IM_T_BOOST), IM_T_RELAX  = (1<<IM_T_VAULT),
    IM_T_MVAI   = (1<<IM_T_JUMP)|(1<<IM_T_BOOST)|(1<<IM_T_DASH)|(1<<IM_T_SLIDE)|(1<<IM_T_LAUNCH)|(1<<IM_T_MELEE)|(1<<IM_T_KICK)|(1<<IM_T_GRAB)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT)|(1<<IM_T_POUND)|(1<<IM_T_AFTER)|(1<<IM_T_PUSHER),
    IM_T_LSAI   = (1<<IM_T_JUMP), IM_T_ROLLER = (1<<IM_T_JUMP)|(1<<IM_T_PARKOUR)|(1<<IM_T_VAULT),
};

enum
{
    SPHY_JUMP = 0, SPHY_BOOST, SPHY_DASH, SPHY_SLIDE, SPHY_LAUNCH, SPHY_MELEE, SPHY_KICK, SPHY_GRAB, SPHY_PARKOUR, SPHY_VAULT, SPHY_POUND, SPHY_AFTER, SPHY_FLING, SPHY_MATERIAL,
    SPHY_SERVER, SPHY_EXTINGUISH = SPHY_SERVER, SPHY_BUFF,
    SPHY_MAX
};

#define CROUCHLOW 0.7f
#define CROUCHHIGH 0.9f
#define GUARDRADIUS 3.f
#define PHYSMILLIS 250
#define DEATHMILLIS 300

enum
{
    ANIM_PAIN = ANIM_GAMESPECIFIC,
    ANIM_JUMP_FORWARD, ANIM_JUMP_BACKWARD, ANIM_JUMP_LEFT, ANIM_JUMP_RIGHT, ANIM_JUMP,
    ANIM_RUN_FORWARD, ANIM_RUN_BACKWARD, ANIM_RUN_LEFT, ANIM_RUN_RIGHT,
    ANIM_BOOST_FORWARD, ANIM_BOOST_BACKWARD, ANIM_BOOST_LEFT, ANIM_BOOST_RIGHT, ANIM_BOOST_UP,
    ANIM_PARKOUR_LEFT, ANIM_PARKOUR_RIGHT, ANIM_PARKOUR_UP, ANIM_PARKOUR_JUMP,
    ANIM_VAULT, ANIM_POWERSLIDE, ANIM_FLYKICK,
    ANIM_SINK, ANIM_EDIT, ANIM_WIN, ANIM_LOSE,
    ANIM_CROUCH, ANIM_CRAWL_FORWARD, ANIM_CRAWL_BACKWARD, ANIM_CRAWL_LEFT, ANIM_CRAWL_RIGHT,
    ANIM_CROUCH_JUMP_FORWARD, ANIM_CROUCH_JUMP_BACKWARD, ANIM_CROUCH_JUMP_LEFT, ANIM_CROUCH_JUMP_RIGHT, ANIM_CROUCH_JUMP,
    ANIM_CLAW, ANIM_CLAW_PRIMARY, ANIM_CLAW_SECONDARY, ANIM_CLAW_RELOAD, ANIM_CLAW_POWER, ANIM_CLAW_ZOOM,
    ANIM_PISTOL, ANIM_PISTOL_PRIMARY, ANIM_PISTOL_SECONDARY, ANIM_PISTOL_RELOAD, ANIM_PISTOL_POWER, ANIM_PISTOL_ZOOM,
    ANIM_SWORD, ANIM_SWORD_PRIMARY, ANIM_SWORD_SECONDARY, ANIM_SWORD_POWER, ANIM_SWORD_ZOOM,
    ANIM_SHOTGUN, ANIM_SHOTGUN_PRIMARY, ANIM_SHOTGUN_SECONDARY, ANIM_SHOTGUN_RELOAD, ANIM_SHOTGUN_POWER, ANIM_SHOTGUN_ZOOM,
    ANIM_SMG, ANIM_SMG_PRIMARY, ANIM_SMG_SECONDARY, ANIM_SMG_RELOAD, ANIM_SMG_POWER, ANIM_SMG_ZOOM,
    ANIM_FLAMER, ANIM_FLAMER_PRIMARY, ANIM_FLAMER_SECONDARY, ANIM_FLAMER_RELOAD, ANIM_FLAMER_POWER, ANIM_FLAMER_ZOOM,
    ANIM_PLASMA, ANIM_PLASMA_PRIMARY, ANIM_PLASMA_SECONDARY, ANIM_PLASMA_RELOAD, ANIM_PLASMA_POWER, ANIM_PLASMA_ZOOM,
    ANIM_ZAPPER, ANIM_ZAPPER_PRIMARY, ANIM_ZAPPER_SECONDARY, ANIM_ZAPPER_RELOAD, ANIM_ZAPPER_POWER, ANIM_ZAPPER_ZOOM,
    ANIM_RIFLE, ANIM_RIFLE_PRIMARY, ANIM_RIFLE_SECONDARY, ANIM_RIFLE_RELOAD, ANIM_RIFLE_POWER, ANIM_RIFLE_ZOOM,
    ANIM_GRENADE, ANIM_GRENADE_PRIMARY, ANIM_GRENADE_SECONDARY, ANIM_GRENADE_RELOAD, ANIM_GRENADE_POWER, ANIM_GRENADE_ZOOM,
    ANIM_MINE, ANIM_MINE_PRIMARY, ANIM_MINE_SECONDARY, ANIM_MINE_RELOAD, ANIM_MINE_POWER, ANIM_MINE_ZOOM,
    ANIM_ROCKET, ANIM_ROCKET_PRIMARY, ANIM_ROCKET_SECONDARY, ANIM_ROCKET_RELOAD, ANIM_ROCKET_POWER, ANIM_ROCKET_ZOOM,
    ANIM_SWITCH, ANIM_USE,
    ANIM_MAX
};

#define RESIDUALS \
    RESIDUAL(burn, BURN, BURN); \
    RESIDUAL(bleed, BLEED, BLEED); \
    RESIDUAL(shock, SHOCK, SHOCK);
#define RESIDUALSF \
    RESIDUAL(burn, BURN, FIRE); \
    RESIDUAL(bleed, BLEED, BLEED); \
    RESIDUAL(shock, SHOCK, SHOCK);

#define FRAG_ENUM(pr, en) \
    en(pr, NONE, 0) en(pr, HEADSHOT, 1<<0) en(pr, OBLITERATE, 1<<1) \
    en(pr, SPREE1, 1<<2) en(pr, SPREE2, 1<<3) en(pr, SPREE3, 1<<4) en(pr, SPREE4, 1<<5) \
    en(pr, MKILL1, 1<<6) en(pr, MKILL2, 1<<7) en(pr, MKILL3, 1<<8) \
    en(pr, REVENGE, 1<<9) en(pr, DOMINATE, 1<<10) en(pr, FIRSTBLOOD, 1<<11) en(pr, BREAKER, 1<<12) \
    en(pr, SPREES, 4) en(pr, SPREE, 3) en(pr, MKILL, 7) \
    en(pr, CHECK, pr##_SPREE1|pr##_SPREE2|pr##_SPREE3|pr##_SPREE4) \
    en(pr, MULTI, pr##_MKILL1|pr##_MKILL2|pr##_MKILL3)
ENUMLI(FRAG, FRAG_ENUM);

enum
{
    SENDMAP_MPZ = 0, SENDMAP_CFG, SENDMAP_PNG, SENDMAP_TXT, SENDMAP_GAME, SENDMAP_WPT = SENDMAP_GAME, SENDMAP_MAX,
    SENDMAP_MIN = SENDMAP_PNG, SENDMAP_HAS = SENDMAP_MIN+1, SENDMAP_EDIT = SENDMAP_CFG+1, SENDMAP_ALL = SENDMAP_MAX-1
};
#ifdef CPP_GAME_SERVER
extern const char * const sendmaptypes[SENDMAP_MAX] = { "mpz", "cfg", "png", "txt", "wpt" };
#else
extern const char * const sendmaptypes[SENDMAP_MAX];
#define CLCOMMANDK(name, body, fail) \
    ICOMMAND(IDF_NAMECOMPLETE, getclient##name, "s", (char *who), \
    { \
        gameent *d = game::getclient(client::parseplayer(who)); \
        if(!d) { fail; return; } \
        body; \
    });
#define CLCOMMAND(name, body) CLCOMMANDK(name, body,)
#define CLCOMMANDMK(name, fmt, args, body, fail) \
    ICOMMAND(IDF_NAMECOMPLETE, getclient##name, fmt, args, \
    { \
        gameent *d = game::getclient(client::parseplayer(who)); \
        if(!d) { fail; return; } \
        body; \
    });
#define CLCOMMANDM(name, fmt, args, body) CLCOMMANDMK(name, fmt, args, body,)
#endif

#include "gamemode.h"
#include "weapons.h"

// network messages codes, c2s, c2c, s2c
enum
{
    N_CONNECT = 0, N_SERVERINIT, N_WELCOME, N_CLIENTINIT, N_POS, N_SPHY, N_TEXT, N_COMMAND, N_GAMELOG, N_DISCONNECT,
    N_SHOOT, N_DESTROY, N_STICKY, N_SUICIDE, N_DIED, N_POINTS, N_TOTALS, N_AVGPOS, N_DAMAGE, N_BURNRES, N_BLEEDRES, N_SHOCKRES, N_SHOTFX,
    N_LOADOUT, N_TRYSPAWN, N_SPAWNSTATE, N_SPAWN, N_WEAPDROP, N_WEAPSELECT, N_WEAPCOOK,
    N_MAPCHANGE, N_MAPVOTE, N_CLEARVOTE, N_CHECKPOINT, N_ITEMSPAWN, N_ITEMUSE, N_TRIGGER, N_EXECLINK,
    N_PING, N_PONG, N_CLIENTPING, N_TICK, N_ITEMACC, N_SERVMSG, N_GETGAMEINFO, N_GAMEINFO, N_ATTRMAP, N_RESUME,
    N_EDITMODE, N_EDITENT, N_EDITLINK, N_EDITVAR, N_EDITF, N_EDITT, N_EDITM, N_FLIP, N_COPY, N_PASTE, N_ROTATE, N_REPLACE, N_DELCUBE,
    N_CALCLIGHT, N_REMIP, N_EDITVSLOT, N_UNDO, N_REDO, N_CLIPBOARD, N_NEWMAP,
    N_GETMAP, N_SENDMAP, N_FAILMAP, N_SENDMAPFILE,
    N_MASTERMODE, N_ADDCONTROL, N_CLRCONTROL, N_CURRENTPRIV, N_SPECTATOR, N_WAITING, N_SETPRIV, N_SETTEAM, N_ADDPRIV,
    N_SETUPAFFIN, N_INFOAFFIN, N_MOVEAFFIN,
    N_TAKEAFFIN, N_RETURNAFFIN, N_RESETAFFIN, N_DROPAFFIN, N_SCOREAFFIN, N_INITAFFIN, N_SCORE,
    N_LISTDEMOS, N_SENDDEMOLIST, N_GETDEMO, N_SENDDEMO, N_DEMOREADY,
    N_DEMOPLAYBACK, N_RECORDDEMO, N_STOPDEMO, N_CLEARDEMOS,
    N_CLIENT, N_RELOAD, N_REGEN, N_INITAI, N_MAPCRC,
    N_SETPLAYERINFO, N_SWITCHTEAM, N_AUTHTRY, N_AUTHCHAL, N_AUTHANS, N_QUEUEPOS,
    N_STEAMCHAL, N_STEAMANS, N_STEAMFAIL,
    NUMMSG
};

enum
{
    SS_F_NONE = 0,
    SS_F_STEAMAUTH = 1<<0,
    SS_F_ALL = SS_F_STEAMAUTH
};

#ifdef CPP_GAME_SERVER
char msgsizelookup(int msg)
{
    static const int msgsizes[] =               // size inclusive message token, 0 for variable or not-checked sizes
    {
        N_CONNECT, 0, N_SERVERINIT, 5, N_WELCOME, 2, N_CLIENTINIT, 0, N_POS, 0, N_SPHY, 0, N_TEXT, 0, N_COMMAND, 0, N_GAMELOG, 0, N_DISCONNECT, 3,
        N_SHOOT, 0, N_DESTROY, 0, N_STICKY, 0, N_SUICIDE, 4, N_DIED, 0, N_POINTS, 5, N_TOTALS, 0, N_AVGPOS, 0, N_DAMAGE, 14, N_SHOTFX, 0,
        N_LOADOUT, 0, N_TRYSPAWN, 2, N_SPAWNSTATE, 0, N_SPAWN, 0, N_WEAPDROP, 0, N_WEAPSELECT, 0, N_WEAPCOOK, 0,
        N_MAPCHANGE, 0, N_MAPVOTE, 0, N_CLEARVOTE, 0, N_CHECKPOINT, 0, N_ITEMSPAWN, 3, N_ITEMUSE, 0, N_TRIGGER, 0, N_EXECLINK, 3,
        N_PING, 2, N_PONG, 2, N_CLIENTPING, 2, N_TICK, 4, N_ITEMACC, 0, N_SERVMSG, 0, N_GETGAMEINFO, 0, N_GAMEINFO, 0, N_ATTRMAP, 0, N_RESUME, 0,
        N_EDITMODE, 2, N_EDITENT, 0, N_EDITLINK, 4, N_EDITVAR, 0, N_EDITF, 16, N_EDITT, 16, N_EDITM, 17, N_FLIP, 14,
        N_COPY, 14, N_PASTE, 14, N_ROTATE, 15, N_REPLACE, 17, N_DELCUBE, 14,
        N_CALCLIGHT, 1, N_REMIP, 1, N_EDITVSLOT, 16, N_UNDO, 0, N_REDO, 0, N_NEWMAP, 0,
        N_GETMAP, 0, N_SENDMAP, 0, N_FAILMAP, 0, N_SENDMAPFILE, 0,
        N_MASTERMODE, 0, N_ADDCONTROL, 0, N_CLRCONTROL, 2, N_CURRENTPRIV, 3, N_SPECTATOR, 3, N_WAITING, 2, N_SETPRIV, 0, N_SETTEAM, 0, N_ADDPRIV, 0,
        N_SETUPAFFIN, 0, N_INFOAFFIN, 0, N_MOVEAFFIN, 0,
        N_DROPAFFIN, 0, N_SCOREAFFIN, 0, N_RETURNAFFIN, 0, N_TAKEAFFIN, 0, N_RESETAFFIN, 0, N_INITAFFIN, 0, N_SCORE, 0,
        N_LISTDEMOS, 1, N_SENDDEMOLIST, 0, N_GETDEMO, 3, N_SENDDEMO, 0, N_DEMOREADY, 0,
        N_DEMOPLAYBACK, 3, N_RECORDDEMO, 2, N_STOPDEMO, 1, N_CLEARDEMOS, 2,
        N_CLIENT, 0, N_RELOAD, 0, N_REGEN, 0, N_INITAI, 0, N_MAPCRC, 0,
        N_SETPLAYERINFO, 0, N_SWITCHTEAM, 0, N_AUTHTRY, 0, N_AUTHCHAL, 0, N_AUTHANS, 0, N_QUEUEPOS, 0,
        N_STEAMCHAL, 0, N_STEAMANS, 0, N_STEAMFAIL, 0,
        -1
    };
    static int sizetable[NUMMSG] = { -1 };
    if(sizetable[0] < 0)
    {
        memset(sizetable, -1, sizeof(sizetable));
        for(const int *p = msgsizes; *p >= 0; p += 2) sizetable[p[0]] = p[1];
    }
    return msg >= 0 && msg < NUMMSG ? sizetable[msg] : -1;
}
#else
extern char msgsizelookup(int msg);
#endif

struct demoheader
{
    char magic[16];
    int gamever, gamemode, mutators, starttime;
    string mapname;
};
#include "player.h"
#include "vars.h"
#ifndef CPP_GAME_SERVER
#include "ai.h"
#endif

template<class T>
static inline void adjustscaled(T &n, int s)
{
    T o = n;
    n = (T)(n/(1.f+sqrtf((float)curtime)/float(s)));
    if((o > 0 && n < 0) || (o < 0 && n > 0)) n = (T)0;
}

static inline void modecheck(int &mode, int &muts, int trying = 0)
{
    if(!m_game(mode)) mode = G_DEATHMATCH;
    if(gametype[mode].implied) muts |= gametype[mode].implied;
    int retries = G_M_MAX*G_M_MAX;
    loop(r, retries)
    {
        if(!muts) break; // nothing to do then
        bool changed = false;
        int mutsidx = gametype[mode].mutators[0];
        loopj(G_M_GSN) if(gametype[mode].mutators[j+1])
        {
            int m = 1<<(j+G_M_GSP);
            if(!(muts&m)) continue;
            if(trying&m) loopi(G_M_GSN) if(i != j && gametype[mode].mutators[i+1])
            {
                int n = 1<<(i+G_M_GSP);
                if(!(muts&n)) continue;
                if(!(gametype[mode].mutators[i+1]&m))
                {
                    muts &= ~n;
                    trying &= ~n;
                    changed = true;
                    break;
                }
            }
            if(changed) break;
            if(gametype[mode].flags&(1<<G_F_GSP))
            {
                //trying |= m; // game specific mutator exclusively provides allowed bits
                mutsidx = gametype[mode].mutators[j+1];
            }
        }
        if(changed) continue;
        loop(s, G_M_MAX)
        {
            if(!(mutsidx&(1<<mutstype[s].type)) && (muts&(1<<mutstype[s].type)))
            {
                muts &= ~(1<<mutstype[s].type);
                trying &= ~(1<<mutstype[s].type);
                changed = true;
                break;
            }
            if(muts&(1<<mutstype[s].type)) loopj(G_M_MAX)
            {
                if(!(mutstype[s].mutators&(1<<mutstype[j].type)) && (muts&(1<<mutstype[j].type)))
                {
                    if(trying && (trying&(1<<mutstype[j].type)) && !(gametype[mode].implied&(1<<mutstype[s].type)))
                    {
                        muts &= ~(1<<mutstype[s].type);
                        trying &= ~(1<<mutstype[s].type);
                    }
                    else
                    {
                        muts &= ~(1<<mutstype[j].type);
                        trying &= ~(1<<mutstype[j].type);
                    }
                    changed = true;
                    break;
                }
                int implying = gametype[mode].implied|mutstype[s].implied;
                if(implying && (implying&(1<<mutstype[j].type)) && !(muts&(1<<mutstype[j].type)))
                {
                    muts |= (1<<mutstype[j].type);
                    changed = true;
                    break;
                }
            }
            if(changed) break;
        }
        if(!changed) break;
    }
}

static inline const char *mastermodename(int type)
{
    switch(type)
    {
        case MM_OPEN: return "open";
        case MM_VETO: return "veto";
        case MM_LOCKED: return "locked";
        case MM_PRIVATE: return "private";
        case MM_PASSWORD: return "password";
        default: return "unknown";
    }
}

struct verinfo
{
    int type, flag, version, major, minor, patch, game, build, platform, arch, gpuglver, gpuglslver, crc;
    char *branch, *revision, *gpuvendor, *gpurenderer, *gpuversion;

    verinfo() : branch(NULL), revision(NULL), gpuvendor(NULL), gpurenderer(NULL), gpuversion(NULL) { reset(); }
    ~verinfo() { reset(); }

    void reset()
    {
        if(branch) delete[] branch;
        if(revision) delete[] revision;
        if(gpuvendor) delete[] gpuvendor;
        if(gpurenderer) delete[] gpurenderer;
        if(gpuversion) delete[] gpuversion;
        branch = revision = gpuvendor = gpurenderer = gpuversion = NULL;
        type = flag = version = major = minor = patch = game = arch = gpuglver = gpuglslver = crc = build = 0;
        platform = -1;
    }

    template <class T>
    void get(T &p)
    {
        string text;
        major = getint(p);
        minor = getint(p);
        patch = getint(p);
        game = getint(p);
        build = getint(p);
        platform = getint(p);
        arch = getint(p);
        gpuglver = getint(p);
        gpuglslver = getint(p);
        crc = getint(p);
        if(branch) delete[] branch;
        getstring(text, p); branch = newstring(text, MAXBRANCHLEN);
        if(revision) delete[] revision;
        getstring(text, p); revision = newstring(text, MAXREVISIONLEN);
        if(gpuvendor) delete[] gpuvendor;
        getstring(text, p); gpuvendor = newstring(text);
        if(gpurenderer) delete[] gpurenderer;
        getstring(text, p); gpurenderer = newstring(text);
        if(gpuversion) delete[] gpuversion;
        getstring(text, p); gpuversion = newstring(text);
    }

    template <class T>
    void put(T &p)
    {
        putint(p, major);
        putint(p, minor);
        putint(p, patch);
        putint(p, game);
        putint(p, build);
        putint(p, platform);
        putint(p, arch);
        putint(p, gpuglver);
        putint(p, gpuglslver);
        putint(p, crc);
        sendstring(branch ? branch : "", p);
        sendstring(revision ? revision : "", p);
        sendstring(gpuvendor ? gpuvendor : "", p);
        sendstring(gpurenderer ? gpurenderer : "", p);
        sendstring(gpuversion ? gpuversion : "", p);
    }

    void grab(verinfo &v)
    {
        major = v.major;
        minor = v.minor;
        patch = v.patch;
        game = v.game;
        build = v.build;
        platform = v.platform;
        arch = v.arch;
        gpuglver = v.gpuglver;
        gpuglslver = v.gpuglslver;
        crc = v.crc;
        if(branch) delete[] branch;
        branch = newstring(v.branch ? v.branch : "", MAXBRANCHLEN);
        if(revision) delete[] revision;
        revision = newstring(v.revision ? v.revision : "", MAXREVISIONLEN);
        if(gpuvendor) delete[] gpuvendor;
        gpuvendor = newstring(v.gpuvendor ? v.gpuvendor : "");
        if(gpurenderer) delete[] gpurenderer;
        gpurenderer = newstring(v.gpurenderer ? v.gpurenderer : "");
        if(gpuversion) delete[] gpuversion;
        gpuversion = newstring(v.gpuversion ? v.gpuversion : "");
    }
};

// inherited by gameent and server clients
struct clientstate
{
    int health, colour, model, pattern, checkpointspawn;
    int weapselect, weapammo[W_MAX][W_A_MAX], weapload[W_MAX][W_A_MAX], weapent[W_MAX], weapshot[W_MAX], weapstate[W_MAX], weapwait[W_MAX], weaptime[W_MAX], prevstate[W_MAX], prevtime[W_MAX];
    int lastdeath, lastspawn, lastpain, lastregen, lastregenamt, lastbuff, lastshoot, lastcook, lastaffinity, lastres[W_R_MAX], lastrestime[W_R_MAX];
    int burntime, burndelay, burndamage, bleedtime, bleeddelay, bleeddamage, shocktime, shockdelay, shockdamage, shockstun, shockstuntime;
    float shockstunscale, shockstunfall;
    int actortype, spawnpoint, ownernum, skill, points, frags, deaths, totalpoints, totalfrags, totaldeaths, spree, lasttimeplayed, timeplayed, cpmillis, cptime, queuepos;
    float totalavgpos;
    bool quarantine;
    string vanity;
    vector<int> loadweap, lastweap, randweap;
    verinfo version;

    clientstate() : colour(0), model(0), pattern(0), checkpointspawn(1), weapselect(W_CLAW), lastdeath(0), lastspawn(0), lastpain(0), lastregen(0), lastregenamt(0), lastbuff(0), lastshoot(0), lastcook(0), lastaffinity(0),
        actortype(A_PLAYER), spawnpoint(-1), ownernum(-1), skill(0), points(0), frags(0), deaths(0), totalpoints(0), totalfrags(0), totaldeaths(0), spree(0), lasttimeplayed(0), timeplayed(0),
        cpmillis(0), cptime(0), queuepos(-1), totalavgpos(0), quarantine(false)
    {
        vanity[0] = '\0';
        loadweap.shrink(0);
        lastweap.shrink(0);
        randweap.shrink(0);
        resetresidual();
    }
    ~clientstate() {}

    int gethealth(int gamemode, int mutators, bool full = false)
    {
        if(m_insta(gamemode, mutators)) return 1;
        int hp = A(actortype, health), sweap = m_weapon(actortype, gamemode, mutators);
        loopi(W_MAX) if(hasweap(i, sweap))
        {
            hp += W(i, modhealth)+(getammo(i, 0, true)*W(i, modhealthammo));
            if(i == weapselect) hp += W(i, modhealthequip);
        }
        hp = hp*(m_hard(gamemode, mutators) ? G(healthscalehard) : G(healthscale));
        if(full) hp = hp*(m_vampire(gamemode, mutators) ? G(maxhealthvampire) : G(maxhealth));
        return max(hp, 1);
    }

    bool setvanity(const char *v)
    {
        bool changed = strcmp(v, vanity);
        if(changed) copystring(vanity, v);
        return changed;
    }

    int getammo(int weap, int millis = 0, bool store = false)
    {
        if(!isweap(weap) || weapammo[weap][W_A_CLIP] < 0) return -1;
        int a = weapammo[weap][W_A_CLIP] > 0 ? weapammo[weap][W_A_CLIP] : 0;
        if(millis && weapstate[weap] == W_S_RELOAD && millis-weaptime[weap] < weapwait[weap] && weapload[weap][W_A_CLIP] > 0)
            a -= weapload[weap][W_A_CLIP];
        if(store) a += weapammo[weap][W_A_STORE];
        return a;
    }

    bool hasweap(int weap, int sweap, int level = 0, int exclude = -1)
    {
        if(isweap(weap) && weap != exclude)
        {
            int ammo = getammo(weap, 0, true);
            if(ammo >= 0) switch(level)
            {
                case 0: default: return true; break; // has weap at all
                case 1: if(w_carry(weap, sweap)) return true; break; // only carriable
                case 2: if(ammo > 0) return true; break; // only with actual ammo
                case 3: if(ammo > 0 || canreload(weap, sweap)) return true; break; // only reloadable or has ammo
                case 4: if(ammo >= (canreload(weap, sweap) ? 0 : W(weap, ammoclip))) return true; break; // only reloadable or those with < clipsize
                case 5: case 6: // special case to determine drop in classic games
                    if(weap == sweap || (level == 6 && weap < W_ITEM && weap >= W_OFFSET) || ammo > 0 || canreload(weap, sweap)) return true;
                    break;
            }
        }
        return false;
    }

    bool holdweap(int weap, int sweap, int millis)
    {
        return weap == weapselect || millis-weaptime[weap] < weapwait[weap] || hasweap(weap, sweap);
    }

    int holdweapcount(int sweap, int millis)
    {
        int n = 0;
        loopi(W_ALL) if(holdweap(i, sweap, millis)) n++;
        return n;
    }

    void addlastweap(int weap)
    {
        loopvrev(lastweap) if(lastweap[i] == weap) lastweap.remove(i);
        lastweap.add(weap);
    }

    int getlastweap(int sweap, int exclude = -1)
    {
        loopvrev(lastweap)
        {
            if(lastweap[i] == exclude) continue;
            if(hasweap(lastweap[i], sweap)) return lastweap[i];
        }
        return -1;
    }

    int bestweapcheck(int sweap, int level, int num = W_ALL, int exclude = -1)
    {
        loopvrev(lastweap) if(lastweap[i] < num && hasweap(lastweap[i], sweap, level, exclude)) return lastweap[i];
        loopirev(num) if(hasweap(i, sweap, level, exclude)) return i;
        return -1;
    }

    int bestweap(int sweap, bool ammo, bool pick = false, int exclude = -1)
    {
        int n = bestweapcheck(sweap, 3, W_ALL, exclude);
        if(isweap(n)) return n;
        if(ammo)
        {
            n = bestweapcheck(sweap, 2, W_ALL, exclude);
            if(isweap(n)) return n;
        }
        if(pick)
        {
            n = bestweapcheck(sweap, 0, W_ITEM, exclude);
            if(isweap(n)) return n;
        }
        return weapselect;
    }

    int carry(int sweap, int level = 1, int exclude = -1)
    {
        int carry = 0;
        loopi(W_ALL) if(hasweap(i, sweap, level, exclude)) carry++;
        return carry;
    }

    int drop(int sweap)
    {
        if(hasweap(weapselect, sweap, 1)) return weapselect;
        int w = getlastweap(sweap, weapselect);
        if(hasweap(w, sweap, 1)) return w;
        loopi(W_ALL) if(hasweap(i, sweap, 1)) return i;
        return -1;
    }

    void weapreset(bool full = false)
    {
        loopi(W_MAX)
        {
            weapstate[i] = prevstate[i] = W_S_IDLE;
            weapwait[i] = weaptime[i] = weapshot[i] = prevtime[0] = 0;
            loopj(W_A_MAX) weapload[i][j] = 0;
            if(full)
            {
                weapammo[i][W_A_CLIP] = weapent[i] = -1;
                weapammo[i][W_A_STORE] = 0;
            }
        }
        if(full) lastweap.shrink(0);
    }

    void setweapstate(int weap, int state, int delay, int millis, int offtime = 0, bool blank = false)
    {
        if(blank || (state >= W_S_RELOAD && state <= W_S_USE))
        {
            prevstate[weap] = W_S_IDLE;
            prevtime[weap] = millis;
        }
        else if(weapstate[weap] == W_S_ZOOM || weapstate[weap] == W_S_POWER)
        {
            prevstate[weap] = weapstate[weap];
            prevtime[weap] = weaptime[weap];
        }
        weapstate[weap] = state;
        weapwait[weap] = delay;
        weaptime[weap] = millis-offtime;
    }

    bool weapswitch(int weap, int millis, int delay = 0, int state = W_S_SWITCH)
    {
        if(isweap(weap) && weap < W_ALL)
        {
            if(isweap(weapselect))
            {
                addlastweap(weapselect);
                setweapstate(weapselect, W_S_SWITCH, delay, millis);
            }
            weapselect = weap;
            setweapstate(weap, state, delay, millis);
            return true;
        }
        return false;
    }

    bool weapwaited(int weap, int millis, int skip = 0)
    {
        if(weap != weapselect) skip &= ~(1<<W_S_RELOAD);
        if(!weapwait[weap] || weapstate[weap] == W_S_IDLE || (skip && skip&(1<<weapstate[weap]))) return true;
        int wait = weapwait[weap];
        if(W_S_INTERRUPT&(1<<weapstate[weap]))
        {
            if(W(weap, cookinterrupt)) return true;
            wait += 50;
        }
        return millis-weaptime[weap] >= wait;
    }

    bool candrop(int weap, int sweap, int millis, bool classic, int skip = 0)
    {
        if(weap < W_ALL && weap != sweap && (classic ? weap >= W_OFFSET && W2(weap, ammosub, false) && W2(weap, ammosub, true) : weap >= W_ITEM)
            && hasweap(weap, sweap) && weapwaited(weap, millis, skip) && weapwaited(weapselect, millis, skip))
                return true;
        return false;
    }

    bool canswitch(int weap, int sweap, int millis, int skip = 0)
    {
        if(!isweap(weap) || weap >= W_ALL) return false;
        if(weap != weapselect && weapwaited(weapselect, millis, skip) && hasweap(weap, sweap) && weapwaited(weap, millis, skip))
            return true;
        return false;
    }

    bool canshoot(int weap, int flags, int sweap, int millis, int skip = 0)
    {
        if(!(A(actortype, abilities)&(WS(flags) ? (1<<A_A_SECONDARY) : (1<<A_A_PRIMARY)))) return false;
        if(weap == weapselect || weap == W_MELEE)
            if(hasweap(weap, sweap) && getammo(weap, millis) >= (W2(weap, cooktime, WS(flags)) ? 1 : W2(weap, ammosub, WS(flags))) && weapwaited(weap, millis, W_S_INTERRUPT|skip))
                return true;
        return false;
    }

    bool canreload(int weap, int sweap, bool check = false, int millis = 0, int skip = 0)
    {
        if(actortype >= A_ENEMY ||
            ((W(weap, ammostore) < 0 || weapammo[weap][W_A_STORE] > 0)
                && (!check || (weap == weapselect && hasweap(weap, sweap) && weapammo[weap][W_A_CLIP] < W(weap, ammoclip) && weapstate[weap] != W_S_ZOOM && weapwaited(weap, millis, skip)))))
            return true;
        return false;
    }

    bool canuseweap(int gamemode, int mutators, int attr, int sweap, int millis, int skip = 0, bool full = true)
    {
        if(!m_classic(gamemode, mutators) && attr < W_ITEM && !hasweap(attr, sweap)) return false;
        if(full)
        {
            int total = W(attr, ammostore) > 0 ? W(attr, ammostore) : W(attr, ammoclip),
                ammo = W(attr, ammostore) > 0 ? weapammo[attr][W_A_STORE] : getammo(attr, millis);
            if(ammo >= total) return false;
        }
        return true;
    }

    bool canuse(int gamemode, int mutators, int type, int attr, attrvector &attrs, int sweap, int millis, int skip = 0, bool full = true)
    {
        switch(enttype[type].usetype)
        {
            case EU_AUTO: case EU_ACT: return true; break;
            case EU_ITEM:
            { // can't use when reloading or firing
                if(type != WEAPON || !isweap(attr) || !m_maxcarry(actortype, gamemode, mutators)) return false;
                if(!weapwaited(weapselect, millis, skip)) return false;
                return canuseweap(gamemode, mutators, attr, sweap, millis, skip, full);
            }
            default: break;
        }
        return false;
    }

    void useitem(int id, int type, int attr, int ammoamt, int sweap, int millis, int delay)
    {
        if(type != WEAPON || !isweap(attr)) return;
        int prevclip = max(weapammo[attr][W_A_CLIP], 0), prevstore = max(weapammo[attr][W_A_STORE], 0);
        weapswitch(attr, millis, delay, W_S_USE);
        weapammo[attr][W_A_CLIP] = W(attr, ammostore) <= 0 || !hasweap(attr, sweap) ? clamp(prevclip+ammoamt, 0, W(attr, ammoclip)) : prevclip;
        int diffclip = max(weapammo[attr][W_A_CLIP], 0)-prevclip, store = ammoamt-diffclip;
        weapammo[attr][W_A_STORE] = W(attr, ammostore) > 0 ? clamp(weapammo[attr][W_A_STORE]+store, 0, W(attr, ammostore)) : 0;
        weapload[attr][W_A_CLIP] = diffclip;
        weapload[attr][W_A_STORE] = max(weapammo[attr][W_A_STORE], 0)-prevstore;
        weapent[attr] = id;
    }

    void resetresidual(int n = -1)
    {
        if(n < 0 || n == W_R_BURN) lastres[W_R_BURN] = lastrestime[W_R_BURN] = burntime = burndelay = burndamage = 0;
        if(n < 0 || n == W_R_BLEED) lastres[W_R_BLEED] = lastrestime[W_R_BLEED] = bleedtime = bleeddelay = bleeddamage = 0;
        if(n < 0 || n == W_R_SHOCK)
        {
            lastres[W_R_SHOCK] = lastrestime[W_R_SHOCK] = shocktime = shockdelay = shockdamage = shockstun = shockstuntime = 0;
            shockstunscale = shockstunfall = 0.f;
        }
    }

    void clearstate()
    {
        spree = lastdeath = lastpain = lastregen = lastregenamt = lastbuff = lastshoot = lastcook = lastaffinity = 0;
        queuepos = -1;
        resetresidual();
    }

    void mapchange(bool change = false)
    {
        points = frags = deaths = cpmillis = cptime = spree = 0;
    }

    void respawn(int millis)
    {
        lastspawn = millis;
        clearstate();
        weapreset(true);
    }

    int updatetimeplayed()
    {
        if(lasttimeplayed)
        {
            int millis = totalmillis-lasttimeplayed, secs = millis/1000;
            timeplayed += secs;
            lasttimeplayed = totalmillis+(secs*1000)-millis;
        }
        else lasttimeplayed = totalmillis ? totalmillis : 1;
        return timeplayed;
    }

    float scoretime(bool update = true)
    {
        if(update) updatetimeplayed();
        return totalpoints/float(max(timeplayed, 1));
    }

    float kdratio(bool total = true)
    {
        if(total) return totalfrags >= totaldeaths ? (totalfrags/float(max(totaldeaths, 1))) : -(totaldeaths/float(max(totalfrags, 1)));
        return frags >= deaths ? (frags/float(max(deaths, 1))) : -(deaths/float(max(frags, 1)));
    }

    float combinedkdratio()
    {
        return ((totalfrags / float(max(totaldeaths, 1))) + (frags / float(max(deaths, 1)))) / ((frags || deaths) ? 2.0f : 1.0f);
    }

    float balancescore(float none = 0.0f)
    {
        switch(G(teambalancestyle))
        {
            case 1: return timeplayed;
            case 2: return totalpoints;
            case 3: return totalfrags;
            case 4: return scoretime();
            case 5: return kdratio();
            case 6: return combinedkdratio();
            case 7: return totalavgpos;
            case 0: default: return none;
        }
    }

    bool canrandweap(int weap)
    {
        int cweap = weap-W_OFFSET;
        if(!randweap.inrange(cweap)) return true;
        return randweap[cweap];
    }

    void spawnstate(int gamemode, int mutators, int sweap, int heal)
    {
        weapreset(true);
        health = heal > 0 ? heal : gethealth(gamemode, mutators);
        int s = sweap;
        if(!isweap(s)) s = m_weapon(actortype, gamemode, mutators);
        if(!isweap(s) || s >= W_ALL) s = W_CLAW;
        if(isweap(s))
        {
            weapammo[s][W_A_CLIP] = W(s, ammospawn);
            weapselect = s;
        }
        if(s != W_CLAW && m_edit(gamemode) && !W(W_CLAW, disabled)) weapammo[W_CLAW][W_A_CLIP] = W(W_CLAW, ammospawn); // give SniperGoth his claw in edit mode
        if(s != W_MELEE && A(actortype, abilities)&(1<<A_A_MELEE) && !W(W_MELEE, disabled)) weapammo[W_MELEE][W_A_CLIP] = W(W_MELEE, ammospawn);
        if(actortype < A_ENEMY)
        {
            if(m_kaboom(gamemode, mutators) && !W(W_MINE, disabled)) weapammo[W_MINE][W_A_CLIP] = W(W_MINE, ammospawn);
            else if(!m_race(gamemode) || m_ra_gauntlet(gamemode, mutators))
            {
                if(s != W_GRENADE && A(actortype, spawngrenades) >= (m_insta(gamemode, mutators) ? 2 : 1) && !W(W_GRENADE, disabled))
                    weapammo[W_GRENADE][W_A_CLIP] = W(W_GRENADE, ammospawn);
                if(s != W_MINE && A(actortype, spawnmines) >= (m_insta(gamemode, mutators) ? 2 : 1) && !W(W_MINE, disabled))
                    weapammo[W_MINE][W_A_CLIP] = W(W_MINE, ammospawn);
            }
        }
        if(m_maxcarry(actortype, gamemode, mutators) && m_loadout(gamemode, mutators))
        {
            vector<int> aweap;
            loopj(W_LOADOUT)
            {
                if(loadweap.inrange(j) && isweap(loadweap[j]) && !hasweap(loadweap[j], sweap) && m_check(W(loadweap[j], modes), W(loadweap[j], muts), gamemode, mutators) && !W(loadweap[j], disabled))
                    aweap.add(loadweap[j]);
                else aweap.add(0);
            }
            vector<int> rand, forcerand;
            for(int t = W_OFFSET; t < W_ITEM; t++)
                if(!hasweap(t, sweap) && m_check(W(t, modes), W(t, muts), gamemode, mutators) && !W(t, disabled) && aweap.find(t) < 0)
                    (canrandweap(t) ? rand : forcerand).add(t);
            int count = 0;
            loopj(W_LOADOUT)
            {
                if(aweap[j] <= 0) // specifically asking for random
                {
                    vector<int> &randsrc = rand.empty() ? forcerand : rand;
                    if(!randsrc.empty())
                    {
                        int n = rnd(randsrc.length());
                        aweap[j] = randsrc.remove(n);
                    }
                    else continue;
                }
                if(!isweap(aweap[j])) continue;
                weapammo[aweap[j]][W_A_CLIP] = W(aweap[j], ammospawn);
                count++;
                if(count >= m_maxcarry(actortype, gamemode, mutators)) break;
            }
            loopj(2) if(isweap(aweap[j])) { weapselect = aweap[j]; break; }
        }
        loopj(W_MAX) if(weapammo[j][W_A_CLIP] > W(j, ammoclip))
        {
            if(W(j, ammostore) > 0) weapammo[j][W_A_STORE] = clamp(weapammo[j][W_A_CLIP]-W(j, ammoclip), 0, W(j, ammostore));
            weapammo[j][W_A_CLIP] = W(j, ammoclip);
        }
    }

    void editspawn(int gamemode, int mutators)
    {
        clearstate();
        spawnstate(gamemode, mutators, m_weapon(actortype, gamemode, mutators), gethealth(gamemode, mutators));
    }

    int respawnwait(int millis, int delay)
    {
        return lastdeath ? max(0, delay-(millis-lastdeath)) : 0;
    }

    int protect(int millis, int delay)
    {
        if(actortype >= A_ENEMY || !lastspawn || !delay) return 0;
        if(G(protectbreakshoot) && lastshoot) return 0;
        if(G(protectbreakcook) && lastcook) return 0;
        if(G(protectbreakaffinity) && lastaffinity) return 0;
        int len = millis-lastspawn;
        if(len > delay) return 0;
        return delay-len;
    }

    #define RESIDUAL(name, type, pulse) bool name##ing(int millis, int len) { return len && lastres[W_R_##type] && millis-lastres[W_R_##type] <= len; }
    RESIDUALS
    #undef RESIDUAL
};

enum { PRJ_SHOT = 0, PRJ_GIBS, PRJ_DEBRIS, PRJ_EJECT, PRJ_ENT, PRJ_AFFINITY, PRJ_VANITY, PRJ_MAX };

namespace server
{
    extern void stopdemo();
    extern void hashpassword(int cn, int sessionid, const char *pwd, char *result, int maxlen = MAXSTRLEN);
    extern bool servcmd(int nargs, const char *cmd, const char *arg);
    extern const char *privname(int priv = PRIV_NONE, int actortype = A_PLAYER);
    extern const char *privnamex(int priv = PRIV_NONE, int actortype = A_PLAYER, bool local = false);
#ifdef CPP_GAME_SERVER
    struct clientinfo;
    extern void waiting(clientinfo *ci, int drop = 0, bool doteam = true, bool exclude = false);
    extern void setteam(clientinfo *ci, int team, int flags = TT_RESET, bool swaps = true);
    extern int chooseteam(clientinfo *ci, int suggest = -1, bool wantbal = false);
#endif
    extern int getlavaburntime(int mat); \
    extern int getlavaburndelay(int mat);
    extern float getwaterextinguish(int mat);
    extern float getwaterextinguishscale(int mat);
}

#define PLCHAN_ENUM(pr, en) en(pr, Announce, ANNOUNCE) en(pr, Message, MESSAGE) en(pr, Voice, VOICE) en(pr, Maximum, MAX)
ENUMNV(PLCHAN, PLCHAN_ENUM);

#ifdef CPP_GAME_SERVER
#define WATERPHYS(name,mat) (server::getwater##name(mat)*server::getwater##name##scale(mat))
#else
template<class T> inline void flashcolour(T &r, T &g, T &b, T br, T bg, T bb, float amt)
{
    r += (br-r)*amt;
    g += (bg-g)*amt;
    b += (bb-b)*amt;
}

template<class T> inline void flashcolourf(T &r, T &g, T &b, T &f, T br, T bg, T bb, T bf, float amt)
{
    r += (br-r)*amt;
    g += (bg-g)*amt;
    b += (bb-b)*amt;
    f += (bf-f)*amt;
}

namespace game
{
    extern int gamestate, gamemode, mutators;
}
#define AFFINITYPOS(n) \
    namespace n \
    { \
        extern bool getpos(int idx, vec &o); \
    }
AFFINITYPOS(capture);
AFFINITYPOS(defend);
AFFINITYPOS(bomber);
struct gameentity : extentity
{
    int schan, affinity, lastspawn, nextemit;
    linkvector kin;
    vec offset, curpos;
    float yaw, pitch;

    gameentity() : schan(-1), affinity(-1), lastspawn(0), nextemit(0), offset(0, 0, 0), yaw(0), pitch(0) {}
    ~gameentity()
    {
        if(issound(schan)) soundsources[schan].clear();
        schan = -1;
    }

    void getcurpos()
    {
        curpos = o;

        if(affinity >= 0)
        {
            if(m_capture(game::gamemode)) capture::getpos(affinity, curpos);
            else if(m_defend(game::gamemode)) defend::getpos(affinity, curpos);
            else if(m_bomber(game::gamemode)) bomber::getpos(affinity, curpos);
        }

        if(dynamic()) curpos.add(offset);
    }
    vec pos() { getcurpos(); return curpos; }
    vec *getpos() { getcurpos(); return &curpos; }
};

struct actitem
{
    enum { ENT = 0, PROJ };
    int type, target;
    float score;

    actitem() : type(ENT), target(-1), score(0) {}
    ~actitem() {}
};

static const char * const animnames[] =
{
    "idle", "walk forward", "walk backward", "walk left", "walk right", "dead", "dying", "swim",
    "mapmodel", "trigger on", "trigger off", "pain",
    "jump forward", "jump backward", "jump left", "jump right", "jump",
    "run forward", "run backward", "run left", "run right",
    "boost forward", "boost backward", "boost left", "boost right", "boost up",
    "parkour left", "parkour right", "parkour up", "parkour jump",
    "vault", "power slide", "fly kick",
    "sink", "edit", "win", "lose",
    "crouch", "crawl forward", "crawl backward", "crawl left", "crawl right",
    "crouch jump forward", "crouch jump backward", "crouch jump left", "crouch jump right", "crouch jump",
    "claw", "claw primary", "claw secondary", "claw reload", "claw power", "claw zoom",
    "pistol", "pistol primary", "pistol secondary", "pistol reload", "pistol power", "pistol zoom",
    "sword", "sword primary", "sword secondary", "sword power", "sword zoom",
    "shotgun", "shotgun primary", "shotgun secondary", "shotgun reload", "shotgun power", "shotgun zoom",
    "smg", "smg primary", "smg secondary", "smg reload", "smg power", "smg zoom",
    "flamer", "flamer primary", "flamer secondary", "flamer reload", "flamer power", "flamer zoom",
    "plasma", "plasma primary", "plasma secondary", "plasma reload", "plasma power", "plasma zoom",
    "zapper", "zapper primary", "zapper secondary", "zapper reload", "zapper power", "zapper zoom",
    "rifle", "rifle primary", "rifle secondary", "rifle reload", "rifle power", "rifle zoom",
    "grenade", "grenade primary", "grenade secondary", "grenade reload", "grenade power", "grenade zoom",
    "mine", "mine primary", "mine secondary", "mine reload", "mine power", "mine zoom",
    "rocket", "rocket primary", "rocket secondary", "rocket reload", "rocket power", "rocket zoom",
    "switch", "use",
    ""
};

struct eventicon
{
    enum { SPREE = 0, MULTIKILL, HEADSHOT, DOMINATE, REVENGE, FIRSTBLOOD, BREAKER, WEAPON, AFFINITY, TOTAL, SORTED = WEAPON, VERBOSE = WEAPON };
    int type, millis, fade, length, value;
};

struct stunevent
{
    int weap, millis, delay;
    float scale, gravity;
};

struct jitterevent
{
    int weap, millis, delay, last;
    float yaw, pitch;
};

enum
{
    TAG_CAMERA, TAG_CROWN, TAG_R_CROWN, TAG_TORSO, TAG_R_TORSO, TAG_LIMBS, TAG_R_LIMBS, TAG_WAIST,
    TAG_MUZZLE, TAG_ORIGIN, TAG_EJECT1, TAG_EJECT2, TAG_JET_LEFT, TAG_JET_RIGHT, TAG_JET_BACK, TAG_TOE_LEFT, TAG_TOE_RIGHT,
    TAG_MAX, // WARNING: ensure this value is more than or equal to VANITYMAX
    TAG_EJECT = TAG_EJECT1, TAG_N_EJECT = 2, TAG_JET = TAG_JET_LEFT, TAG_N_JET = 3, TAG_TOE = TAG_TOE_LEFT, TAG_N_TOE = 2
};

enum
{
    WS_BEGIN_CHAN = 0,
    WS_MAIN_CHAN,
    WS_END_CHAN,
    WS_POWER_CHAN,
    WS_OTHER_CHAN,

    WS_CHANS
};

struct projent;

struct gameent : dynent, clientstate
{
    editinfo *edit;
    ai::aiinfo *ai;
    int team, clientnum, privilege, projid, lastnode, checkpoint, cplast, respawned, suicided, lastupdate, lastpredict, plag, ping, lastflag, totaldamage,
        actiontime[AC_MAX], impulse[IM_MAX], impulsetime[IM_T_MAX], smoothmillis, turnside, turnmillis, plchan[PLCHAN_MAX], wschan[WS_CHANS], sschan[2],
        lasthit, lastteamhit, lastkill, lastattacker, lastpoints, quake, wasfiring, lastfoot;
    float deltayaw, deltapitch, newyaw, newpitch, stunscale, stungravity, turnyaw, turnroll;
    bool action[AC_MAX], conopen, k_up, k_down, k_left, k_right, obliterated, headless;
    vec tag[TAG_MAX];
    string hostip, name, handle, steamid, info, obit;
    vector<gameent *> dominating, dominated;
    vector<eventicon> icons;
    vector<stunevent> stuns;
    vector<jitterevent> jitters;
    vector<int> vitems;
    fx::emitter *weaponfx, *impulsefx;
    projent *projchain;

    gameent() : edit(NULL), ai(NULL), team(T_NEUTRAL), clientnum(-1), privilege(PRIV_NONE), projid(0), checkpoint(-1), cplast(0), lastupdate(0), lastpredict(0), plag(0), ping(0),
        totaldamage(0), smoothmillis(-1), lastattacker(-1), lastpoints(0), quake(0), wasfiring(-1), conopen(false), k_up(false), k_down(false), k_left(false), k_right(false), obliterated(false),
        weaponfx(NULL), impulsefx(NULL), projchain(NULL)
    {
        state = CS_DEAD;
        type = ENT_PLAYER;
        copystring(hostip, "0.0.0.0");
        name[0] = handle[0] = steamid[0] = info[0] = obit[0] = '\0';
        removesounds(true);
        respawn(-1, 0, 0);
    }
    ~gameent()
    {
        removefx();
        removesounds();
        freeeditinfo(edit);
        if(ai) delete ai;
        removetrackedparticles(this);
        removetrackedsounds(this);
        fx::removetracked(this);
        entities::removepassenger(this);
    }

    static bool is(int t) { return t == ENT_PLAYER || t == ENT_AI; }
    static bool is(physent *d) { return d && (d->type == ENT_PLAYER || d->type == ENT_AI); }


    void addstun(int weap, int millis, int delay, float scale, float gravity)
    {
        if(delay <= 0 || (scale == 0 && gravity == 0)) return;
        stunevent &s = stuns.add();
        s.weap = weap;
        s.millis = millis;
        s.delay = delay;
        s.scale = scale;
        s.gravity = gravity;
    }

    float stunned(int millis, bool gravity = false)
    {
        float stun = 0;
        loopvrev(stuns)
        {
            stunevent &s = stuns[i];
            if(!s.delay)
            {
                stuns.remove(i);
                continue;
            }
            int etime = s.millis+s.delay, len = etime-clamp(millis, s.millis, etime);
            if(len > 0) stun += (gravity ? s.gravity : s.scale)*(len/float(s.delay));
            if(gravity && millis >= etime) stuns.remove(i);
        }
        return 1.f-clamp(stun, 0.f, 1.f);
    }

    void addjitter(int weap, int millis, int delay, float yawmin, float yawmax, float pitchmin, float pitchmax, int dir = 0)
    {
        if(delay <= 0) return;
        jitterevent &s = jitters.add();
        s.weap = weap;
        s.millis = s.last = millis;
        s.delay = delay;
        float yaw1 = min(yawmin, yawmax), yaw2 = max(yawmin, yawmax), yawv = yaw1,
              pitch1 = min(pitchmin, pitchmax), pitch2 = max(pitchmin, pitchmax), pitchv = pitch1;
        if(yaw2 > yaw1) yawv += (yaw2-yaw1)*(rnd(10000)+1)/10000.f;
        if(pitch2 > pitch1) pitchv += (pitch2-pitch1)*(rnd(10000)+1)/10000.f;
        if(dir)
        {
            int value = rnd((dir > 0 ? dir : -dir)+1);
            if(dir > 0 ? value == 0 : value != 0) pitchv = -pitchv;
        }
        s.yaw = yawv;
        s.pitch = pitchv;
    }

    void jitter(int millis)
    {
        loopvrev(jitters)
        {
            jitterevent &s = jitters[i];
            if(!s.delay)
            {
                jitters.remove(i);
                continue;
            }
            int etime = s.millis+s.delay, len = clamp(millis, s.millis, etime)-s.last;
            if(len > 0)
            {
                float scale = len/float(s.delay);
                yaw += s.yaw*scale;
                if(yaw < 0.0f) yaw = 360.0f - fmodf(-yaw, 360.0f);
                else if(yaw >= 360.0f) yaw = fmodf(yaw, 360.0f);
                pitch += s.pitch*scale;
                if(pitch > 89.9f) pitch = 89.9f;
                else if(pitch < -89.9f) pitch = -89.9f;
                s.last = millis;
            }
            if(millis >= etime) jitters.remove(i);
        }
    }

    void configure(int millis, int gamemode, int mutators, int affinities = 0, int cur = 0)
    {
        #define MODPHYSL \
            MODPHYS(speed, float, speedscale); \
            MODPHYS(jumpspeed, float, speedscale); \
            MODPHYS(impulsespeed, float, speedscale); \
            MODPHYS(weight, float, curscale); \
            MODPHYS(buoyancy, float, curscale);

        float scale = A(actortype, scale), speedscale = 1;

        if(actortype >= A_ENEMY && entities::ents.inrange(spawnpoint) && entities::ents[spawnpoint]->type == ACTOR)
        {
            if(entities::ents[spawnpoint]->attrs[8] > 0) speedscale *= entities::ents[spawnpoint]->attrs[8]/100.f;
            if(entities::ents[spawnpoint]->attrs[9] > 0) scale *= (entities::ents[spawnpoint]->attrs[9]/100.f);
        }

        if(m_resize(gamemode, mutators) && cur)
        {
            float amtscale = m_insta(gamemode, mutators) ? 1+(spree*G(instaresizeamt)) : max(health, 1)/float(max(gethealth(gamemode, mutators), 1));
            if(amtscale < 1) amtscale = (amtscale*(1-G(minresizescale)))+G(minresizescale);
            scale *= clamp(amtscale, G(minresizescale), G(maxresizescale));
        }

        if(scale != curscale)
        {
            if(cur && state == CS_ALIVE)
                curscale = scale > curscale ? min(curscale+cur/2000.f, scale) : max(curscale-cur/2000.f, scale);
            else curscale = scale;
        }

        loopi(W_MAX) if(!cur || (weapstate[i] != W_S_IDLE && (weapselect != i || (weapstate[i] != W_S_POWER && weapstate[i] != W_S_ZOOM)) && millis-weaptime[i] >= weapwait[i]+100))
            setweapstate(i, W_S_IDLE, 0, millis);

        xradius = yradius = actors[actortype].radius*curscale;
        zradius = actors[actortype].height*curscale;
        radius = max(xradius, yradius);
        aboveeye = curscale;

        #define MODPHYS(a,b,c) a = A(actortype, a)*c;
        MODPHYSL;
        #undef MODPHYS
        if(m_single(gamemode, mutators))
        {
            #define MODPHYS(a,b,c) a += A(actortype, a##extra)*c;
            MODPHYSL;
            #undef MODPHYS
        }

        if(cur)
        {
            if(affinities > 0)
            {
                if(m_capture(gamemode))
                {
                    #define MODPHYS(a,b,c) a += G(capturecarry##a)+(affinities*G(capturecarry##a##each));
                    MODPHYSL;
                    #undef MODPHYS
                }
                else if(m_bomber(gamemode))
                {
                    #define MODPHYS(a,b,c) a += G(bombercarry##a);
                    MODPHYSL;
                    #undef MODPHYS
                }
            }
            int sweap = m_weapon(actortype, gamemode, mutators);
            loopi(W_MAX) if(hasweap(i, sweap))
            {
                int numammo = getammo(i, 0, true);
                #define MODCARRY(a) (m_arena(gamemode, mutators) ? (a)*A(actortype, maxcarry)/W_LOADOUT : a)
                #define MODPHYS(a,b,c) a += MODCARRY(W(i, mod##a)+(numammo*W(i, mod##a##ammo)));
                MODPHYSL;
                #undef MODPHYS
                if(i != weapselect) continue;
                #define MODPHYS(a,b,c) a += W(i, mod##a##equip);
                MODPHYSL;
                #undef MODPHYS
                switch(weapstate[i])
                {
                    case W_S_PRIMARY: case W_S_SECONDARY:
                    {
                        #define MODPHYS(a,b,c) a += W2(i, mod##a##attack, weapstate[i] == W_S_SECONDARY);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                    case W_S_RELOAD:
                    {
                        #define MODPHYS(a,b,c) a += W(i, mod##a##reload);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                    case W_S_SWITCH:
                    {
                        #define MODPHYS(a,b,c) a += W(i, mod##a##switch);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                    case W_S_USE:
                    {
                        #define MODPHYS(a,b,c) a += W(i, mod##a##use);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                    case W_S_POWER:
                    {
                        #define MODPHYS(a,b,c) a += W(i, mod##a##power);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                    case W_S_ZOOM:
                    {
                        #define MODPHYS(a,b,c) a += W(i, mod##a##zoom);
                        MODPHYSL;
                        #undef MODPHYS
                        break;
                    }
                }
            }
            jitter(millis);
            stunscale = stunned(millis, false);
            stungravity = stunned(millis, true);
        }
        else
        {
            stunscale = stungravity = 1;
            height = zradius;
        }

        #define MODPHYS(a,b,c) a = max(a, b(0));
        MODPHYSL;
        #undef MODPHYS
        #undef MODPHYSL
    }

    int getprojid()
    {
        projid++;
        if(projid < 2) projid = 2;
        return projid;
    }

    void removefx()
    {
        if(weaponfx) fx::stopfx(weaponfx);
        if(impulsefx) fx::stopfx(impulsefx);
    }

    void removesounds(bool init = false)
    {
        loopi(PLCHAN_MAX)
        {
            if(!init && issound(plchan[i])) soundsources[plchan[i]].clear();
            plchan[i] = -1;
        }
        loopi(WS_CHANS)
        {
            if(!init && issound(wschan[i])) soundsources[wschan[i]].clear();
            wschan[i] = -1;
        }
        loopi(2)
        {
            if(!init && issound(sschan[i])) soundsources[sschan[i]].clear();
            sschan[i] = -1;
        }
    }

    void stopmoving(bool full)
    {
        if(full) move = strafe = 0;
        loopi(AC_MAX)
        {
            action[i] = false;
            actiontime[i] = 0;
        }
    }

    void clearstate(int millis, int gamemode, int mutators)
    {
        loopi(IM_MAX) impulse[i] = 0;
        loopi(IM_T_MAX) impulsetime[i] = 0;
        lasthit = lastkill = quake = turnside = turnmillis = 0;
        lastteamhit = lastflag = respawned = suicided = lastnode = lastfoot = wasfiring = -1;
        turnyaw = turnroll = 0;
        obit[0] = '\0';
        obliterated = headless = false;
        icons.shrink(0);
        stuns.shrink(0);
        jitters.shrink(0);
        used.shrink(0);
        cleartags();
    }

    void respawn(int millis, int gamemode, int mutators)
    {
        stopmoving(false);
        removefx();
        removesounds();
        physent::reset();
        clearstate(millis, gamemode, mutators);
        clientstate::respawn(millis);
        configure(millis, gamemode, mutators);
    }

    void editspawn(int gamemode, int mutators)
    {
        stopmoving(false);
        clearstate(lastmillis, gamemode, mutators);
        inmaterial = airmillis = floormillis = 0;
        forcepos = false;
        physstate = PHYS_FALL;
        vel = falling = vec(0, 0, 0);
        floor = vec(0, 0, 1);
        resetinterp();
        clientstate::editspawn(gamemode, mutators);
    }

    void resetstate(int millis, int gamemode, int mutators)
    {
        respawn(millis, gamemode, mutators);
        checkpoint = -1;
        frags = deaths = totaldamage = cplast = 0;
    }

    void mapchange(int millis, int gamemode, int mutators)
    {
        dominating.shrink(0);
        dominated.shrink(0);
        icons.shrink(0);
        resetstate(millis, gamemode, mutators);
        clientstate::mapchange();
    }

    void cleartags()
    {
        loopi(TAG_MAX) tag[i] = vec(-1, -1, -1);
    }

    float headsize()
    {
        if(!(actors[actortype].collidezones&CLZ_HEAD)) return 0.f;
        return max(xradius*0.45f, yradius*0.45f);
    }

    vec &headtag()
    {
        if(tag[TAG_CROWN] == vec(-1, -1, -1))
        {
            tag[TAG_CROWN] = o;
            tag[TAG_CROWN].z -= headsize()*0.375f;
        }
        return tag[TAG_CROWN];
    }

    vec &cameratag()
    {
        if(tag[TAG_CAMERA] == vec(-1, -1, -1)) tag[TAG_CAMERA] = o;
        return tag[TAG_CAMERA];
    }

    vec &headbox()
    {
        if(tag[TAG_R_CROWN] == vec(-1, -1, -1))
            tag[TAG_R_CROWN] = vec(xradius*0.5f, yradius*0.5f, headsize());
        return tag[TAG_R_CROWN];
    }

    float torsosize()
    {
        if(!(actors[actortype].collidezones&CLZ_TORSO)) return 0.f;
        return (headtag().z-headbox().z)-torsotag().z;
    }

    vec &torsotag()
    {
        if(tag[TAG_TORSO] == vec(-1, -1, -1))
        {
            tag[TAG_TORSO] = o;
            tag[TAG_TORSO].z -= height*0.45f;
        }
        return tag[TAG_TORSO];
    }

    vec &torsobox()
    {
        if(tag[TAG_R_TORSO] == vec(-1, -1, -1))
            tag[TAG_R_TORSO] = vec(xradius, yradius, torsosize());
        return tag[TAG_R_TORSO];
    }

    float limbsize()
    {
        if(!(actors[actortype].collidezones&CLZ_LIMBS)) return 0.f;
        return ((torsotag().z-torsobox().z)-(o.z-height))*0.5f;
    }

    vec &limbstag()
    {
        if(tag[TAG_LIMBS] == vec(-1, -1, -1))
        {
            tag[TAG_LIMBS] = torsotag();
            tag[TAG_LIMBS].z -= torsobox().z+limbsize();
        }
        return tag[TAG_LIMBS];
    }

    vec &limbsbox()
    {
        if(tag[TAG_R_LIMBS] == vec(-1, -1, -1))
            tag[TAG_R_LIMBS] = vec(xradius*0.85f, yradius*0.85f, limbsize());
        return tag[TAG_R_LIMBS];
    }

    vec &origintag(int weap = -1)
    {
        if(!isweap(weap)) weap = weapselect;
        if(tag[TAG_ORIGIN] == vec(-1, -1, -1))
        {
            if(weap == W_MELEE) tag[TAG_ORIGIN] = feetpos();
            else
            {
                vec dir, right;
                vecfromyawpitch(yaw, pitch, 1, 0, dir);
                dir.mul(radius*3);
                vecfromyawpitch(yaw, pitch, 0, -1, right);
                right.mul(radius*1.5f);
                tag[TAG_ORIGIN] = vec(headpos(-height/6)).add(right).add(dir);
            }
        }
        return tag[TAG_ORIGIN];
    }

    vec &muzzletag(int weap = -1)
    {
        if(!isweap(weap)) weap = weapselect;
        if(tag[TAG_MUZZLE] == vec(-1, -1, -1))
        {
            if(weap == W_SWORD && ((weapstate[weap] == W_S_PRIMARY) || (weapstate[weap] == W_S_SECONDARY)))
            {
                float frac = (lastmillis-weaptime[weap])/float(weapwait[weap]), yx = yaw, px = pitch;
                if(weapstate[weap] == W_S_PRIMARY)
                {
                    yx -= 90;
                    yx += frac*180;
                    if(yx >= 360) yx -= 360;
                    if(yx < 0) yx += 360;
                }
                else
                {
                    px += 90;
                    px -= frac*180;
                    if(px >= 180) px -= 360;
                    if(px < -180) px += 360;
                }
                tag[TAG_MUZZLE] = vec(origintag(weap)).add(vec(yx*RAD, px*RAD).mul(8));
            }
            else
            {
                vec dir(yaw*RAD, pitch*RAD);
                if(weap != W_CLAW)
                {
                    vec right;
                    vecfromyawpitch(yaw, pitch, 0, -1, right);
                    tag[TAG_MUZZLE] = vec(origintag(weap)).add(dir.mul(radius*0.75f)).add(right.mul(radius*0.6f));
                }
                else tag[TAG_MUZZLE] = vec(origintag(weap)).add(dir.mul(radius*2));
            }
        }
        return tag[TAG_MUZZLE];
    }

    vec &ejecttag(int weap = -1, int idx = 0)
    {
        if(!isweap(weap)) weap = weapselect;
        if(idx < 0 || idx >= TAG_N_EJECT) idx = 0;
        int tnum = TAG_EJECT+idx;
        if(tag[tnum] == vec(-1, -1, -1)) tag[tnum] = idx ? origintag(weap) : muzzletag(weap);
        return tag[tnum];
    }

    vec &waisttag()
    {
        if(tag[TAG_WAIST] == vec(-1, -1, -1))
        {
            vec dir;
            vecfromyawpitch(yaw, 0, -1, 0, dir);
            dir.mul(radius*1.6f);
            dir.z -= height*0.5f;
            tag[TAG_WAIST] = vec(o).add(dir);
        }
        return tag[TAG_WAIST];
    }

    vec &jetlefttag()
    {
        if(tag[TAG_JET_LEFT] == vec(-1, -1, -1))
        {
            vec dir;
            vecfromyawpitch(yaw, 0, -1, -1, dir);
            dir.mul(radius);
            dir.z -= height;
            tag[TAG_JET_LEFT] = vec(o).add(dir);
        }
        return tag[TAG_JET_LEFT];
    }

    vec &jetrighttag()
    {
        if(tag[TAG_JET_RIGHT] == vec(-1, -1, -1))
        {
            vec dir;
            vecfromyawpitch(yaw, 0, -1, 1, dir);
            dir.mul(radius);
            dir.z -= height;
            tag[TAG_JET_RIGHT] = vec(o).add(dir);
        }
        return tag[TAG_JET_RIGHT];
    }

    vec &jetbacktag()
    {
        if(tag[TAG_JET_BACK] == vec(-1, -1, -1))
        {
            vec dir;
            vecfromyawpitch(yaw, 0, -1, 0, dir);
            dir.mul(radius*1.25f);
            dir.z -= height*0.35f;
            tag[TAG_JET_BACK] = vec(o).add(dir);
        }
        return tag[TAG_JET_BACK];
    }

    vec &jettag(int idx)
    {
        switch(idx)
        {
            case 2: return jetbacktag();
            case 1: return jetrighttag();
            case 0: return jetlefttag();
            default: break;
        }
        return jetbacktag();
    }

    vec &foottag(int idx = 0)
    {
        if(idx < 0 || idx > 1) idx = 0;
        int tnum = TAG_TOE+idx;
        if(tag[tnum] == vec(-1, -1, -1))
        {
            int millis = lastmillis%500;
            float amt = millis > 250 ? (500-millis)/250.f : millis/250.f;
            vec dir, right;
            vecfromyawpitch(yaw, pitch, 1, 0, dir);
            vecfromyawpitch(yaw, pitch, 0, idx ? 1 : -1, right);
            dir.mul(radius*0.5f);
            right.mul(radius*(!move && strafe ? amt-0.5f : 0.5f));
            dir.z -= height*0.6f+(height*0.4f*(idx ? 1-amt : amt));
            tag[tnum] = vec(o).add(dir).add(right);
        }
        return tag[tnum];
    }

    vec &toetag(int idx)
    {
        switch(idx)
        {
            case 1: return foottag(0);
            case 0: return foottag(1);
            default: break;
        }
        return foottag(0);
    }

    vec *gettag(int idx)
    {
        if(idx < 0 || idx >= TAG_MAX) return NULL;
        switch(idx)
        {
            case TAG_CAMERA: cameratag(); break;
            case TAG_CROWN: headtag(); break;
            case TAG_R_CROWN: headbox(); break;
            case TAG_TORSO: torsotag(); break;
            case TAG_R_TORSO: torsobox(); break;
            case TAG_LIMBS: limbstag(); break;
            case TAG_R_LIMBS: limbsbox(); break;
            case TAG_WAIST: waisttag(); break;
            case TAG_MUZZLE: muzzletag(); break;
            case TAG_ORIGIN: origintag(); break;
            case TAG_EJECT1: ejecttag(-1, 0); break;
            case TAG_EJECT2: ejecttag(-1, 1); break;
            case TAG_JET_LEFT: jetlefttag(); break;
            case TAG_JET_RIGHT: jetrighttag(); break;
            case TAG_JET_BACK: jetbacktag(); break;
            case TAG_TOE_LEFT: toetag(0); break;
            case TAG_TOE_RIGHT: toetag(1); break;
            default: return NULL;
        }
        return &tag[idx];
    }

    void resetjump(bool wait = false)
    {
        airmillis = turnside = impulse[IM_FLING] = 0;
        if(!impulsecostcount) impulse[IM_COUNT] = 0;
        impulsetime[IM_T_JUMP] = 0;
        if(!wait)
        {
            impulse[IM_TYPE] = IM_T_JUMP;
            impulsetime[IM_T_PUSHER] = 0;
        }
    }

    void resetair(bool wait = false)
    {
        resetphys();
        resetjump(wait);
    }

    void doimpulse(int type, int millis, int cost = 0, int side = 0, int turn = 0, float yaw = 0, float roll = 0)
    {
        if(type < 0 || type >= IM_T_MAX) return;
        if(cost)
        {
            if(type == IM_T_PUSHER) impulse[IM_PUSHER] = cost;
            else impulse[IM_METER] += cost;
        }
        impulse[IM_SLIP] = impulsetime[type] = millis;
        impulse[IM_TYPE] = type;
        if(type != IM_T_JUMP && !impulsetime[IM_T_JUMP]) impulsetime[IM_T_JUMP] = millis;
        if(impulsecounttypes&(1<<type)) impulse[IM_COUNT]++;
        if(type != IM_T_AFTER)
        {
            impulse[IM_REGEN] = millis;
            if(type == IM_T_PUSHER) resetair(true);
            else resetphys((IM_T_TOUCH&(1<<type)) != 0);
        }
        else impulse[IM_FLING] = 0;
        turnside = side;
        turnmillis = turn;
        turnyaw = yaw;
        turnroll = roll;
    }

    bool hasparkour()
    {
        return impulse[IM_TYPE] == IM_T_PARKOUR || impulse[IM_TYPE] == IM_T_VAULT;
    }

    bool delayimpulse(bool regen)
    {
        loopi(IM_T_MAX)
        {
            if(!impulsetime[i]) continue;
            int check = 0, delay = 0;
            switch(i)
            {
                case IM_T_BOOST: check = impulseboostcheck; delay = impulseboostdelay; break;
                case IM_T_DASH: check = impulsedashcheck; delay = impulsedashdelay; break;
                case IM_T_SLIDE: check = impulseslidecheck; delay = impulseslidedelay; break;
                case IM_T_LAUNCH: check = impulselaunchcheck; delay = impulselaunchdelay; break;
                case IM_T_MELEE: check = impulsemeleecheck; delay = impulsemeleedelay; break;
                case IM_T_KICK: check = impulsekickcheck; delay = impulsekickdelay; break;
                case IM_T_GRAB: check = impulsegrabcheck; delay = impulsegrabdelay; break;
                case IM_T_PARKOUR: check = impulseparkourcheck; delay = impulseparkourdelay; break;
                case IM_T_VAULT: check = impulsevaultcheck; delay = impulsevaultdelay; break;
                case IM_T_POUND: check = impulsepoundcheck; delay = impulsepounddelay; break;
                case IM_T_PUSHER: delay = impulse[IM_PUSHER]; break;
                case IM_T_AFTER: continue; // no delay
                case IM_T_JUMP: default:
                    if(regen) continue; // jump doesn't inhibit regen
                    check = impulsejumpcheck;
                    delay = impulsejumpdelay;
                    break;
            }
            if(check&(1<<i) && lastmillis - impulsetime[i] < delay) return false;
        }
        return true;
    }

    bool regenimpulse()
    {
        if(!isalive() || !delayimpulse(true)) return false;
        if(impulseregendelay && lastmillis - impulse[IM_REGEN] < impulseregendelay) return false;
        return (impulsecostcount && impulse[IM_COUNT] > 0) || (impulsecostmeter && impulse[IM_METER] > 0);
    }

    bool canimpulse(int type)
    {
        if(!(A(actortype, impulse)&(1<<type))) return false;
        if(PHYS(gravity) != 0 && impulsetouchchecks&(1<<type) && impulsetouchtypes&(1<<impulse[IM_TYPE])) return false;
        if(impulsecounttypes&(1<<type) && impulse[IM_COUNT] >= impulsecount) return false;
        return delayimpulse(false);
    }

    void addicon(int type, int millis, int fade, int value = 0)
    {
        int pos = -1;
        loopv(icons)
        {
            if(icons[i].type == type)
            {
                switch(icons[i].type)
                {
                    case eventicon::WEAPON:
                    {
                        if(icons[i].value != value) continue;
                        break;
                    }
                    default: break;
                }
                icons[i].length = max(icons[i].fade, fade);
                icons[i].fade = millis-icons[i].millis+fade;
                icons[i].value = value;
                return;
            }
            if(pos < 0 && type >= eventicon::SORTED && icons[i].type > type) pos = i;
        }
        eventicon e;
        e.type = type;
        e.millis = millis;
        e.fade = e.length = fade;
        e.value = value;
        if(pos < 0) icons.add(e);
        else icons.insert(pos, e);
    }

    void setname(const char *n)
    {
        if(n && *n) copystring(name, n, MAXNAMELEN+1);
        else name[0] = '\0';
    }

    bool setvanity(const char *v)
    {
        if(clientstate::setvanity(v))
        {
            vitems.shrink(0);
            return true;
        }
        return false;
    }

    void setinfo(const char *n, int c, int m, int p, const char *v, vector<int> &w, vector<int> &r)
    {
        setname(n);
        colour = c;
        model = m;
        pattern = p;
        setvanity(v);
        loadweap.shrink(0);
        loopv(w) loadweap.add(w[i]);
        randweap.shrink(0);
        loopv(r) randweap.add(r[i]);
    }

    bool hasmelee(int millis, bool check = true)
    {
        if(!(A(actortype, abilities)&(1<<A_A_MELEE))) return false;
        if(check && ((weapstate[W_MELEE] != W_S_PRIMARY && weapstate[W_MELEE] != W_S_SECONDARY) || millis-weaptime[W_MELEE] >= weapwait[W_MELEE])) return false;
        return true;
    }

    bool canmelee(int sweap, int millis, bool alt = false)
    {
        if(!hasmelee(millis, false)) return false;
        if(!canshoot(W_MELEE, alt ? HIT_ALT : 0, sweap, millis, (1<<W_S_RELOAD))) return false;
        return true;
    }

    int curfoot()
    {
        int foot = -1;
        vec fp = feetpos();

        float d0 = fabs(foottag(0).z - fp.z),
              d1 = fabs(foottag(1).z - fp.z);

        if(d0 < FOOTSTEP_DIST * curscale) foot = 0;
        else if(d1 < FOOTSTEP_DIST * curscale) foot = 1;

        return foot;
    }

    bool crouching(bool check = false)
    {
        if(!(A(actortype, abilities)&(1<<A_A_CROUCH))) return false;
        return action[AC_CROUCH] || (check && zradius > height);
    }

    bool running(float minspeed = 0)
    {
        if(minspeed != 0 && vel.magnitude() >= speed*0.5f*minspeed) return true;
        return sliding(true) || (!action[AC_WALK] && !crouching());
    }

    int slidetime(bool power = false)
    {
        if(G(impulseslidelen) && impulsetime[IM_T_SLIDE] && lastmillis-impulsetime[IM_T_SLIDE] <= G(impulseslidelen)) return impulsetime[IM_T_SLIDE];
        if(!power && G(impulsesliplen) && impulse[IM_SLIP] && lastmillis-impulse[IM_SLIP] <= G(impulsesliplen)) return impulse[IM_SLIP];
        return 0;
    }

    bool sliding(bool power = false)
    {
        if(G(impulseslidelen) && impulsetime[IM_T_SLIDE] && lastmillis-impulsetime[IM_T_SLIDE] <= G(impulseslidelen)) return true;
        if(!power && G(impulsesliplen) && impulse[IM_SLIP] && lastmillis-impulse[IM_SLIP] <= G(impulsesliplen)) return true;
        return false;
    }

    int zooming()
    {
        if(isweap(weapselect) && W2(weapselect, cooked, true)&W_C_ZOOM)
        {
            if(weapstate[weapselect] == W_S_ZOOM) return weaptime[weapselect];
            if(W2(weapselect, cooked, true)&W_C_KEEP && prevstate[weapselect] == W_S_ZOOM && action[AC_SECONDARY])
                return prevtime[weapselect];
        }
        return 0;
    }
};

enum { INANIMATE_NONE = 0, INANIMATE_RAIL, INANIMATE_MAX };
enum { INANIMATE_C_KILL = 0, INANIMATE_C_NOPASS, INANIMATE_C_MAX, INANIMATE_C_ALL = (1<<INANIMATE_C_KILL)|(1<<INANIMATE_C_NOPASS) };

struct passenger
{
    physent *ent;
    vec offset;
    bool local;

    passenger(physent *d = NULL, const vec &off = vec(0, 0, 0), bool loc = true) : ent(d), offset(off), local(loc) {}
    ~passenger() {}
};

struct inanimate : dynent
{
    int control, ent, coltype, curstep;
    float yawed, pitched;
    vec moved, resized;
    vector<passenger> passengers;
    physent *coltarget;

    inanimate() : control(INANIMATE_NONE), ent(-1), coltype(0), curstep(0), yawed(0), pitched(0), moved(0, 0, 0), resized(0, 0, 0), coltarget(NULL)
    {
        physent::reset();
        type = ENT_INANIMATE;
        state = CS_ALIVE;
    }

    ~inanimate()
    {
        removetrackedparticles(this);
        removetrackedsounds(this);
    }

    static bool is(int t) { return t == ENT_INANIMATE; }
    static bool is(physent *d) { return d && d->type == ENT_INANIMATE; }

    int findpassenger(physent *d)
    {
        loopv(passengers) if(passengers[i].ent == d) return i;
        return -1;
    }

    bool localpassenger(physent *d)
    {
        int n = findpassenger(d);
        if(n >= 0)
        {
            passenger &p = passengers[n];
            p.offset = vec(d->o).sub(o);
            return false;
        }
        passengers.add(passenger(d, vec(d->o).sub(o), true));
        return true;
    }

    bool remotepassenger(physent *d, const vec &offset)
    {
        int n = findpassenger(d);
        if(n >= 0)
        {
            passenger &p = passengers[n];
            p.offset = offset;
            p.local = false;
            return false;
        }
        passengers.add(passenger(d, offset, false));
        return true;
    }
};

struct projent : dynent
{
    projent *prev, *next;

    vec from, dest, norm, inertia, sticknrm, stickpos, effectpos, trailpos, lastgood;
    int addtime, lifetime, lifemillis, waittime, spawntime, fadetime, lastradial, lasteffect, lastbounce, beenused, extinguish, stuck;
    float movement, distance, lifespan, lifesize, speedmin, speedmax;
    bool local, limited, escaped, child, bounced;
    int projtype, projcollide, interacts;
    float elasticity, relativity, liquidcoast;
    int schan, id, weap, fromweap, fromflags, value, flags, collidezones;
    gameent *owner, *target, *stick;
    physent *hit;
    const char *mdlname;
    bvec material;
    fx::emitter *effect;
    int fxtype;

    projent() : projtype(PRJ_SHOT), id(-1), collidezones(CLZ_NONE), owner(NULL), target(NULL),
        stick(NULL), hit(NULL), mdlname(NULL), effect(NULL), fxtype(FX_P_NONE) { reset(); }
    ~projent()
    {
        if(owner) listremove(this, owner->projchain, prev, next);

        removetrackedparticles(this);
        removetrackedsounds(this);
        if(issound(schan)) soundsources[schan].clear();
        if(effect) effect->unhook();
        schan = -1;
    }

    static bool is(int t) { return t == ENT_PROJ; }
    static bool is(physent *d) { return d && d->type == ENT_PROJ; }
    static bool shot(int t, int w) { return t == ENT_PROJ && w == PRJ_SHOT; }
    static bool shot(physent *d) { return d && d->type == ENT_PROJ && ((projent*)d)->projtype == PRJ_SHOT; }

    void reset()
    {
        physent::reset();
        type = ENT_PROJ;
        state = CS_ALIVE;
        norm = vec(0, 0, 1);
        inertia = sticknrm = stickpos = lastgood = vec(0, 0, 0);
        effectpos = vec(-1e16f, -1e16f, -1e16f);
        addtime = lifetime = lifemillis = waittime = spawntime = fadetime = lastradial = lasteffect = lastbounce = beenused = flags = 0;
        schan = id = weap = fromweap = fromflags = value = -1;
        movement = distance = lifespan = speedmin = speedmax = 0;
        curscale = lifesize = 1;
        extinguish = stuck = interacts = 0;
        limited = escaped = child = bounced = false;
        projcollide = BOUNCE_GEOM|BOUNCE_PLAYER;
        material = bvec(255, 255, 255);
        if(effect) effect->unhook();
        fxtype = FX_P_NONE;
    }

    bool ready(bool used = true)
    {
        if(owner && (!used || projtype == PRJ_SHOT || !beenused) && waittime <= 0 && state != CS_DEAD)
            return true;
        return false;
    }

    bool usestuck()
    {
        if(projcollide&COLLIDE_SCAN) return false;
        return stuck != 0;
    }
};

struct cament
{
    enum { ENTITY = 0, PLAYER, AFFINITY, MAX };

    int cn, type, id, inview[MAX], lastinview[MAX], lastyawtime, lastpitchtime;
    vec o, dir;
    float dist, lastyaw, lastpitch;
    gameent *player;
    bool ignore, chase;

    cament(int p, int t) : cn(p), type(t), id(-1), o(0, 0, 0), dir(0, 0, 0), player(NULL), ignore(false), chase(true)
    {
        reset();
        resetlast();
    }
    cament(int p, int t, int n) : cn(p), type(t), id(n), o(0, 0, 0), dir(0, 0, 0), player(NULL), ignore(false), chase(true)
    {
        reset();
        resetlast();
    }
    cament(int p, int t, int n, const vec &d) : cn(p), type(t), id(n), o(0, 0, 0), dir(0, 0, 0), player(NULL), ignore(false), chase(true)
    {
        reset();
        resetlast();
        o = d;
    }
    cament(int p, int t, int n, const vec &c, gameent *d) : cn(p), type(t), id(n), o(0, 0, 0), dir(0, 0, 0), player(d), ignore(false), chase(true)
    {
        reset();
        resetlast();
        o = c;
    }
    ~cament() {}

    void reset()
    {
        loopi(MAX) inview[i] = lastinview[i] = 0;
        if(dir.iszero()) dir = vec(float(rnd(360)), float(rnd(91)-45));
    }

    void resetlast()
    {
        lastyawtime = lastpitchtime = totalmillis;
        lastyaw = lastpitch = 0;
    }

    static bool compare(const cament *a, const cament *b)
    {
        if(!a->ignore && b->ignore) return true;
        if(a->ignore && !b->ignore) return false;
        if(a->inview[cament::PLAYER] > b->inview[cament::PLAYER]) return true;
        if(a->inview[cament::PLAYER] < b->inview[cament::PLAYER]) return false;
        if(a->inview[cament::AFFINITY] > b->inview[cament::AFFINITY]) return true;
        if(a->inview[cament::AFFINITY] < b->inview[cament::AFFINITY]) return false;
        return !rnd(a->player ? 2 : 3);
    }
};

namespace client
{
    extern int showpresence, showpresencehostinfo, showteamchange, needsmap, gettingmap;
    extern bool demoplayback, isready, loadedmap;
    extern vector<uchar> messages;
    extern bool radarallow(gameent *d, vec &dir, float &dist);
    extern void clearvotes(gameent *d, bool msg = false);
    extern void ignore(int cn);
    extern void unignore(int cn);
    extern bool isignored(int cn);
    extern bool addmsg(int type, const char *fmt = NULL, ...);
    extern void saytext(gameent *f, gameent *t, int flags, char *text);
    extern void c2sinfo(bool force = false);
    extern bool haspriv(gameent *d, int priv = PRIV_NONE);
}

namespace physics
{
    extern int smoothmove, smoothdist, physframetime, grabstyle, grabplayerstyle;
    extern float getdeathplane();
    extern bool isghost(gameent *d, gameent *e, bool proj = false);
    extern float liquidmerge(physent *d, float from, float to);
    extern int carryaffinity(gameent *d);
    extern bool secondaryweap(gameent *d);
    extern float impulsevelocity(physent *d, float amt, int &cost, int type, float redir, vec &keep);
    extern bool movecamera(physent *d, const vec &dir, float dist, float stepdist);
    extern void smoothplayer(gameent *d, int res, bool local);
    extern void update();
    extern void reset();

#define GETLIQUIDFUNCS(name) \
    extern float get##name##buoyancy(int mat); \
    extern float get##name##buoyancyscale(int mat); \
    extern float get##name##falldist(int mat); \
    extern float get##name##fallspeed(int mat); \
    extern float get##name##fallpush(int mat); \
    extern float get##name##speed(int mat); \
    extern float get##name##speedscale(int mat); \
    extern float get##name##coast(int mat); \
    extern float get##name##coastscale(int mat); \
    extern float get##name##boost(int mat); \
    extern float get##name##boostscale(int mat); \
    extern float get##name##submerge(int mat); \
    extern float get##name##submergescale(int mat); \

    GETLIQUIDFUNCS(water)
    GETLIQUIDFUNCS(lava)

    extern float getwaterextinguish(int mat);
    extern float getwaterextinguishscale(int mat);
}
#define LIQUIDPHYS(name,mat) ((mat&MATF_VOLUME) == MAT_LAVA ? physics::getlava##name(mat)*physics::getlava##name##scale(mat) : physics::getwater##name(mat)*physics::getwater##name##scale(mat))
#define LIQUIDVAR(name,mat) ((mat&MATF_VOLUME) == MAT_LAVA ? physics::getlava##name(mat) : physics::getwater##name(mat))
#define WATERPHYS(name,mat) (physics::getwater##name(mat)*physics::getwater##name##scale(mat))

namespace projs
{
    extern vector<projent *> projs, collideprojs;

    extern void mapprojfx();
    extern void reset();
    extern void update();
    extern projent *create(const vec &from, const vec &to, bool local, gameent *d, int type, int fromweap, int fromflags, int lifetime, int lifemillis, int waittime, int speed, int id = 0, int weap = -1, int value = -1, int flags = 0, float scale = 1, bool child = false, gameent *target = NULL);
    extern void preload();
    extern void removeplayer(gameent *d);
    extern void destruct(gameent *d, int targ, int id, bool all = false);
    extern void sticky(gameent *d, int id, vec &norm, vec &pos, gameent *f = NULL);
    extern void shootv(int weap, int flags, int sub, int offset, float scale, vec &from, vec &dest, vector<shotmsg> &shots, gameent *d, bool local, gameent *v = NULL);
    extern void drop(gameent *d, int weap, int ent, int ammo = -1, bool local = true, int targ = -1, int index = 0, int count = 1);
    extern void render();
}

namespace weapons
{
    extern int slot(gameent *d, int n, bool back = false);
    extern vec *getweapsoundpos(gameent *d, int weaptag);
    extern bool weapselect(gameent *d, int weap, int filter, bool local = true);
    extern bool weapreload(gameent *d, int weap, int load = -1, int ammo = -1, int store = 0, bool local = true);
    extern void weapdrop(gameent *d, int w = -1);
    extern void checkweapons(gameent *d);
    extern float accmodspread(gameent *d, int weap, bool secondary, bool zooming);
    extern bool doshot(gameent *d, vec &targ, int weap, bool pressed = false, bool secondary = false, int force = 0, gameent *v = NULL);
    extern void shoot(gameent *d, vec &targ, int force = 0);
    extern void preload();
    extern bool canuse(int weap);
}

namespace hud
{
    extern char *chattex, *playertex, *deadtex, *waitingtex, *spectatortex, *editingtex, *dominatingtex, *dominatedtex, *inputtex,
        *pointtex, *flagtex, *bombtex, *arrowtex, *arrowrighttex, *arrowdowntex, *arrowlefttex, *alerttex, *questiontex, *flagdroptex,
        *flagtakentex, *bombdroptex, *bombtakentex, *attacktex, *warningtex, *indicatortex, *crosshairtex, *hithairtex,
        *spree1tex, *spree2tex, *spree3tex, *spree4tex, *multi1tex, *multi2tex, *multi3tex, *headshottex, *dominatetex, *revengetex,
        *firstbloodtex, *breakertex;
    extern int hudwidth, hudheight, hudsize, lastteam, damageresidue, damageresiduefade, radaraffinitynames, teamhurthud, teamhurttime, teamhurtdist, aboveheadui;
    extern float radaraffinityblend, radarblipblend, radaraffinitysize;
    extern bool scoreson, scoresoff, shownscores;
    extern vector<int> teamkills;
    extern const char *geticon(int type, int value);
    extern void drawindicator(int weap, int x, int y, int s);
    extern void drawpointertex(const char *tex, int x, int y, int s, float r = 1, float g = 1, float b = 1, float fade = 1);
    extern void drawpointer(int w, int h, int index);
    extern int numteamkills();
    extern void damage(int n, const vec &loc, gameent *v, int weap, int flags);
    extern void hit(int n, const vec &loc, gameent *v, int weap, int flags);
    extern void removeplayer(gameent *d);
    extern const char *teamtexname(int team = T_NEUTRAL);
    extern const char *itemtex(int type, int stype);
    extern const char *privtex(int priv = PRIV_NONE, int actortype = A_PLAYER);
    extern bool canshowscores();
    extern void showscores(bool on, bool interm = false, bool onauto = true, bool ispress = false);
    extern score &teamscore(int team);
    extern void resetscores();
    extern void cleanup();
}

enum { CTONE_TEAM = 0, CTONE_TONE, CTONE_TEAMED, CTONE_ALONE, CTONE_MIXED, CTONE_TMIX, CTONE_AMIX, CTONE_MAX };
namespace game
{
    extern int nextmode, nextmuts, lastzoom, lasttvcam, lasttvchg, spectvtime, waittvtime,
            maptime, mapstart, timeremaining, timeelapsed, timelast, timesync, bloodfade, bloodsize, bloodsparks, eventiconfade, eventiconshort, damageinteger,
            announcefilter, dynlighteffects, aboveheadnames, followthirdperson, nogore, forceplayermodel, forceplayerpattern,
            playerovertone, playerundertone, playerdisplaytone, playereffecttone, playerteamtone, follow, specmode, spectvfollow, clientcrc, aboveheaddead;
    extern float bloodscale, aboveitemiconsize, playerovertonelevel, playerundertonelevel, playerdisplaytonelevel, playereffecttonelevel, playerteamtonelevel,
            affinityfadeat, affinityfadecut, affinityfollowblend, affinitythirdblend, damagedivisor, damagecritical;
    extern bool zooming, wantsloadoutmenu;
    extern vec swaypush, swaydir;
    extern string clientmap;
    extern int attrmap[W_MAX];
    extern int showweapfx;

    extern gameent *player1, *focus;
    extern vector<gameent *> players, waiting;

    struct avatarent : dynent
    {
        avatarent() { type = ENT_CAMERA; }
    };
    extern avatarent avatarmodel, bodymodel;

    extern fx::FxHandle getweapfx(int type);
    extern bool needname(gameent *d);
    extern const char *vanityfname(gameent *d, int n, int head = -1, bool proj = false);
    extern void followswitch(int n, bool other = false);
    extern vector<cament *> cameras;
    extern gameent *newclient(int cn);
    extern gameent *getclient(int cn);
    extern gameent *intersectclosest(vec &from, vec &to, gameent *at);
    extern void clientdisconnected(int cn, int reason = DISC_NONE);
    extern const char *colourname(const char *name, int clientnum, int team, int actortype, int col, int privilege, int weapselect, bool icon = false, bool dupname = true, int colour = 3);
    extern const char *colourname(gameent *d, const char *name = NULL, bool icon = false, bool dupname = true, int colour = 3);
    extern const char *colourteam(int team, const char *icon = "");
    extern int findcolour(int team, int colour, int weapselect, bool tone = true, bool mix = false, float level = 1);
    extern int findcolour(gameent *d, bool tone = true, bool mix = false, float level = 1);
    extern int getcolour(gameent *d, int type = 0, float level = 1.f);
    extern void errorsnd(gameent *d);
    extern void specreset(gameent *d = NULL, bool clear = false);
    extern float opacity(gameent *d, bool third = true);
    extern void respawn(gameent *d);
    extern void respawned(gameent *d, bool local, int ent = -1);
    extern void spawneffect(int type, const vec &pos, float radius, int colour, float size);
    extern void impulseeffect(gameent *d, int effect = 0);
    extern void suicide(gameent *d, int flags = 0);
    extern void getyawpitch(const vec &from, const vec &pos, float &yaw, float &pitch);
    extern void scaleyawpitch(float &yaw, float &pitch, float targyaw, float targpitch, float yawspeed = 1, float pitchspeed = 1, float rotate = 0);
    extern bool allowmove(physent *d);
    extern void checkzoom();
    extern bool inzoom();
    extern float zoomscale();
    extern bool tvmode(bool check = true);
    extern void resetcamera(bool cam = true, bool input = true);
    extern void resetsway();
    extern void resetworld();
    extern void resetstate();
    extern void hiteffect(int weap, int flags, int damage, gameent *d, gameent *v, vec &dir, vec &vel, float dist, bool local = false);
    extern void damaged(int weap, int flags, int damage, int health, gameent *d, gameent *v, int millis, vec &dir, vec &vel, float dist);
    extern void killed(int weap, int flags, int damage, gameent *d, gameent *v, vector<gameent*> &log, int style, int material);
    extern void timeupdate(int state, int remain, int elapsed);
    extern void footstep(gameent *d, int curfoot = -1);
    #define RESIDUAL(name, type, pulse) extern void get##name##effect(physent *d, modelstate &mdl, int length, int millis, int delay);
    RESIDUALS
    #undef RESIDUAL
    extern void getplayermaterials(gameent *d, modelstate &mdl);
    extern void getplayereffects(gameent *d, modelstate &mdl);
    extern const char *getplayerstate(gameent *d, modelstate &mdl, int third = 1, float size = 1, int flags = 0, modelattach *mdlattach = NULL, int *lastoffset = NULL, bool vanitypoints = false);
    extern bool haloallow(gameent *d);
}

namespace entities
{
    extern vector<inanimate *> inanimates;
    extern int showentfull, showentweapons;
    extern float showentavailable, showentunavailable;
    extern void runrails();
    extern void updaterails();
    extern void localpassenger(inanimate *m, physent *d);
    extern inanimate *remotepassenger(int ent, physent *d, const vec &offset);
    extern void updatepassengers();
    extern inanimate *currentpassenger(physent *d);
    extern void announce(int idx, gameent *d = NULL, int chan = -1, int flags = 0);
    extern bool execitem(int n, int cn, dynent *d, float dist);
    extern bool collateitems(dynent *d, vec &pos, float radius, vector<actitem> &actitems);
    extern void checkitems(dynent *d);
    extern void putitems(packetbuf &p);
    extern void setspawn(int n, int m);
    extern bool tryspawn(dynent *d, const vec &o, float yaw = 0, float pitch = 0);
    extern void spawnplayer(gameent *d, int ent = -1, bool suicide = false);
    extern void useeffects(gameent *d, int cn, int ent, int ammoamt, bool spawn, int weap, int drop, int ammo = -1);
    extern void adddynlights();
    extern bool haloallow(int id, bool justtest = false);
    extern void render();
    extern void update();
    extern void reset();
}
#endif
#include "capture.h"
#include "defend.h"
#include "bomber.h"
#endif

#define GAMELOG_ENUM(pr, en) en(pr, Event, EVENT) en(pr, Message, MESSAGE) en(pr, Maximum, MAX)
ENUMNV(GAMELOG, GAMELOG_ENUM);

#define GAMELOG_F_ENUM(pr, en) en(pr, NONE, 0) en(pr, CLIENT1, 1<<0) en(pr, CLIENT2, 1<<1) en(pr, CLIENTN, 1<<2) en(pr, BROADCAST, 1<<3) en(pr, UNMAPPED, 1<<4) \
    en(pr, CLIENTS, pr##_CLIENT1|pr##_CLIENT2|pr##_CLIENTN) en(pr, ALL, pr##_CLIENT1|pr##_CLIENT2|pr##_CLIENTN|pr##_BROADCAST)
ENUMLI(GAMELOG_F, GAMELOG_F_ENUM);

#ifndef CPP_GAME_SERVER
#include "gamelog.h"
#ifdef CPP_GAME_MAIN
#define ENUM_DEFINE
#endif
#include "enum.h"
#endif
