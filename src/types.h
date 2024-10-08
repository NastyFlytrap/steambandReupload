/* File: types.h */

/*
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */


/*
 * Note that "char" may or may not be signed, and that "signed char"
 * may or may not work on all machines.  So always use "s16b" or "s32b"
 * for signed values.  Also, note that unsigned values cause math problems
 * in many cases, so try to only use "u16b" and "u32b" for "bit flags",
 * unless you really need the extra bit of information, or you really
 * need to restrict yourself to a single byte for storage reasons.
 *
 * Also, if possible, attempt to restrict yourself to sub-fields of
 * known size (use "s16b" or "s32b" instead of "int", and "byte" instead
 * of "bool"), and attempt to align all fields along four-byte words, to
 * optimize storage issues on 32-bit machines.  Also, avoid "bit flags"
 * since these increase the code size and slow down execution.  When
 * you need to store bit flags, use one byte per flag, or, where space
 * is an issue, use a "byte" or "u16b" or "u32b", and add special code
 * to access the various bit flags.
 *
 * Many of these structures were developed to reduce the number of global
 * variables, facilitate structured program design, allow the use of ascii
 * template files, simplify access to indexed data, or facilitate efficient
 * clearing of many variables at once.
 *
 * Note that certain data is saved in multiple places for efficient access,
 * and when modifying the data in one place it must also be modified in the
 * other places, to prevent the creation of inconsistant data.
 */



/**** Available Types ****/


/*
 * An array of 256 byte's
 */
typedef byte byte_256[256];

/*
 * An array of 256 u16b's
 */
typedef u16b u16b_256[256];


/*
 * An array of DUNGEON_WID byte's
 */
typedef byte byte_wid[DUNGEON_WID];

/*
 * An array of DUNGEON_WID s16b's
 */
typedef s16b s16b_wid[DUNGEON_WID];



/**** Available Structs ****/


typedef struct maxima maxima;
typedef struct info_entry info_entry;
typedef struct feature_type feature_type;
typedef struct object_kind object_kind;
typedef struct artifact_type artifact_type;
typedef struct ego_item_type ego_item_type;
typedef struct monster_blow monster_blow;
typedef struct monster_race monster_race;
typedef struct monster_lore monster_lore;
typedef struct vault_type vault_type;
typedef struct effect_type effect_type;
typedef struct object_type object_type;
typedef struct monster_type monster_type;
typedef struct alloc_entry alloc_entry;
typedef struct quest_type quest_type;
typedef struct owner_type owner_type;
typedef struct store_type store_type;
typedef struct magic_type magic_type;
typedef struct spell_book spell_book;
typedef struct player_sex player_sex;
typedef struct player_race player_race;
typedef struct player_class player_class;
typedef struct hist_type hist_type;
typedef struct player_other player_other;
typedef struct player_type player_type;
typedef struct start_item start_item;
typedef struct player_skills player_skills;
typedef struct part_data part_data;
typedef struct steamware steamware;
typedef struct high_score high_score;
typedef struct mind_type mind_type;
typedef struct mind_power mind_power;
typedef struct res_cap res_cap;


/**** Available structs ****/


/*
 * Information about maximal indices of certain arrays
 * Actually, these are not the maxima, but the maxima plus one
 */
struct maxima
{
	u32b fake_text_size;
	u32b fake_name_size;

	u16b f_max;		/* Max size for "f_info[]" */
	u16b k_max;		/* Max size for "k_info[]" */
	u16b a_max;		/* Max size for "a_info[]" */
	u16b e_max;		/* Max size for "e_info[]" */
	u16b r_max;		/* Max size for "r_info[]" */
	u16b v_max;		/* Max size for "v_info[]" */
	u16b p_max;		/* Max size for "p_info[]" */
	u16b h_max;		/* Max size for "h_info[]" */
	u16b x_max;		/* Max size for "x_info[]" */	
	u16b b_max;		/* Max size per element of "b_info[]" */
	u16b q_max;		/* Max size for "q_info[]" */
	u16b c_max;		/* Max size for "c_info[]" */

	u16b o_max;		/* Max size for "o_list[]" */
	u16b m_max;		/* Max size for "m_list[]" */
};

/*
 * Entries for spell/activation descriptions
 */
struct info_entry
{
	u16b	index;
	cptr	desc;
};


/*
 * Information about terrain "features"
 */
struct feature_type
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	byte mimic;			/* Feature to mimic */

	byte extra;			/* Extra byte (unused) */

	s16b unused;		/* Extra bytes (unused) */


	byte d_attr;		/* Default feature attribute */
	char d_char;		/* Default feature character */


	byte x_attr;		/* Desired feature attribute */
	char x_char;		/* Desired feature character */
};


/*
 * Information about object "kinds", including player knowledge.
 *
 * Only "aware" and "tried" are saved in the savefile
 */
struct object_kind
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	byte tval;			/* Object type */
	byte sval;			/* Object sub type */

	s16b pval;			/* Object extra info */
	u32b flags_pval;    /* Pval-dependant object flags */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to damage */
	s16b to_a;			/* Bonus to armor */
	s16b force;			/* Weapon Force */

	s16b ac;			/* Base armor */

	byte dd, ds;		/* Damage dice/sides */

	s16b weight;		/* Weight */

	s32b cost;			/* Object "base cost" */

	u32b flags1;		/* Flags, set 1 */
	u32b flags2;		/* Flags, set 2 */
	u32b flags3;		/* Flags, set 3 */

	s16b res[RS_MAX];	/* Percentile Resistances */

	byte locale[3];		/* Allocation level(s) */
	byte chance[3];		/* Allocation chance(s) */

	byte gen_mult_prob;     /* Probability of generating more than one */
	byte gen_dice;          /* Average number to generate - dice rolled */
	byte gen_side;          /* Average number to generate - dice sides */

	byte level;			/* Level */
	byte extra;			/* Something */


	byte d_attr;		/* Default object attribute */
	char d_char;		/* Default object character */


	byte x_attr;		/* Desired object attribute */
	char x_char;		/* Desired object character */


	byte flavor;		/* Special object flavor (or zero) */


	bool aware;			/* The player is "aware" of the item's effects */

	bool tried;			/* The player has "tried" one of the items */

	/* Gun data -CJN- */
	byte ammo_tval;        /* Ammunition TVAL */
	byte ammo_mult;        /* Gun multiplier */
	byte num_fire;         /* Magazine size */
	byte range;            /* Range */
	int degree;            /* Spread of the gun */

};



/*
 * Information about "artifacts".
 *
 * Note that the save-file only writes "cur_num" to the savefile.
 *
 * Note that "max_num" is always "1" (if that artifact "exists")
 */
struct artifact_type
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	byte tval;			/* Artifact type */
	byte sval;			/* Artifact sub type */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to damage */
	s16b to_a;			/* Bonus to armor */
	s16b force;			/* Weapon Force */


	s16b ac;			/* Base armor */

	byte dd, ds;		/* Damage when hits */

	s16b weight;		/* Weight */

	s32b cost;			/* Artifact "cost" */

	s16b pval1;               /* Special numeric value # 1 */
	u32b flags_pval1;         /* Artifact flags affected by this value */
	s16b pval2;               /* Special numeric value # 2 */
	u32b flags_pval2;         /* Artifact flags affected by this value */
	s16b pval3;               /* Special numeric value # 3 */
	u32b flags_pval3;         /* Artifact flags affected by this value */

	u32b flags1;		/* Artifact Flags, set 1 */
	u32b flags2;		/* Artifact Flags, set 2 */
	u32b flags3;		/* Artifact Flags, set 3 */

	s16b res[RS_MAX];			/* Percentile Resistances */

	byte level;			/* Artifact level */
	byte rarity;		/* Artifact rarity */

	byte cur_num;		/* Number created (0 or 1) */
	byte max_num;		/* Unused (should be "1") */

	byte activation;	/* Activation to use */
	u16b time;			/* Activation time */
	u16b randtime;		/* Activation time dice */
};


/*
 * Information about "ego-items".
 */
struct ego_item_type
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	byte slot;			/* Standard slot value */
	byte rating;		/* Rating boost */

	byte level;			/* Minimum level */
	byte rarity;		/* Object rarity */

	byte tval[3];		/* Legal tval */
	byte min_sval[3];	/* Minimum legal sval */
	byte max_sval[3];	/* Maximum legal tval */
	byte xtra;			/* Extra Sustain/Resist/Power */

	s16b max_to_h;		/* Maximum to-hit bonus */
	s16b max_to_d;		/* Maximum to-dam bonus */
	s16b max_to_a;		/* Maximum to-ac bonus */

	s16b max_pval1;           /* Special numeric value # 1 */
	u32b flags_pval1;         /* Ego-item flags affected by this value */
	s16b max_pval2;           /* Special numeric value # 2 */
	u32b flags_pval2;         /* Ego-item flags affected by this value */

	s16b res[RS_MAX];	/* Percentile Resistances */

	s32b cost;			/* Ego-item "cost" */

	u32b flags1;		/* Ego-Item Flags, set 1 */
	u32b flags2;		/* Ego-Item Flags, set 2 */
	u32b flags3;		/* Ego-Item Flags, set 3 */
};




/*
 * Monster blow structure
 *
 *	- Method (RBM_*)
 *	- Effect (RBE_*)
 *	- Damage Dice
 *	- Damage Sides
 */
struct monster_blow
{
	byte method;
	byte effect;
	byte d_dice;
	byte d_side;
};



/*
 * Monster "race" information, including racial memories
 *
 * Note that "d_attr" and "d_char" are used for MORE than "visual" stuff.
 *
 * Note that "x_attr" and "x_char" are used ONLY for "visual" stuff.
 *
 * Note that "cur_num" (and "max_num") represent the number of monsters
 * of the given race currently on (and allowed on) the current level.
 * This information yields the "dead" flag for Unique monsters.
 *
 * Note that "max_num" is reset when a new player is created.
 * Note that "cur_num" is reset when a new level is created.
 *
 * Maybe "x_attr", "x_char", "cur_num", and "max_num" should
 * be moved out of this array since they are not read from
 * "monster.txt".
 */
struct monster_race
{
	u32b name;				/* Name (offset) */
	u32b text;				/* Text (offset) */

	u16b hitpoints;         /* Monster hitpoints */
	s16b ac;				/* Armour Class */

	s16b sleep;				/* Inactive counter (base) */
	byte aaf;				/* Area affect radius (1-100) */
	byte speed;				/* Speed (normally 110) */

	s32b mexp;				/* Exp value for kill */

	byte freq_ranged;			/* Ranged attack frequency */

	byte mana;				/* Max mana */
	byte spell_power;		/* Power of (damage-dealing) spells */
	byte unused;         	/* Not currently used */

	u32b flags1;			/* Flags 1 (general) */
	u32b flags2;			/* Flags 2 (abilities) */
	u32b flags3;			/* Flags 3 (race/resist) */
	u32b flags4;			/* Flags 4 (inate/breath) */
	u32b flags5;			/* Flags 5 (normal spells) */
	u32b flags6;			/* Flags 6 (special spells) */
	u32b flags7;			/* Flags 7 (other) */
	u32b flags8;			/* Flags 8 (vulnerabilities) */

	monster_blow blow[4];	/* Up to four blows per round */

	byte level;				/* Level of creature */
	byte rarity;			/* Rarity of creature */

	byte d_attr;			/* Default monster attribute */
	char d_char;			/* Default monster character */

	byte x_attr;			/* Desired monster attribute */
	char x_char;			/* Desired monster character */

	byte max_num;			/* Maximum population allowed per level */
	byte cur_num;			/* Monster population on current level */

	u16b r_see;				/* Number of monsters of this type visible */
};


/*
 * Monster "lore" information
 *
 * Note that these fields are related to the "monster recall" and can
 * be scrapped if space becomes an issue, resulting in less "complete"
 * monster recall (no knowledge of spells, etc). XXX XXX XXX
 *
 * ToDo: The "r_" prefix is no longer needed and should be removed.
 */
struct monster_lore
{
	s16b r_sights;			/* Count sightings of this monster */
	s16b r_deaths;			/* Count deaths from this monster */

	s16b r_pkills;			/* Count monsters killed in this life */
	s16b r_tkills;			/* Count monsters killed in all lives */

	byte r_wake;			/* Number of times woken up (?) */
	byte r_ignore;			/* Number of times ignored (?) */

	byte r_xtra1;			/* Something (unused) */
	byte r_xtra2;			/* Something (unused) */

	byte r_drop_gold;		/* Max number of gold dropped at once */
	byte r_drop_item;		/* Max number of item dropped at once */

	byte r_ranged;			/* Observed ranged attacks */

	byte r_blows[4];		/* Number of times each blow type was seen */

	u32b r_flags1;			/* Observed racial flags */
	u32b r_flags2;			/* Observed racial flags */
	u32b r_flags3;			/* Observed racial flags */
	u32b r_flags4;			/* Observed racial flags */
	u32b r_flags5;			/* Observed racial flags */
	u32b r_flags6;			/* Observed racial flags */
	u32b r_flags7;			/* Observed racial flags */
	u32b r_flags8;			/* Observed racial flags */
};


/*
 * Information about "vault generation"
 */
struct vault_type
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	byte typ;			/* Vault type */

	byte rat;			/* Vault rating */

	byte hgt;			/* Vault height */
	byte wid;			/* Vault width */
};



/*
 * Information about effects.
 *
 *   The "index" is used to trigger any effect-type-specific code.  It is
 *      /not/ the same as the order of this effect in the effect array.
 *      The index need not be unique - you can have multiple "wall of fire"
 *      spells going at the same time.
 *   The "type" can be either the index of the projection type used in
 *      this effect, or a terrain type that this effect temporarily
 *      creates, or even correspond to multiple projection types and/or
 *      terrains.
 *   "y0" and "x0" are the coordinates of the source grid.
 *   The "time_count" is how many game turns have elapsed since this
 *      effect was last processed.  When it reaches zero, the effect is
 *      processed again, the "age" is incremented by one, and the "time_
 *      count" reset to the value stored in "time_delay".
 *   The "age" is how old this effect is; special functions use this
 *      variable to make the effect do different things over time,
 *      including "dying".
 *   The "angle_dir" is the angular direction from the source of the effect
 *      to the target grid, expressed in degrees divided by 2.  See the
 *      table "get_angle_to_grid".
 *   The "power" is how strong this effect is.  This is usually, but
 *      not always, the same as base damage.
 *   The "power2" is often used for radius or effectiveness.
 *   The "flags" store bitflags corresponding to effect state; they serve
 *      (with the "age") as the effect memory.  Unlike monster or object
 *      bitflags, these need not have fixed meanings.
 */
struct effect_type
{
	s16b index;             /* Numerical index or classifier */
	s16b type;              /* Projection (or terrain) type */

	byte y0;                /* Source location */
	byte x0;

	byte y1;                /* Target location */
	byte x1;

	s16b time_count;        /* Game turns until effect takes its turn */
	s16b time_delay;        /* Number of game turns between effect turns */

	s16b age;               /* Number of turns effect has lasted */
	s16b lifespan;          /* Number of turns effect can last */

	s16b power;             /* Strength of effect */
	s16b power2;

	byte practice_skill;   	/* Skill this effect practices when it hurts or
	                           kills monsters */
	byte unused;
	u16b unused2;

	u16b flags;             /* Effect "memory" bitflags */
};

/*
 * Object information, for a specific object.
 *
 * Note that a "discount" on an item is permanent and never goes away.
 *
 * Note that inscriptions are now handled via the "quark_str()" function
 * applied to the "note" field, which will return NULL if "note" is zero.
 *
 * Note that "object" records are "copied" on a fairly regular basis,
 * and care must be taken when handling such objects.
 *
 * Note that "object flags" must now be derived from the object kind,
 * the artifact and ego-item indexes, and the two "xtra" fields.
 *
 * Each cave grid points to one (or zero) objects via the "o_idx"
 * field (above).  Each object then points to one (or zero) objects
 * via the "next_o_idx" field, forming a singly linked list, which
 * in game terms, represents a "stack" of objects in the same grid.
 *
 * Each monster points to one (or zero) objects via the "hold_o_idx"
 * field (below).  Each object then points to one (or zero) objects
 * via the "next_o_idx" field, forming a singly linked list, which
 * in game terms, represents a pile of objects held by the monster.
 *
 * The "held_m_idx" field is used to indicate which monster, if any,
 * is holding the object.  Objects being held have "ix=0" and "iy=0".
 */
struct object_type
{
	s16b k_idx;			/* Kind index (zero if "dead") */

	byte iy;			/* Y-position on map, or zero */
	byte ix;			/* X-position on map, or zero */

	byte tval;			/* Item type (from kind) */
	byte sval;			/* Item sub-type (from kind) */

	s16b pval;                /* Special numeric value # 1 */
	u32b flags_pval1;         /* Flags affected by this value */

	s16b pval2;               /* Special numeric value # 2 */
	u32b flags_pval2;         /* Flags affected by this value */

	s16b pval3;               /* Special numeric value # 3 */
	u32b flags_pval3;         /* Flags affected by this value */


	u32b flags1;              /* Non pval-dependant flags, set 1 */
	u32b flags2;              /* Non pval-dependant flags, set 2 */
	u32b flags3;              /* Non pval-dependant flags, set 3 */

	byte discount;		/* Discount (if any) */

	byte number;		/* Number of items */

	s16b weight;		/* Item weight */

	s16b name1;			/* Artifact type, if any */
	s16b name2;			/* Ego-Item type, if any */

	byte xtra1;			/* Extra info type */
	byte xtra2;			/* Extra info index */

	s16b to_h;			/* Plusses to hit */
	s16b to_d;			/* Plusses to damage */
	s16b to_a;			/* Plusses to AC */
	s16b force;			/* force */

	s16b ac;			/* Normal AC */

	byte dd, ds;		/* Damage dice/sides */

	s16b timeout;		/* Timeout Counter */

	s16b ident;			/* Special flags */

	byte marked;		/* Object is marked */

	u16b note;			/* Inscription index */

	byte inscrip;		/* INSCRIP_XXX constant */

	s16b next_o_idx;	/* Next object in stack (if any) */

	s16b held_m_idx;	/* Monster holding us (if any) */

	/* Gun data -CJN- */
	byte ammo_tval;        /* Ammunition TVAL */
	byte ammo_mult;        /* Gun multiplier */
	byte num_fire;         /* Magazine size */
	byte range;            /* Range */
	int degree;            /* Spread of the gun */


};



/*
 * Monster information, for a specific monster.
 *
 * Note: fy, fx constrain dungeon size to 256x256
 *
 * The "hold_o_idx" field points to the first object of a stack
 * of objects (if any) being carried by the monster (see above).
 */
struct monster_type
{
	s16b r_idx;			/* Monster race index */

	byte fy;			/* Y location on map */
	byte fx;			/* X location on map */

	s16b hp;			/* Current Hit points */
	s16b maxhp;			/* Max Hit points */

	s16b csleep;		/* Inactive counter */
	byte stasis;			/* Monster being held */

	byte mspeed;		/* Monster "speed" */
	byte energy;		/* Monster "energy" */

	byte stunned;		/* Monster is stunned */
	byte confused;		/* Monster is confused */
	byte monfear;		/* Monster is afraid */

	byte cdis;			/* Current dis from player */

	u16b mflag;			/* Extra monster flags */

	bool ml;			/* Monster is "visible" */

	s16b hold_o_idx;	/* Object being held (if any) */

	u32b smart;			/* Field for "smart_learn" */

	byte ty;				/* Monster target */
	byte tx;

	/* Harrassment spells are more likely early in a battle */
	byte harass;

	byte min_range;		/* What is the closest we want to be? */  /* Not saved */
	byte best_range;	/* How close do we want to be? */  /* Not saved */

	byte mana;			/* Current mana level */

};




/*
 * An entry for the object/monster allocation functions
 *
 * Pass 1 is determined from allocation information
 * Pass 2 is determined from allocation restriction
 * Pass 3 is determined from allocation calculation
 */
struct alloc_entry
{
	s16b index;		/* The actual index */

	byte level;		/* Base dungeon level */
	byte prob1;		/* Probability, pass 1 */
	byte prob2;		/* Probability, pass 2 */
	byte prob3;		/* Probability, pass 3 */

	u16b total;		/* Unused for now */
};



/*
 * Structure for the "quests"
 */
struct quest_type
{
	u32b name;			/* Name (offset) */

	byte type;			/* Quest Type */
	byte reward;		/* Quest Reward */

	byte active_level;	/* Equals dungeon level if not completed, 0 if completed */
	byte base_level;	/* The dungeon level on which the quest was assigned*/

	s16b mon_idx;		/* Monster race/unique */

	s16b cur_num;		/* Number killed */
	s16b max_num;		/* Number required */

	bool started;		/* Has the player start the quest */
};



/*
 * A store owner
 */
struct owner_type
{
	u32b owner_name;	/* Name (offset) */

	s16b max_cost;		/* Purse limit */

	byte max_inflate;	/* Inflation (max) */
	byte min_inflate;	/* Inflation (min) */

	byte haggle_per;	/* Haggle unit */

	byte insult_max;	/* Insult limit */

	byte owner_race;	/* Owner race */
};




/*
 * A store, with an owner, various state flags, a current stock
 * of items, and a table of items that are often purchased.
 */
struct store_type
{
	byte owner;				/* Owner index */

	s16b insult_cur;		/* Insult counter */

	s16b good_buy;			/* Number of "good" buys */
	s16b bad_buy;			/* Number of "bad" buys */

	s32b store_open;		/* Closed until this turn */

	s16b table_num;			/* Table -- Number of entries */
	s16b table_size;		/* Table -- Total Size of Array */
	s16b *table;			/* Table -- Legal item kinds */

	s16b stock_num;			/* Stock -- Number of entries */
	s16b stock_size;		/* Stock -- Total Size of Array */
	object_type *stock;		/* Stock -- Actual stock items */
};



/*
 * The "name" of spell 'N' is stored as spell_names[X][N],
 * where X is 0 for mage-spells and 1 for priest-spells.
 */
struct magic_type
{
	u16b index;			/* The internal spell index. */
	cptr sname;			/* The name of the spell in the spellbook */
	byte slevel;		/* Required level (to learn) */
	s16b smana;			/* Required mana (to cast) */
	byte sfail;			/* Minimum chance of failure */
};



/* 
 * Information about a spell book's contents
 */
struct spell_book
{
	byte flags;		/* Spellbook Flags */

	magic_type contents[MAX_BOOK_SPELLS];
};



/*
 * Player sex info
 */
struct player_sex
{
	cptr title;			/* Type of sex */

	cptr winner;		/* Name of winner */
};


/*
 * Player racial info
 */
struct player_race
{
	u32b name;			/* Name (offset) */
	u32b text;			/* Text (offset) */

	s16b r_adj[A_MAX];	/* Racial stat bonuses */
	byte r_mhp;			/* Race hit-dice modifier */
	byte r_exp;			/* Race experience factor */

	byte b_age;			/* base age */
	byte m_age;			/* mod age */

	s16b m_b_ht;		/* base height (males) */
	s16b m_m_ht;		/* mod height (males) */
	s16b m_b_wt;		/* base weight (males) */
	s16b m_m_wt;		/* mod weight (males) */

	s16b f_b_ht;		/* base height (females) */
	s16b f_m_ht;		/* mod height (females) */
	s16b f_b_wt;		/* base weight (females) */
	s16b f_m_wt;		/* mod weight (females) */

	byte infra;			/* Infra-vision	range */

	u32b choice;		/* Legal class choices */

	s16b hist;			/* Starting history index */

	u32b flags1;		/* Racial Flags, set 1 */
	u32b flags2;		/* Racial Flags, set 2 */
	u32b flags3;		/* Racial Flags, set 3 */
};


/*
 * Starting equipment entry
 */
struct start_item
{
	byte tval;	/* Item's tval */
	byte sval;	/* Item's sval */
	s16b pval;  /* Item's pval */
	byte min;	/* Minimum starting amount */
	byte max;	/* Maximum starting amount */
};


struct player_skills
{
	char *name;			/* Name of the skill */
	s16b skill_rank;	/* current modified rank of skill */
	s16b skill_max;		/* maximum internal (character) rank of skill */
	s16b skill_raise;	/* Helps slow down skill gain */
	s16b skill_index;	/* Index of skill */
	s16b skill_type;	/* Type of skill, used for formatting */
	char *description;  /* description of skill */
};

struct part_data
{
	char *name;
	s32b cost;
	s32b install_price;
	s32b research_time;
};

struct steamware
{
	byte body_part;
	
	part_data data[MAX_UPGRADE];
};

/*
 * Player class info
 */
struct player_class
{
	u32b name;			/* Name (offset) */

	u32b mtitle[10];		/* Titles - offset */
	u32b ftitle[10];		/* Titles - offset */

	s16b c_adj[A_MAX];	/* Class stat modifier */

	s16b c_mhp;			/* Class hit-dice adjustment */
	s16b c_exp;			/* Class experience factor */

	u32b flags;			/* Class Flags */

	u16b max_attacks;	/* Maximum possible attacks */
	u16b min_weight;	/* Minimum weapon weight for calculations */
	u16b att_multiply;	/* Multiplier for attack calculations */

	u16b spell_stat;
	s16b spell_weight;						/* Weight that hurts spells */
	bool spell_book[SV_BOOK_MAX];			/* The list of legal books */
	s16b spell_handicap[SV_BOOK_MAX];		/* Spell handicap per realm */

	u32b sense_base;	/* Base pseudo-id value */
	u16b sense_div;		/* Pseudo-id divisor */
	byte pet_upkeep_div; /* Pet upkeep divider */

	

	start_item start_items[MAX_START_ITEMS];/* The starting inventory */

/*	player_magic spells;  Magic spells */
};


/*
 * Player background information
 */
struct hist_type
{
	u32b text;			    /* Text (offset) */

	byte roll;			    /* Frequency of this entry */
	byte chart;			    /* Chart index */
	byte next;			    /* Next chart index */
	byte bonus;			    /* Social Class Bonus + 50 */
};



/*
 * Some more player information
 *
 * This information is retained across player lives
 */
struct player_other
{
	char full_name[32];		/* Full name */
	char base_name[32];		/* Base name */

	bool opt[OPT_MAX];		/* Options */

	u32b window_flag[ANGBAND_TERM_MAX];	/* Window flags */

	byte hitpoint_warn;		/* Hitpoint warning (0 to 9) */

	byte delay_factor;		/* Delay factor (0 to 9) */
};


/*
 * Most of the "player" information goes here.
 *
 * This stucture gives us a large collection of player variables.
 *
 * This entire structure is wiped when a new character is born.
 *
 * This structure is more or less laid out so that the information
 * which must be saved in the savefile precedes all the information
 * which can be recomputed as needed.
 */
struct player_type
{
	s16b py;			/* Player location */
	s16b px;			/* Player location */

	byte psex;			/* Sex index */
	byte prace;			/* Race index */
	byte pclass;		/* Class index */
	byte oops;			/* Unused */

	byte hitdie;		/* Hit dice (sides) */
	byte expfact;		/* Experience factor */

	s16b age;			/* Characters age */
	s16b ht;			/* Height */
	s16b wt;			/* Weight */
	s16b sc;			/* Social Class */

	u16b fame;			/* Fame - used for quests */

	s32b au;			/* Current Gold */
	
	/* Generation fields (for quick start) */
	s32b au_birth;			/* Current Gold */
	s16b stat_birth[A_MAX];	/* Current "natural" stat values */

	s16b max_depth;		/* Max depth */
	s16b depth;			/* Cur depth */

	s16b max_lev;		/* Max level */
	s16b lev;			/* Cur level */

	s32b max_exp;		/* Max experience */
	s32b exp;			/* Cur experience */
	u16b exp_frac;		/* Cur exp frac (times 2^16) */
	s16b free_skpts;	/* Skill points remaining to spend */
	s16b free_sgain;	/* Number of Statgains remaining */
	player_skills skills[N_SKILLS]; /* Player Skills */

	s16b mhp;			/* Max hit pts */
	s16b chp;			/* Cur hit pts */
	u16b chp_frac;		/* Cur hit frac (times 2^16) */

	s16b mwp;			/* Max wound pts */
	s16b cwp;			/* Cur wound pts */
	u16b cwp_frac;		/* Cur wound frac (times 2^16) */

	s16b msp;			/* Max mana pts */
	s16b csp;			/* Cur mana pts */
	u16b csp_frac;		/* Cur mana frac (times 2^16) */

	s16b stat_max[A_MAX];	/* Current "maximal" stat values */
	s16b stat_cur[A_MAX];	/* Current "natural" stat values */

	s16b fast;			/* Timed -- Fast */
	s16b slow;			/* Timed -- Slow */
	s16b blind;			/* Timed -- Blindness */
	s16b paralyzed;		/* Timed -- Paralysis */
	s16b confused;		/* Timed -- Confusion */
	s16b afraid;		/* Timed -- Fear */
	s16b image;			/* Timed -- Hallucination */
	s16b poisoned;		/* Timed -- Poisoned */
	s16b cut;			/* Timed -- Cut */
	s16b stun;			/* Timed -- Stun */

	s16b protevil;		/* Timed -- Protection */
	s16b invuln;		/* Timed -- Invulnerable */
	s16b hero;			/* Timed -- Heroism */
	s16b shero;			/* Timed -- Super Heroism */
	s16b shield;		/* Timed -- Shield Spell */
	s16b blessed;		/* Timed -- Blessed */
	s16b tim_invis;		/* Timed -- See Invisible */
	s16b tim_infra;		/* Timed -- Infra Vision */
	s16b tim_wraith;	/* Timed -- Wraith Form */
	s16b tim_esp;	    /* Timed ESP */
	s16b tim_harding;	 /* Timed Automata Armor */
	s16b tim_invisiblity;/* Timed -- invisiblity */
	s16b tim_light;		 /* Timed -- Light radius */
	s16b tim_demonspell; /* Timed -- Demonic spellpower */
	s16b tim_demonhealth; /* Timed -- Demonic Health */
	s16b tim_wormsense; 	/* Timed -- Wormsense */
	s16b tim_voorish;		/* Timed -- Voorish spell skill */
	s16b tim_stygian;		/* Timed -- Stygian war skill */
	s16b tim_muscle;		 /* Timed -- Muscle buff */
	s16b tim_vigor;			/* Timed -- Vigor buff */
	s16b tim_no_tele;		/* Timed -- No Teleport */
	s16b tim_free_act;		/* Timed -- Free Action */
	s16b tim_anti_magic;	/* Timed -- Anti Magic */
	s16b tim_evade;			/* Timed -- evasion */
				
	s16b tim_res[RS_MAX];/* Timed resistancs */

	u32b muta1;			/* Mutation flags 1 */
	u32b muta2;			/* Mutation flags 2 */
	u32b muta3;			/* Mutation flags 3 */
	u32b muta4;			/* Mutation flags 4 */
	u32b muta5;			/* Mutation flags 5 */
	u32b muta6;			/* Mutation flags 6 */

	s16b word_recall;	/* Word of recall counter */

	s16b energy;		/* Current energy */

	s16b food;			/* Current nutrition */

	byte confusing;		/* Glowing hands */
	byte searching;		/* Currently searching */

	s16b base_wakeup_chance;	/* Base amount of character noise -- NOT SAVED! */

	s16b player_hp[PY_MAX_LEVEL];	/* HP Array */

	char died_from[80];		/* Cause of death */
	char history[4][60];	/* Initial history */

	u16b total_winner;		/* Total winner */
	u16b panic_save;		/* Panic save */

	u16b noscore;			/* Cheating flags */

	bool is_dead;			/* Player is dead */

	bool wizard;			/* Player is in wizard mode */


	/*** Temporary fields ***/

	bool playing;			/* True if player is playing */

	bool leaving;			/* True if player is leaving */

	bool create_up_stair;	/* Create up stair on next level */
	bool create_down_stair;	/* Create down stair on next level */

	s16b wy;				/* Dungeon panel */
	s16b wx;				/* Dungeon panel */

	s32b total_weight;		/* Total weight being carried */

	s16b inven_cnt;			/* Number of items in inventory */
	s16b equip_cnt;			/* Number of items in equipment */

	s16b target_set;		/* Target flag */
	s16b target_who;		/* Target identity */
	s16b target_row;		/* Target location */
	s16b target_col;		/* Target location */

	s16b health_who;		/* Health bar trackee */

	s16b monster_race_idx;	/* Monster race trackee */

	s16b object_kind_idx;	/* Object kind trackee */

	s16b energy_use;		/* Energy use this turn */

	s16b resting;			/* Resting counter */
	s16b running;			/* Running counter */

	s16b run_cur_dir;		/* Direction we are running */
	s16b run_old_dir;		/* Direction we came from */
	bool run_unused;		/* Unused (padding field) */
	bool run_open_area;		/* Looking for an open area */
	bool run_break_right;	/* Looking for a break (right) */
	bool run_break_left;	/* Looking for a break (left) */

	s16b command_cmd;		/* Gives identity of current command */
	s16b command_arg;		/* Gives argument of current command */
	s16b command_rep;		/* Gives repetition of current command */
	s16b command_dir;		/* Gives direction of current command */

	s16b command_see;		/* See "cmd1.c" */
	s16b command_wrk;		/* See "cmd1.c" */

	s16b command_new;		/* Hack -- command chaining XXX XXX */

	bool old_cumber_armor;
	bool old_cumber_glove;
	bool old_heavy_wield;
	bool old_heavy_shoot;
	bool old_icky_wield;

	s16b old_lite;		/* Old radius of lite (if any) */
	s16b old_view;		/* Old radius of view (if any) */

	s16b old_food_aux;	/* Old value of food */

	bool cumber_armor;	/* Mana draining armor */
	bool cumber_glove;	/* Mana draining gloves */
	bool heavy_wield;	/* Heavy weapon */
	bool heavy_shoot;	/* Heavy shooter */
	bool icky_wield;	/* Icky weapon */

	s16b cur_lite;		/* Radius of lite (if any) */

	s16b extrahp;		/* Hit points to be added on for heroism, berserk */

	u32b notice;		/* Special Updates (bit flags) */
	u32b update;		/* Pending Updates (bit flags) */
	u32b redraw;		/* Normal Redraws (bit flags) */
	u32b window;		/* Window Redraws (bit flags) */

	s16b stat_use[A_MAX];	/* Current modified stats */
	s16b stat_top[A_MAX];	/* Maximal modified stats */

	/*** Extracted fields ***/

	s16b stat_add[A_MAX];	/* Equipment stat bonuses */

	s16b res[RS_MAX];	/* Percentile Resistances */
	s16b dis_res[RS_MAX];	/* Percentile Resistances (display)*/

	bool wormsense;		/* Tremorsense */
	
	bool resist_fear;	/* Resist fear */
	bool resist_blind;	/* Resist blindness */
	bool resist_confu;	/* Resist confusion */

	bool sustain_mus;	/* Keep strength */
	bool sustain_agi;	/* Keep dexterity */
	bool sustain_vig;	/* Keep constitution */
	bool sustain_sch;	/* Keep intelligence */
	bool sustain_ego;	/* Keep wisdom */
	bool sustain_chr;	/* Keep charisma */

	bool slow_digest;	/* Slower digestion */
	bool ffall;			/* Feather falling */
	bool lite;			/* Permanent light */
	bool flicker;		/* Are we weilding flame? */
	bool regenerate_25;	/* Regeneration */
	bool regenerate_50;	/* Regeneration */
	bool regenerate_75;	/* Regeneration */
	bool telepathy;		/* Telepathy */
	bool see_inv;		/* See invisible */
	bool free_act;		/* Free action */
	bool hold_life;		/* Hold life */
	bool wraith_form;	/* Wraithform */
	bool counter;		/* Counter-strike */
	bool invisiblity;	/* Invisiblity */

	bool sh_fire;		/* Fiery 'immolation' effect */
	bool sh_elec;		/* Electric 'immolation' effect */
	bool sh_spine;		/* Spiny Skin */
	
	bool impact;		/* Earthquake blows */
	bool aggravate;		/* Aggravate monsters */
	bool teleport;		/* Random teleporting */
	bool no_teleport;	/* No teleportation */
	bool no_magic;		/* No spellcasting */
	bool exp_drain;		/* Experience draining */
	bool hp_drain;		/* Drains HP */
	bool sp_drain;		/* Drains SP */
	bool item_drain;	/* Drains charges */
	bool spell_disrupt;	/* Disrupt casting */
	bool anti_magic;	/* Anti-Magic Shell */

	bool bless_blade;	/* Blessed blade */

	s16b dis_to_h;		/* Known bonus to hit */
	s16b dis_to_d;		/* Known bonus to dam */
	s16b dis_to_a;		/* Known bonus to ac */

	s16b dis_ac;		/* Known base ac */

	s16b to_h;			/* Bonus to hit */
	s16b to_d;			/* Bonus to dam */
	s16b to_a;			/* Bonus to ac */

	s16b ac;			/* Base ac */

	s16b see_infra;		/* Infravision range */
	s16b mana_bonus;	/* Bonus to mana from equipment */
	s16b health_bonus;	/* Bonus to health from equipment */

	s16b skill_sav;			/* Skill: Saving throw */
	s16b skill_stl;			/* Skill: Stealth factor */
	s16b skill_dig;			/* Skill: Digging */

	s16b num_blow;		/* Number of blows */
	s16b num_fire;		/* Number of shots */

	byte ammo_mult;		/* Ammo multiplier */

	byte ammo_tval;		/* Ammo variety */

	s16b pspeed;		/* Current speed */
	
	/*** Steamware Data ***/
	s32b eyes_research;		/* Time left on eye research */
	byte eyes_level;		/* Current (abcd)eye level */
	s32b reflex_research;	/* Time left on reflex research */
	byte reflex_level;		/* Current (abcd)reflex level */
	s32b plate_research;	/* Time left on plate research */
	byte plate_level;		/* Current (abcd)plate level */
	s32b core_research;		/* Time left on core research */
	byte core_level;		/* Current (abcd)core level */
	s32b spur_research;		/* Time left on spur research */
	byte spur_level;		/* Current (abcd)spur level */
	
	/*** Pet commands ***/
	s16b pet_follow_distance; /* Length of the imaginary "leash" for pets */
	bool pet_open_doors;      /* flag - allow pets to open doors */
	bool pet_pickup_items;    /* flag - allow pets to pickup items */

	byte wonderland;		/* You're a ghost if true -- Gumband */
	byte was_wonderland;	/* You used to be a ghost -- Gumband */
	byte wonderland_start;	/* For generating the starting stairs - Gumband */
	
	/* NOT SAVED!! */
	byte vulnerability;	/* Used to make animal packs charge and retreat */

	u16b cur_quest;		/* Current quest */

	u16b max_seen_r_idx;	/* Most powerful monster visible */

	bool monster_mem_fmt; /* Monster memory format, TRUE = table */
};


/*
 * Semi-Portable High Score List Entry (128 bytes)
 *
 * All fields listed below are null terminated ascii strings.
 *
 * In addition, the "number" fields are right justified, and
 * space padded, to the full available length (minus the "null").
 *
 * Note that "string comparisons" are thus valid on "pts".
 */


struct high_score
{
	char what[8];		/* Version info (string) */

	char pts[10];		/* Total Score (number) */

	char gold[10];		/* Total Gold (number) */

	char turns[10];		/* Turns Taken (number) */

	char day[10];		/* Time stamp (string) */

	char who[16];		/* Player Name (string) */

	char uid[8];		/* Player UID (number) */

	char sex[2];		/* Player Sex (string) */
	char p_r[3];		/* Player Race (number) */
	char p_c[3];		/* Player Class (number) */

	char cur_lev[4];		/* Current Player Level (number) */
	char cur_dun[4];		/* Current Dungeon Level (number) */
	char max_lev[4];		/* Max Player Level (number) */
	char max_dun[4];		/* Max Dungeon Level (number) */

	char how[32];		/* Method of death (string) */
};

/* class craft */
struct mind_type
{
	int     min_lev;
	int     mana_cost;
	int     fail;
	cptr    name;
};

struct mind_power
{
	mind_type info[MAX_CLASS_POWERS];
};

/*
 * Information about resistance caps
 */
struct res_cap
{
	byte normal; /* Normal cap */
	byte temp;	 /* Cap including temporary resist */
};

