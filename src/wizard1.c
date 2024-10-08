/* File: wizard1.c */

/*
 * Copyright (c) 1997 Ben Harrison, and others
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */

#include "angband.h"


#ifdef ALLOW_SPOILERS


/*
 * The spoiler file being created
 */
static FILE *fff = NULL;



/*
 * Extract a textual representation of an attribute
 */
static cptr attr_to_text(byte a)
{
	switch (a)
	{
		case TERM_DARK:    return ("xxx");
		case TERM_WHITE:   return ("White");
		case TERM_SLATE:   return ("Slate");
		case TERM_ORANGE:  return ("Orange");
		case TERM_RED:     return ("Red");
		case TERM_GREEN:   return ("Green");
		case TERM_BLUE:    return ("Blue");
		case TERM_UMBER:   return ("Umber");
		case TERM_L_DARK:  return ("L.Dark");
		case TERM_L_WHITE: return ("L.Slate");
		case TERM_VIOLET:  return ("Violet");
		case TERM_YELLOW:  return ("Yellow");
		case TERM_L_RED:   return ("L.Red");
		case TERM_L_GREEN: return ("L.Green");
		case TERM_L_BLUE:  return ("L.Blue");
		case TERM_L_UMBER: return ("L.Umber");
	}

	/* Oops */
	return ("Icky");
}



/*
 * A tval grouper
 */
typedef struct
{
	byte tval;
	cptr name;
} grouper;



/*
 * Item Spoilers by Ben Harrison (benh@phial.com)
 */


/*
 * The basic items categorized by type
 */
static const grouper group_item[] =
{
	{ TV_AMMO,		"Ammo" },
	{ TV_BULLET,		  NULL },
	{ TV_SHOT,		  NULL },

	{ TV_GUN,		"Guns" },

	{ TV_SWORD,		"Weapons" },
	{ TV_POLEARM,	  NULL },
	{ TV_HAFTED,	  NULL },
	{ TV_DAGGER,	  NULL },
	{ TV_AXES,		  NULL },
	{ TV_BLUNT,		  NULL },
	{ TV_DIGGING,	  NULL },

	{ TV_SOFT_ARMOR,	"Armour (Body)" },
	{ TV_HARD_ARMOR,	  NULL },
	{ TV_DRAG_ARMOR,	  NULL },

	{ TV_CLOAK,		"Armour (Misc)" },
	{ TV_LEG,		  NULL },
	{ TV_HELM,		  NULL },
	{ TV_CROWN,		  NULL },
	{ TV_GLOVES,	  NULL },
	{ TV_BOOTS,		  NULL },

	{ TV_AMULET,	"Amulets" },
	{ TV_RING,		"Rings" },

	{ TV_MECHANISM,	"Mechanisms" },
	{ TV_TEXT,		"Parchments" },
	{ TV_TONIC,	"tonics" },
	{ TV_FOOD,		"Food" },

	{ TV_APPARATUS,		"Apparatuses" },
	{ TV_RAY,		"Ray guns" },
	{ TV_TOOL,		"Tools" },

	{ TV_MAGIC_BOOK,	"Powers" },

	{ TV_CHEST,		"Chests" },

	{ TV_SPIKE,		"Various" },
	{ TV_LITE,		  NULL },
	{ TV_FLASK,		  NULL },
	{ TV_JUNK,		  NULL },
	{ TV_BOTTLE,	  NULL },
	{ TV_SKELETON,	  NULL },

	{ 0, "" }
};





/*
 * Describe the kind
 */
static void kind_info(char *buf, char *dam, char *wgt, int *lev, s32b *val, int k)
{
	object_kind *k_ptr;

	object_type *i_ptr;
	object_type object_type_body;


	/* Get local object */
	i_ptr = &object_type_body;

	/* Prepare a fake item */
	object_prep(i_ptr, k);

	/* Obtain the "kind" info */
	k_ptr = &k_info[i_ptr->k_idx];

	/* It is known */
	i_ptr->ident |= (IDENT_KNOWN);

	/* Cancel bonuses */
	i_ptr->pval = 0;
	i_ptr->to_a = 0;
	i_ptr->to_h = 0;
	i_ptr->to_d = 0;


	/* Level */
	(*lev) = k_ptr->level;

	/* Value */
	(*val) = object_value(i_ptr);


	/* Hack */
	if (!buf || !dam || !wgt) return;


	/* Description (too brief) */
	object_desc_store(buf, i_ptr, FALSE, 0);


	/* Misc info */
	strcpy(dam, "");

	/* Damage */
	switch (i_ptr->tval)
	{
		/* Guns */
		case TV_GUN:
		{
			break;
		}

		/* Ammo */
		case TV_AMMO:
		case TV_SHOT:
		case TV_BULLET:
		{
			sprintf(dam, "%dd%d", i_ptr->dd, i_ptr->ds);
			break;
		}

		/* Weapons */
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		case TV_DAGGER:
		case TV_AXES:
		case TV_BLUNT:
		case TV_DIGGING:
		{
			sprintf(dam, "%dd%d", i_ptr->dd, i_ptr->ds);
			break;
		}

		/* Armour */
		case TV_BOOTS:
		case TV_GLOVES:
		case TV_CLOAK:
		case TV_CROWN:
		case TV_HELM:
		case TV_LEG:
		case TV_SOFT_ARMOR:
		case TV_HARD_ARMOR:
		case TV_DRAG_ARMOR:
		{
			sprintf(dam, "%d", i_ptr->ac);
			break;
		}
	}


	/* Weight */
	sprintf(wgt, "%3d.%d", i_ptr->weight / 10, i_ptr->weight % 10);
}


/*
 * Create a spoiler file for items
 */
static void spoil_obj_desc(cptr fname)
{
	int i, k, s, t, n = 0;

	u16b who[200];

	char buf[1024];

	char wgt[80];
	char dam[80];

	cptr format = "%-51s  %7s%6s%4s%9s\n";

	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Open the file */
	fff = my_fopen(buf, "w");

	/* Oops */
	if (!fff)
	{
		msg_print("Cannot create spoiler file.");
		return;
	}


	/* Header */
	fprintf(fff, "Spoiler File -- Basic Items (%s)\n\n\n", VERSION_STRING);

	/* More Header */
	fprintf(fff, format, "Description", "Dam/AC", "Wgt", "Lev", "Cost");
	fprintf(fff, format, "----------------------------------------",
	        "------", "---", "---", "----");

	/* List the groups */
	for (i = 0; TRUE; i++)
	{
		/* Write out the group title */
		if (group_item[i].name)
		{
			/* Hack -- bubble-sort by cost and then level */
			for (s = 0; s < n - 1; s++)
			{
				for (t = 0; t < n - 1; t++)
				{
					int i1 = t;
					int i2 = t + 1;

					int e1;
					int e2;

					s32b t1;
					s32b t2;

					kind_info(NULL, NULL, NULL, &e1, &t1, who[i1]);
					kind_info(NULL, NULL, NULL, &e2, &t2, who[i2]);

					if ((t1 > t2) || ((t1 == t2) && (e1 > e2)))
					{
						int tmp = who[i1];
						who[i1] = who[i2];
						who[i2] = tmp;
					}
				}
			}

			/* Spoil each item */
			for (s = 0; s < n; s++)
			{
				int e;
				s32b v;

				/* Describe the kind */
				kind_info(buf, dam, wgt, &e, &v, who[s]);

				/* Dump it */
				fprintf(fff, "  %-51s%7s%6s%4d%9ld\n",
				        buf, dam, wgt, e, (long)(v));
			}

			/* Start a new set */
			n = 0;

			/* Notice the end */
			if (!group_item[i].tval) break;

			/* Start a new set */
			fprintf(fff, "\n\n%s\n\n", group_item[i].name);
		}

		/* Get legal item types */
		for (k = 1; k < z_info->k_max; k++)
		{
			object_kind *k_ptr = &k_info[k];

			/* Skip wrong tval's */
			if (k_ptr->tval != group_item[i].tval) continue;

			/* Hack -- Skip instant-artifacts */
			if (k_ptr->flags3 & (TR3_INSTA_ART)) continue;

			/* Save the index */
			who[n++] = k;
		}
	}


	/* Check for errors */
	if (ferror(fff) || my_fclose(fff))
	{
		msg_print("Cannot close spoiler file.");
		return;
	}

	/* Message */
	msg_print("Successfully created a spoiler file.");
}



/*
 * Artifact Spoilers by: randy@PICARD.tamu.edu (Randy Hutson)
 */


/*
 * Returns a "+" string if a number is non-negative and an empty
 * string if negative
 */
#define POSITIZE(v) (((v) >= 0) ? "+" : "")

/*
 * These are used to format the artifact spoiler file. INDENT1 is used
 * to indent all but the first line of an artifact spoiler. INDENT2 is
 * used when a line "wraps". (Bladeturner's resistances cause this.)
 */
#define INDENT1 "    "
#define INDENT2 "      "

/*
 * MAX_LINE_LEN specifies when a line should wrap.
 */
#define MAX_LINE_LEN 75

/*
 * Given an array, determine how many elements are in the array
 */
#define N_ELEMENTS(a) (sizeof (a) / sizeof ((a)[0]))

/*
 * The artifacts categorized by type
 */
static const grouper group_artifact[] =
{
	{ TV_SWORD,		"Edged Swords" },
	{ TV_POLEARM,	"Piercing Polearms" },
	{ TV_HAFTED,	"Blunt Hafted Weapons" },
	{ TV_DAGGER,	"Piercing Daggers" },
	{ TV_AXES,		"Slashing Axes" },
	{ TV_BLUNT,		"Blunt Weapons" },
	{ TV_GUN,		"Guns" },
	{ TV_DIGGING,	"Diggers" },

	{ TV_SOFT_ARMOR,	"Body Armor" },
	{ TV_HARD_ARMOR,	"Body Armor" },
	{ TV_DRAG_ARMOR,	  NULL },

	{ TV_CLOAK,		"Cloaks" },
	{ TV_LEG,	"Pants (formerly Shields)" },
	{ TV_HELM,		"Helms/Crowns" },
	{ TV_CROWN,		  NULL },
	{ TV_GLOVES,	"Gloves" },
	{ TV_BOOTS,		"Boots" },

	{ TV_LITE,		"Light Sources" },
	{ TV_AMULET,	"Amulets" },
	{ TV_RING,		"Rings" },

	{ 0, NULL }
};



/*
 * Pair together a constant flag with a textual description.
 *
 * Note that it sometimes more efficient to actually make an array
 * of textual names, where entry 'N' is assumed to be paired with
 * the flag whose value is "1L << N", but that requires hard-coding.
 */

typedef struct flag_desc flag_desc;

struct flag_desc
{
	const u32b flag;
	const char *const desc;
};



/*
 * These are used for "+3 to STR, DEX", etc. These are separate from
 * the other pval affected traits to simplify the case where an object
 * affects all stats.  In this case, "All stats" is used instead of
 * listing each stat individually.
 */

static const flag_desc stat_flags_desc[] =
{
	{ TR_PVAL_MUS,        "Muscle" },
	{ TR_PVAL_AGI,        "Agility" },
	{ TR_PVAL_VIG,        "Vigor" },
	{ TR_PVAL_SCH,        "Schooling" },
	{ TR_PVAL_EGO,        "Ego" },
	{ TR_PVAL_CHR,        "Charm" }
};

/*
 * Besides stats, these are the other player traits
 * which may be affected by an object's pval
 */

static const flag_desc pval_flags1_desc[] =
{
	{ TR_PVAL_STEALTH,			"Stealth" },
	{ TR_PVAL_SEARCH,			"Searching" },
	{ TR_PVAL_INFRA,			"Infravision" },
	{ TR_PVAL_TUNNEL,			"Tunneling" },
	{ TR_PVAL_SPEED,			"Speed" },
	{ TR_PVAL_MANA,				"Mana" },
	{ TR_PVAL_HEALTH,			"Health" },
	{ TR_PVAL_LIGHT,			"Light" },
	{ TR_PVAL_SAVE,				"Save" },
	{ TR_PVAL_MAGIC_MASTER,		"Magic Master" },
	{ TR_PVAL_BLOWS,			"Attacks" },
	{ TR_PVAL_SHOTS,			"Shots" },
	{ TR_PVAL_MIGHT,			"Might" }
};

/*
 * Slaying preferences for weapons
 */

static const flag_desc slay_flags_desc[] =
{
	{ TR1_SLAY_ANIMAL,			"Animal" },
	{ TR1_SLAY_EVIL,			"Evil" },
	{ TR1_SLAY_UNDEAD,			"Undead" },
	{ TR1_SLAY_DEMON,			"Demon" },
	{ TR1_SLAY_AUTOMATA,		"Automata" },
	{ TR1_SLAY_DINOSAUR,		"Dinosaur" },
	{ TR1_SLAY_CONSTRUCT,		"Construct" },
	{ TR1_SLAY_ELEMENTAL,		"Elemental" },
	{ TR1_SLAY_ALIEN,			"Alien" },
	{ TR1_SLAY_CARDS,			"Cards" },
	{ TR1_SLAY_BEASTMAN,		"Beastman" }
};

/*
 * Elemental brands for weapons
 */
static const flag_desc brand_flags_desc[] =
{
	{ TR1_BRAND_ACID,			"Acid Brand" },
	{ TR1_BRAND_ELEC,			"Lightning Brand" },
	{ TR1_BRAND_FIRE,			"Flame Tongue" },
	{ TR1_BRAND_ICE,			"Frost Brand" },
	{ TR1_BRAND_POISON,			"Venom" },
	{ TR1_BRAND_FORCE,			"Force" },
};

/*
 * The basic resistances
 */

static const flag_desc resist_flags_desc[] =
{
	{ TR2_RES_CONFU,			"Confusion" },
	{ TR2_RES_BLIND,			"Blindness" },
	{ TR2_RES_FEAR,				"Fear" },
};

/*
 * Elemental immunities (along with poison)
 */

static const flag_desc curse_flags_desc[] =
{
	{ TR3_MUTABLE,			"Mutable" },
	{ TR3_TELEPORT,			"Teleport" },
	{ TR3_EARTHQUAKE,		"Earthquake" },
	{ TR3_NEVER_BLOW,		"Never Blow" },
	{ TR3_NO_TELEPORT,		"No_Teleport" },
	{ TR3_NO_MAGIC,			"No_Magic" },
	{ TR3_DRAIN_EXP,		"Drain Experience" },
	{ TR3_DRAIN_HP,			"Drain HP" },
	{ TR3_DRAIN_SP,			"Drain SP" },
	{ TR3_DRAIN_ITEM,		"Drain Item" },
	{ TR3_AGGRAVATE,		"Aggravate" },
	{ TR3_DISRUPT_SPELL,	"Disrupt Spell" },
	{ TR3_LIGHT_CURSE,		"Light Curse" },
	{ TR3_HEAVY_CURSE,		"Heavy Curse" },
	{ TR3_PERMA_CURSE,		"Perma Curse" },
};

/*
 * Sustain stats -  these are given their "own" line in the
 * spoiler file, mainly for simplicity
 */
static const flag_desc sustain_flags_desc[] =
{
	{ TR1_SUST_MUS,   "Muscle" },
	{ TR1_SUST_AGI,   "Agility" },
	{ TR1_SUST_VIG,   "Vigor" },
	{ TR1_SUST_SCH,   "Schooling" },
	{ TR1_SUST_EGO,   "Ego" },
	{ TR1_SUST_CHR,   "Charm" },
};

static const flag_desc shield_flags_desc[] =
{
	{ TR3_SH_FIRE,		"Fire Shield" },
	{ TR3_SH_ELEC,		"Electric Shield" },
	{ TR3_SPINES,		"Spines" },
	{ TR3_SH_ETHER,		"Ether Shield (non-function)" },
};

/*
 * Miscellaneous magic given by an object's "flags3" field
 *
 * Note that cursed artifacts and objects with permanent light
 * are handled "directly" -- see analyze_misc_magic()
 */

static const flag_desc misc_flags2_desc[] =
{
	{ TR2_IMPACT,			"Impact" },
	{ TR2_CRITICAL,			"Critical" },
	{ TR2_VAMPIRIC,			"Vampiric" },
	{ TR2_THROW,			"Throwing" },
	{ TR2_PERFECT_BALANCE,	"Perfect Balance" },
	{ TR2_VORPAL,			"Vorpal" },
	{ TR2_RETURN,			"Return" },
	{ TR2_EDGED,			"Edged" },
	{ TR2_BLUNT,			"Blunt" },
	{ TR2_PIERCE,			"Pierce" },
	{ TR2_SLOW_DIGEST,		"Slow Digestion" },
	{ TR2_FEATHER,			"Feather fall" },
	{ TR2_TELEPATHY,		"Telepathy" },
	{ TR2_SEE_INVIS,		"See Invisible" },
	{ TR2_FREE_ACT,			"Free action" },
	{ TR2_WRAITH,			"Wraithform" },
	{ TR2_BLESSED,			"Blessed" },
	{ TR2_HOLD_LIFE,		"Hold Life" },
	{ TR2_REFLECT,			"Reflect (non-function)" },
	{ TR2_INVISIBLE,		"Invisible" },
	{ TR2_IMMUNE,			"Immune" },
	{ TR2_REGEN_25,			"Regenerate 25%" },
	{ TR2_REGEN_50,			"Regenerate 50%" },
	{ TR2_REGEN_75,			"Regenerate 75%" },
};

/*
 * A special type used just for deailing with pvals
 */

typedef struct
{
	/*
	 * This will contain a string such as "+2", "-10", etc.
	 */
	char pval_desc[12];

	/*
	 * A list of various player traits affected by an object's pval such
	 * as stats, speed, stealth, etc.  "Extra attacks" is NOT included in
	 * this list since it will probably be desirable to format its
	 * description differently.
	 *
	 * Note that room need only be reserved for the number of stats - 1
	 * since the description "All stats" is used if an object affects all
	 * all stats. Also, room must be reserved for a sentinel NULL pointer.
	 *
	 * This will be a list such as ["STR", "DEX", "Stealth", NULL] etc.
	 *
	 * This list includes extra attacks, for simplicity.
	 */
	cptr pval_affects[N_ELEMENTS(stat_flags_desc) - 1 +
	                  N_ELEMENTS(pval_flags1_desc) + 1];

} pval_info_type;


#if 0
static const flag_desc stat_flags_desc[] =
static const flag_desc pval_flags1_desc[] =
static const flag_desc slay_flags_desc[] =  **
static const flag_desc brand_flags_desc[] = **
static const flag_desc resist_flags_desc[] = **
static const flag_desc curse_flags_desc[] = 
static const flag_desc sustain_flags_desc[] = **
static const flag_desc shield_flags_desc[] =
static const flag_desc misc_flags3_desc[] = **

#endif

/*
 * An "object analysis structure"
 *
 * It will be filled with descriptive strings detailing an object's
 * various magical powers. The "ignore X" traits are not noted since
 * all artifacts ignore "normal" destruction.
 */

typedef struct
{
	/* "The Longsword Dragonsmiter (6d4) (+20, +25)" */
	char description[160];

	/* Description of what is affected by an object's pval */
	pval_info_type pval_info1;
	pval_info_type pval_info2;
	pval_info_type pval_info3;

	/* A list of an object's slaying preferences */
	cptr slays[N_ELEMENTS(slay_flags_desc) + 1];

	/* A list if an object's elemental brands */
	cptr brands[N_ELEMENTS(brand_flags_desc) + 1];

	/* A list of resistances granted by an object */
	cptr resistances[N_ELEMENTS(resist_flags_desc) + 1];

	/* A list of stats sustained by an object */
	cptr sustains[N_ELEMENTS(sustain_flags_desc)  - 1 + 1];

	/* A list of stats sustained by an object */
	cptr curse[N_ELEMENTS(curse_flags_desc)  - 1 + 1];

	/* A list of stats sustained by an object */
	cptr shield[N_ELEMENTS(shield_flags_desc)  - 1 + 1];

	/* A list of various magical qualities an object may have */
	cptr misc_magic[N_ELEMENTS(misc_flags2_desc)
	                + 1       /* type of curse */
	                + 1];     /* sentinel NULL */

	char resist[1024];
	
	/* A string describing an artifact's activation */
	char activation[80];

	/* "Level 20, Rarity 30, 3.0 lbs, 20000 Gold" */
	char misc_desc[80];

} obj_desc_list;



/*
 * Write out `n' of the character `c' to the spoiler file
 */
static void spoiler_out_n_chars(int n, char c)
{
	while (--n >= 0) fputc(c, fff);
}

/*
 * Write out `n' blank lines to the spoiler file
 */
static void spoiler_blanklines(int n)
{
	spoiler_out_n_chars(n, '\n');
}

/*
 * Write a line to the spoiler file and then "underline" it with hypens
 */
static void spoiler_underline(cptr str)
{
	fprintf(fff, "%s\n", str);
	spoiler_out_n_chars(strlen(str), '-');
	fprintf(fff, "\n");
}



/*
 * This function does most of the actual "analysis". Given a set of bit flags
 * (which will be from one of the flags fields from the object in question),
 * a "flag description structure", a "description list", and the number of
 * elements in the "flag description structure", this function sets the
 * "description list" members to the appropriate descriptions contained in
 * the "flag description structure".
 *
 * The possibly updated description pointer is returned.
 */

static cptr *spoiler_flag_aux(const u32b art_flags, const flag_desc *flag_x_ptr,
                              cptr *desc_x_ptr, const int n_elmnts)
{
	int i;

	for (i = 0; i < n_elmnts; ++i)
	{
		if (art_flags & flag_x_ptr[i].flag)
		{
			*desc_x_ptr++ = flag_x_ptr[i].desc;
		}
	}

	return desc_x_ptr;
}


/*
 * Get a "basic" description "The Cloak of Death [1,+10]"
 */
static void analyze_general(object_type *o_ptr, char *desc_x_ptr)
{
	/* Get a "useful" description of the object */
	object_desc_store(desc_x_ptr, o_ptr, TRUE, 1);

}

/*
 * List attributes altered by an artifact's pvals.
 */
static void analyze_pval(int num, object_type *o_ptr,
	pval_info_type *pval_x_ptr)
{
	const u32b all_stats = (TR_PVAL_MUS | TR_PVAL_AGI | TR_PVAL_VIG |
	                        TR_PVAL_SCH | TR_PVAL_EGO | TR_PVAL_CHR);

	int pval;
	u32b flags_pval;

	cptr *affects_list = pval_x_ptr->pval_affects;

	/* Get the right pval and pval-dependant flags */
	if (num == 1)
	{
		pval = o_ptr->pval;
		flags_pval = o_ptr->flags_pval1;
	}
	else if (num == 2)
	{
		pval = o_ptr->pval2;
		flags_pval = o_ptr->flags_pval2;
	}
	else if (num == 3)
	{
		pval = o_ptr->pval3;
		flags_pval = o_ptr->flags_pval3;
	}
	else return;

	/* Create the "+N" string */
	strnfmt(pval_x_ptr->pval_desc, sizeof(pval_x_ptr->pval_desc),
	        "%+d", pval);

	/* First, check to see if the pval affects all stats */
	if ((flags_pval & all_stats) == all_stats)
	{
		*affects_list++ = "All stats";
	}

	/* Are any stats affected? */
	else if (flags_pval & all_stats)
	{
		affects_list = spoiler_flag_aux(flags_pval, stat_flags_desc,
						affects_list,
						N_ELEMENTS(stat_flags_desc));
	}

	/* And now the "rest" */
	affects_list = spoiler_flag_aux(flags_pval, pval_flags1_desc,
					affects_list,
					N_ELEMENTS(pval_flags1_desc));

	/* Terminate the description list */
	*affects_list = NULL;
}




/*
 * Note the slaying specialities of a weapon
 */
static void analyze_slay(object_type *o_ptr, cptr *slay_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	slay_list = spoiler_flag_aux(f1, slay_flags_desc, slay_list,
	                             N_ELEMENTS(slay_flags_desc));

	/* Terminate the description list */
	*slay_list = NULL;
}


/*
 * Note an object's elemental brands
 */
static void analyze_brand(object_type *o_ptr, cptr *brand_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	brand_list = spoiler_flag_aux(f1, brand_flags_desc, brand_list,
	                              N_ELEMENTS(brand_flags_desc));

	/* Terminate the description list */
	*brand_list = NULL;
}


/*
 * Note the resistances granted by an object
 */
static void analyze_resist(object_type *o_ptr, cptr *resist_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	resist_list = spoiler_flag_aux(f2, resist_flags_desc,
	                               resist_list, N_ELEMENTS(resist_flags_desc));

	/* Terminate the description list */
	*resist_list = NULL;
}


/*
 * Note the immunities granted by an object
 */
static void analyze_curse(object_type *o_ptr, cptr *curse_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	curse_list = spoiler_flag_aux(f3, curse_flags_desc,
	                               curse_list, N_ELEMENTS(curse_flags_desc));

	/* Terminate the description list */
	*curse_list = NULL;

}

/*
 * Note the immunities granted by an object
 */
static void analyze_shield(object_type *o_ptr, cptr *shield_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	shield_list = spoiler_flag_aux(f3, shield_flags_desc,
	                               shield_list, N_ELEMENTS(shield_flags_desc));

	/* Terminate the description list */
	*shield_list = NULL;

}

/*
 * Note which stats an object sustains
 */
static void analyze_sustains(object_type *o_ptr, cptr *sustain_list)
{
	const u32b all_sustains = (TR1_SUST_MUS | TR1_SUST_AGI | TR1_SUST_VIG |
	                           TR1_SUST_SCH | TR1_SUST_EGO | TR1_SUST_CHR);

	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	/* Simplify things if an item sustains all stats */
	if ((f1 & all_sustains) == all_sustains)
	{
		*sustain_list++ = "All stats";
	}

	/* Should we bother? */
	else if ((f2 & all_sustains))
	{
		sustain_list = spoiler_flag_aux(f1, sustain_flags_desc,
		                                sustain_list,
		                                N_ELEMENTS(sustain_flags_desc));
	}

	/* Terminate the description list */
	*sustain_list = NULL;
}


/*
 * Note miscellaneous powers bestowed by an artifact such as see invisible,
 * free action, permanent light, etc.
 */
static void analyze_misc_magic(object_type *o_ptr, cptr *misc_list)
{
	u32b f1, f2, f3;

	object_flags(o_ptr, &f1, &f2, &f3);

	/*
	 * Special flags
	 */
	misc_list = spoiler_flag_aux(f2, misc_flags2_desc, misc_list,
	                             N_ELEMENTS(misc_flags2_desc));

	/*
	 * Handle cursed objects here to avoid redundancies such as noting
	 * that a permanently cursed object is heavily cursed as well as
	 * being "lightly cursed".
	 */

	if (cursed_p(o_ptr))
	{
		if (f3 & (TR3_PERMA_CURSE))
		{
			*misc_list++ = "Permanently Cursed";
		}
		else if (f3 & (TR3_HEAVY_CURSE))
		{
			*misc_list++ = "Heavily Cursed";
		}
		else
		{
			*misc_list++ = "Cursed";
		}
	}

	/* Terminate the description list */
	*misc_list = NULL;
}


/*
 * Determine the minimum depth an artifact can appear, its rarity, its weight,
 * and its value in gold pieces
 */
static void analyze_misc(object_type *o_ptr, char *misc_desc)
{
	artifact_type *a_ptr = &a_info[o_ptr->name1];

	sprintf(misc_desc, "Level %u, Rarity %u, %d.%d lbs, %ld Gold",
	        a_ptr->level, a_ptr->rarity,
	        a_ptr->weight / 10, a_ptr->weight % 10, a_ptr->cost);
}


/*
 * Fill in an object description structure for a given object
 */
static void object_analyze(object_type *o_ptr, obj_desc_list *desc_x_ptr)
{
	char buf[1024];
	buf[0] = '\0';

	analyze_general(o_ptr, desc_x_ptr->description);

	analyze_pval(1, o_ptr, &desc_x_ptr->pval_info1);
	analyze_pval(2, o_ptr, &desc_x_ptr->pval_info2);
	analyze_pval(3, o_ptr, &desc_x_ptr->pval_info3);

	analyze_brand(o_ptr, desc_x_ptr->brands);

	analyze_slay(o_ptr, desc_x_ptr->slays);

	analyze_shield(o_ptr, desc_x_ptr->shield);

	analyze_resist(o_ptr, desc_x_ptr->resistances);

	analyze_sustains(o_ptr, desc_x_ptr->sustains);

	analyze_misc_magic(o_ptr, desc_x_ptr->misc_magic);

	analyze_misc(o_ptr, desc_x_ptr->misc_desc);

	analyze_curse(o_ptr, desc_x_ptr->curse);

		/* Resistances. */
	if ((object_resist(o_ptr, RS_FIR) != 0) || (object_resist(o_ptr, RS_EAR) != 0) ||
		(object_resist(o_ptr, RS_AIR) != 0) || (object_resist(o_ptr, RS_WTR) != 0) ||
		(object_resist(o_ptr, RS_ELC) != 0) || (object_resist(o_ptr, RS_ICE) != 0) ||
		(object_resist(o_ptr, RS_ACD) != 0) || (object_resist(o_ptr, RS_PSN) != 0) ||
		(object_resist(o_ptr, RS_TIM) != 0) || (object_resist(o_ptr, RS_ETH) != 0) ||
		(object_resist(o_ptr, RS_SND) != 0) || (object_resist(o_ptr, RS_NTH) != 0) ||
		(object_resist(o_ptr, RS_LIT) != 0) || (object_resist(o_ptr, RS_DRK) != 0) ||
		(object_resist(o_ptr, RS_PSI) != 0) || (object_resist(o_ptr, RS_TLK) != 0) ||
		(object_resist(o_ptr, RS_SPI) != 0))
	{
		/* Clear number of items to list, and items listed. */
		int attr_num = 0;
		int attr_listed = 0;
		int j;
		
		/* How many attributes need to be listed? */
		if (object_resist(o_ptr, RS_FIR) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_EAR) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_AIR) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_WTR) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_ELC) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_ICE) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_ACD) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_PSN) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_TIM) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_ETH) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_SND) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_NTH) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_LIT) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_DRK) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_PSI) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_TLK) != 0)  attr_num++;
		if (object_resist(o_ptr, RS_SPI) != 0)  attr_num++;

		strcat(buf, "It provides"); /* resistance to"); */

		/* Loop for number of attributes in this group. */
		for (j = 0; j < RS_MAX; j++)
		{
			bool list_ok = FALSE;

			if ((j ==  0) && (object_resist(o_ptr, RS_FIR) != 0)) list_ok = TRUE;
			if ((j ==  1) && (object_resist(o_ptr, RS_EAR) != 0)) list_ok = TRUE;
			if ((j ==  2) && (object_resist(o_ptr, RS_AIR) != 0)) list_ok = TRUE;
			if ((j ==  3) && (object_resist(o_ptr, RS_WTR) != 0)) list_ok = TRUE;
			if ((j ==  4) && (object_resist(o_ptr, RS_ELC) != 0)) list_ok = TRUE;
			if ((j ==  5) && (object_resist(o_ptr, RS_ICE) != 0)) list_ok = TRUE;
			if ((j ==  6) && (object_resist(o_ptr, RS_ACD) != 0)) list_ok = TRUE;
			if ((j ==  7) && (object_resist(o_ptr, RS_PSN) != 0)) list_ok = TRUE;
			if ((j ==  8) && (object_resist(o_ptr, RS_TIM) != 0)) list_ok = TRUE;
			if ((j ==  9) && (object_resist(o_ptr, RS_ETH) != 0)) list_ok = TRUE;
			if ((j == 10) && (object_resist(o_ptr, RS_SND) != 0)) list_ok = TRUE;
			if ((j == 11) && (object_resist(o_ptr, RS_NTH) != 0)) list_ok = TRUE;
			if ((j == 12) && (object_resist(o_ptr, RS_LIT) != 0)) list_ok = TRUE;
			if ((j == 13) && (object_resist(o_ptr, RS_DRK) != 0)) list_ok = TRUE;
			if ((j == 14) && (object_resist(o_ptr, RS_PSI) != 0)) list_ok = TRUE;
			if ((j == 15) && (object_resist(o_ptr, RS_TLK) != 0)) list_ok = TRUE;
			if ((j == 16) && (object_resist(o_ptr, RS_SPI) != 0)) list_ok = TRUE;

			if (!list_ok) continue;

			/* Listing another attribute. */
			attr_listed++;

			/* Commas separate members of a list of more than two. */
			if ((attr_num > 2) && (attr_listed > 1)) strcat(buf, ",");

			/* "and" before final member of a list of more than one. */
			if ((attr_num > 1) && (j != 0))
			{
				if (attr_num == attr_listed) strcat(buf, " and");
			}
							
			/* List the attribute description, in its proper place. */
			if (j ==  0) strcat(buf, format(" %d%% fire", (object_resist(o_ptr, RS_FIR))));
			if (j ==  1) strcat(buf, format(" %d%% earth/shards", (object_resist(o_ptr, RS_EAR))));
			if (j ==  2) strcat(buf, format(" %d%% air", (object_resist(o_ptr, RS_AIR))));
			if (j ==  3) strcat(buf, format(" %d%% water", (object_resist(o_ptr, RS_WTR))));
			if (j ==  4) strcat(buf, format(" %d%% electricity", (object_resist(o_ptr, RS_ELC))));
			if (j ==  5) strcat(buf, format(" %d%% ice", (object_resist(o_ptr, RS_ICE))));
			if (j ==  6) strcat(buf, format(" %d%% acid", (object_resist(o_ptr, RS_ACD))));
			if (j ==  7) strcat(buf, format(" %d%% poison", (object_resist(o_ptr, RS_PSN))));
			if (j ==  8) strcat(buf, format(" %d%% time", (object_resist(o_ptr, RS_TIM))));
			if (j ==  9) strcat(buf, format(" %d%% ether", (object_resist(o_ptr, RS_ETH))));
			if (j == 10) strcat(buf, format(" %d%% sound", (object_resist(o_ptr, RS_SND))));
			if (j == 11) strcat(buf, format(" %d%% nether", (object_resist(o_ptr, RS_NTH))));
			if (j == 12) strcat(buf, format(" %d%% light", (object_resist(o_ptr, RS_LIT))));
			if (j == 13) strcat(buf, format(" %d%% dark", (object_resist(o_ptr, RS_DRK))));
			if (j == 14) strcat(buf, format(" %d%% mental", (object_resist(o_ptr, RS_PSI))));
			if (j == 15) strcat(buf, format(" %d%% forces", (object_resist(o_ptr, RS_TLK))));
			if (j == 16) strcat(buf, format(" %d%% spirit", (object_resist(o_ptr, RS_SPI))));
		}

		/* End sentence.  Go to next line. */
		strcat(buf, " resistance. \0");
	}
	
	strcpy(desc_x_ptr->resist, buf);

	strcpy(desc_x_ptr->activation, do_activation_aux(OBJECT_INFO, o_ptr));
}


static void print_header(void)
{
	char buf[80];

	sprintf(buf, "Artifact Spoilers for %s Version %s",
	        VERSION_NAME, VERSION_STRING);
}


/*
 * This is somewhat ugly.
 *
 * Given a header ("Resist", e.g.), a list ("Fire", "Cold", Acid", e.g.),
 * and a separator character (',', e.g.), write the list to the spoiler file
 * in a "nice" format, such as:
 *
 *      Resist Fire, Cold, Acid
 *
 * That was a simple example, but when the list is long, a line wrap
 * should occur, and this should induce a new level of indention if
 * a list is being spread across lines. So for example, Bladeturner's
 * list of resistances should look something like this
 *
 *     Resist Acid, Lightning, Fire, Cold, Poison, Light, Dark, Blindness,
 *       Confusion, Sound, Shards, Nether, Nexus, Chaos, Disenchantment
 *
 * However, the code distinguishes between a single list of many items vs.
 * many lists. (The separator is used to make this determination.) A single
 * list of many items will not cause line wrapping (since there is no
 * apparent reason to do so). So the lists of Ulmo's miscellaneous traits
 * might look like this:
 *
 *     Free Action; Hold Life; See Invisible; Slow Digestion; Regeneration
 *     Blessed Blade
 *
 * So comparing the two, "Regeneration" has no trailing separator and
 * "Blessed Blade" was not indented. (Also, Ulmo's lists have no headers,
 * but that's not relevant to line wrapping and indention.)
 */

/* ITEM_SEP separates items within a list */
#define ITEM_SEP ','


/* LIST_SEP separates lists */
#define LIST_SEP ';'


static void spoiler_outlist(cptr header, const cptr *list, char separator)
{
	int line_len, buf_len;
	char line[MAX_LINE_LEN+1], buf[80];

	/* Ignore an empty list */
	if (*list == NULL) return;

	/* This function always indents */
	strcpy(line, INDENT1);

	/* Create header (if one was given) */
	if (header && (header[0]))
	{
		strcat(line, header);
		strcat(line, " ");
	}

	line_len = strlen(line);

	/* Now begin the tedious task */
	while (1)
	{
		/* Copy the current item to a buffer */
		strcpy(buf, *list);

		/* Note the buffer's length */
		buf_len = strlen(buf);

		/*
		 * If there is an item following this one, pad with separator and
		 * a space and adjust the buffer length
		 */
		if (list[1])
		{
			sprintf(buf + buf_len, "%c ", separator);
			buf_len += 2;
		}

		/*
		 * If the buffer will fit on the current line, just append the
		 * buffer to the line and adjust the line length accordingly.
		 */
		if (line_len + buf_len <= MAX_LINE_LEN)
		{
			strcat(line, buf);
			line_len += buf_len;
		}
		/* Apply line wrapping and indention semantics described above */
		else
		{
			/*
			 * Don't print a trailing list separator but do print a trailing
			 * item separator.
			 */
			if ((line_len > 1) && (line[line_len - 1] == ' ') &&
			    (line[line_len - 2] == LIST_SEP))
			{
				/* Ignore space and separator */
				line[line_len - 2] = '\0';

				/* Write to spoiler file */
				fprintf(fff, "%s\n", line);

				/* Begin new line at primary indention level */
				sprintf(line, "%s%s", INDENT1, buf);
			}
			else
			{
				/* Write to spoiler file */
				fprintf(fff, "%s\n", line);

				/* Begin new line at secondary indention level */
				sprintf(line, "%s%s", INDENT2, buf);
			}

			line_len = strlen(line);
		}

		/* Advance, with break */
		if (!*++list) break;
	}

	/* Write what's left to the spoiler file */
	fprintf(fff, "%s\n", line);
}


/*
 * Create a spoiler file entry for an artifact
 */
static void spoiler_print_art(obj_desc_list *art_ptr)
{
	pval_info_type *pval_ptr = &art_ptr->pval_info1;

	char buf[80];

	
	/* Don't indent the first line */
	fprintf(fff, "%s\n", art_ptr->description);

	/* End with the miscellaneous facts */
	fprintf(fff, "%s%s\n", INDENT1, art_ptr->misc_desc);
	
	/* An "empty" pval description indicates that the pval affects nothing */
	if (pval_ptr->pval_desc[0])
	{
		/* Mention the effects of pval */
		strnfmt(buf, sizeof(buf), "%s to", pval_ptr->pval_desc);
		spoiler_outlist(buf, pval_ptr->pval_affects, ITEM_SEP);
	}

	pval_ptr = &art_ptr->pval_info2;
	if (pval_ptr->pval_desc[0])
	{
		/* Mention the effects of pval */
		strnfmt(buf, sizeof(buf), "%s to", pval_ptr->pval_desc);
		spoiler_outlist(buf, pval_ptr->pval_affects, ITEM_SEP);
	}

	pval_ptr = &art_ptr->pval_info3;
	if (pval_ptr->pval_desc[0])
	{
		/* Mention the effects of pval */
		strnfmt(buf, sizeof(buf), "%s to", pval_ptr->pval_desc);
		spoiler_outlist(buf, pval_ptr->pval_affects, ITEM_SEP);
	}

	/* Now deal with the description lists */

	spoiler_outlist("Slay", art_ptr->slays, ITEM_SEP);

	spoiler_outlist("", art_ptr->brands, LIST_SEP);

	spoiler_outlist("Curses:", art_ptr->curse, ITEM_SEP);

	spoiler_outlist("Shields:", art_ptr->shield, ITEM_SEP);

	spoiler_outlist("Sustain", art_ptr->sustains, ITEM_SEP);

	spoiler_outlist("", art_ptr->misc_magic, LIST_SEP);

	spoiler_outlist("Resist", art_ptr->resistances, ITEM_SEP);


	/* Write out the possible activation at the primary indention level */
	if (art_ptr->activation)
	{
		fprintf(fff, "%sActivates for %s\n", INDENT1, art_ptr->activation);
	}
	fprintf(fff, "%s%s\n\n", INDENT1, art_ptr->resist);

}


/*
 * Hack -- Create a "forged" artifact
 */
static bool make_fake_artifact(object_type *o_ptr, int name1)
{
	int i;

	artifact_type *a_ptr = &a_info[name1];


	/* Ignore "empty" artifacts */
	if (!a_ptr->name) return FALSE;

	/* Get the "kind" index */
	i = lookup_kind(a_ptr->tval, a_ptr->sval);

	/* Oops */
	if (!i) return (FALSE);

	/* Create the artifact */
	object_prep(o_ptr, i);

	/* Save the name */
	o_ptr->name1 = name1;

	/* Extract the fields */
	o_ptr->ac = a_ptr->ac;
	o_ptr->force = a_ptr->force;
	o_ptr->dd = a_ptr->dd;
	o_ptr->ds = a_ptr->ds;
	o_ptr->to_a = a_ptr->to_a;
	o_ptr->to_h = a_ptr->to_h;
	o_ptr->to_d = a_ptr->to_d;
	o_ptr->weight = a_ptr->weight;

	/* Extract pvals and related flags */
	o_ptr->pval = a_ptr->pval1;
	o_ptr->flags_pval1 = a_ptr->flags_pval1;
	o_ptr->pval2 = a_ptr->pval2;
	o_ptr->flags_pval2 = a_ptr->flags_pval2;
	o_ptr->pval3 = a_ptr->pval3;
	o_ptr->flags_pval3 = a_ptr->flags_pval3;

	o_ptr->flags1 = a_ptr->flags1;
	o_ptr->flags2 = a_ptr->flags2;
	o_ptr->flags3 = a_ptr->flags3;

	/* Success */
	return (TRUE);
}


/*
 * Create a spoiler file for artifacts
 */
static void spoil_artifact(cptr fname)
{
	int i, j;

	object_type *i_ptr;
	object_type object_type_body;

	obj_desc_list artifact;

	char buf[1024];


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Open the file */
	fff = my_fopen(buf, "w");

	/* Oops */
	if (!fff)
	{
		msg_print("Cannot create spoiler file.");
		return;
	}

	/* Dump the header */
	print_header();

	/* List the artifacts by tval */
	for (i = 0; group_artifact[i].tval; i++)
	{
		/* Write out the group title */
		if (group_artifact[i].name)
		{
			spoiler_blanklines(2);
			spoiler_underline(group_artifact[i].name);
			spoiler_blanklines(1);
		}

		/* Now search through all of the artifacts */
		for (j = 1; j < z_info->a_max; ++j)
		{
			artifact_type *a_ptr = &a_info[j];

			/* We only want objects in the current group */
			if (a_ptr->tval != group_artifact[i].tval) continue;

			/* Get local object */
			i_ptr = &object_type_body;

			/* Wipe the object */
			object_wipe(i_ptr);

			/* Attempt to "forge" the artifact */
			if (!make_fake_artifact(i_ptr, j)) continue;

			/* Analyze the artifact */
			object_analyze(i_ptr, &artifact);

			/* Write out the artifact description to the spoiler file */
			spoiler_print_art(&artifact);
		}
	}

	/* Check for errors */
	if (ferror(fff) || my_fclose(fff))
	{
		msg_print("Cannot close spoiler file.");
		return;
	}

	/* Message */
	msg_print("Successfully created a spoiler file.");
}





/*
 * Create a spoiler file for monsters
 */
static void spoil_mon_desc(cptr fname)
{
	int i, n = 0;

	char buf[1024];

	char nam[80];
	char lev[80];
	char rar[80];
	char spd[80];
	char ac[80];
	char hp[80];
	char exp[80];

	u16b *who;
	u16b why = 2;


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Open the file */
	fff = my_fopen(buf, "w");

	/* Oops */
	if (!fff)
	{
		msg_print("Cannot create spoiler file.");
		return;
	}

	/* Dump the header */
	fprintf(fff, "Monster Spoilers for %s Version %s\n",
	        VERSION_NAME, VERSION_STRING);
	fprintf(fff, "------------------------------------------\n\n");

	/* Dump the header */
	fprintf(fff, "%-40.40s%4s%4s%6s%8s%4s  %11.11s\n",
	        "Name", "Lev", "Rar", "Spd", "Hp", "Ac", "Visual Info");
	fprintf(fff, "%-40.40s%4s%4s%6s%8s%4s  %11.11s\n",
	        "----", "---", "---", "---", "--", "--", "-----------");


	/* Allocate the "who" array */
	C_MAKE(who, z_info->r_max, u16b);

	/* Scan the monsters (except the ghost) */
	for (i = 1; i < z_info->r_max - 1; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Use that monster */
		if (r_ptr->name) who[n++] = i;
	}

	/* Select the sort method */
	ang_sort_comp = ang_sort_comp_hook;
	ang_sort_swap = ang_sort_swap_hook;

	/* Sort the array by dungeon depth of monsters */
	ang_sort(who, &why, n);

	/* Scan again */
	for (i = 0; i < n; i++)
	{
		monster_race *r_ptr = &r_info[who[i]];

		cptr name = (r_name + r_ptr->name);

		/* Get the "name" */
		if (r_ptr->flags1 & (RF1_QUESTOR))
		{
			sprintf(nam, "[Q] %s", name);
		}
		else if (r_ptr->flags1 & (RF1_UNIQUE))
		{
			sprintf(nam, "[U] %s", name);
		}
		else
		{
			sprintf(nam, "The %s", name);
		}


		/* Level */
		sprintf(lev, "%d", r_ptr->level);

		/* Rarity */
		sprintf(rar, "%d", r_ptr->rarity);

		/* Speed */
		if (r_ptr->speed >= 110)
		{
			sprintf(spd, "+%d", (r_ptr->speed - 110));
		}
		else
		{
			sprintf(spd, "-%d", (110 - r_ptr->speed));
		}

		/* Armor Class */
		sprintf(ac, "%d", r_ptr->ac);

		/* Hitpoints */
		if (r_ptr->flags1 & (RF1_FIXED_HPS))
		{
			sprintf(hp, "%d", r_ptr->hitpoints);
		}
		else
		{
			sprintf(hp, "~%d", r_ptr->hitpoints);
		}

		/* Experience */
		sprintf(exp, "%ld", (long)(r_ptr->mexp));

		/* Hack -- use visual instead */
		sprintf(exp, "%s '%c'", attr_to_text(r_ptr->d_attr), r_ptr->d_char);

		/* Dump the info */
		fprintf(fff, "%-40.40s%4s%4s%6s%8s%4s  %11.11s\n",
		        nam, lev, rar, spd, hp, ac, exp);
	}

	/* End it */
	fprintf(fff, "\n");

	/* Free the "who" array */
	C_KILL(who, z_info->r_max, u16b);


	/* Check for errors */
	if (ferror(fff) || my_fclose(fff))
	{
		msg_print("Cannot close spoiler file.");
		return;
	}

	/* Worked */
	msg_print("Successfully created a spoiler file.");
}




/*
 * Monster spoilers by: smchorse@ringer.cs.utsa.edu (Shawn McHorse)
 *
 * Adapted from the "monster_desc()" code in "monster1.c"
 */

/*
 * Pronoun arrays
 */
static cptr wd_che[3] =
{ "It", "He", "She" };
static cptr wd_lhe[3] =
{ "it", "he", "she" };

/*
 * Buffer text to the given file. (-SHAWN-)
 * This is basically c_roff() from mon-desc.c with a few changes.
 */
static void spoil_out(cptr str)
{
	cptr r;

	/* Line buffer */
	static char roff_buf[256];

	/* Current pointer into line roff_buf */
	static char *roff_p = roff_buf;

	/* Last space saved into roff_buf */
	static char *roff_s = NULL;

	/* Special handling for "new sequence" */
	if (!str)
	{
		if (roff_p != roff_buf) roff_p--;
		while (*roff_p == ' ' && roff_p != roff_buf) roff_p--;
		if (roff_p == roff_buf) fprintf(fff, "\n");
		else
		{
			*(roff_p + 1) = '\0';
			fprintf(fff, "%s\n\n", roff_buf);
		}
		roff_p = roff_buf;
		roff_s = NULL;
		roff_buf[0] = '\0';
		return;
	}

	/* Scan the given string, character at a time */
	for (; *str; str++)
	{
		char ch = *str;
		int wrap = (ch == '\n');

		if (!isprint(ch)) ch = ' ';
		if (roff_p >= roff_buf + 75) wrap = 1;
		if ((ch == ' ') && (roff_p + 2 >= roff_buf + 75)) wrap = 1;

		/* Handle line-wrap */
		if (wrap)
		{
			*roff_p = '\0';
			r = roff_p;
			if (roff_s && (ch != ' '))
			{
				*roff_s = '\0';
				r = roff_s + 1;
			}
			fprintf(fff, "%s\n", roff_buf);
			roff_s = NULL;
			roff_p = roff_buf;
			while (*r) *roff_p++ = *r++;
		}

		/* Save the char */
		if ((roff_p > roff_buf) || (ch != ' '))
		{
			if (ch == ' ') roff_s = roff_p;
			*roff_p++ = ch;
		}
	}
}


/*
 * Create a spoiler file for monsters (-SHAWN-)
 */
static void spoil_mon_info(cptr fname)
{
	char buf[1024];
	int msex, vn, i, j, k, n;
	bool breath, magic, sin;
	cptr p, q;
	cptr vp[64];
	u32b flags1, flags2, flags3, flags4, flags5, flags6, flags7, flags8;
	u16b why = 2;
	s16b *who;
	int count = 0;


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Open the file */
	fff = my_fopen(buf, "w");

	/* Oops */
	if (!fff)
	{
		msg_print("Cannot create spoiler file.");
		return;
	}


	/* Dump the header */
	sprintf(buf, "Monster Spoilers for %s Version %s\n",
	        VERSION_NAME, VERSION_STRING);
	spoil_out(buf);
	spoil_out("------------------------------------------\n\n");

	/* Allocate the "who" array */
	C_MAKE(who, z_info->r_max, s16b);

	/* Scan the monsters */
	for (i = 1; i < z_info->r_max; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Use that monster */
		if (r_ptr->name) who[count++] = i;
	}

	/* Select the sort method */
	ang_sort_comp = ang_sort_comp_hook;
	ang_sort_swap = ang_sort_swap_hook;

	/* Sort the array by dungeon depth of monsters */
	ang_sort(who, &why, count);

	/*
	 * List all monsters in order (except the ghost).
	 */
	for (n = 0; n < count; n++)
	{
		monster_race *r_ptr = &r_info[who[n]];

		/* Extract the flags */
		flags1 = r_ptr->flags1;
		flags2 = r_ptr->flags2;
		flags3 = r_ptr->flags3;
		flags4 = r_ptr->flags4;
		flags5 = r_ptr->flags5;
		flags6 = r_ptr->flags6;
		flags7 = r_ptr->flags7;
		flags8 = r_ptr->flags8;
		breath = FALSE;
		magic = FALSE;

		/* Extract a gender (if applicable) */
		if (flags1 & (RF1_FEMALE)) msex = 2;
		else if (flags1 & (RF1_MALE)) msex = 1;
		else msex = 0;


		/* Prefix */
		if (flags1 & (RF1_QUESTOR))
		{
			spoil_out("[Q] ");
		}
		else if (flags1 & (RF1_UNIQUE))
		{
			spoil_out("[U] ");
		}
		else
		{
			spoil_out("The ");
		}

		/* Name */
		sprintf(buf, "%s  (", (r_name + r_ptr->name));	/* ---)--- */
		spoil_out(buf);

		/* Color */
		spoil_out(attr_to_text(r_ptr->d_attr));

		/* Symbol --(-- */
		sprintf(buf, " '%c')\n", r_ptr->d_char);
		spoil_out(buf);


		/* Indent */
		sprintf(buf, "=== ");
		spoil_out(buf);

		/* Number */
		sprintf(buf, "Num:%d  ", who[n]);
		spoil_out(buf);

		/* Level */
		sprintf(buf, "Lev:%d  ", r_ptr->level);
		spoil_out(buf);

		/* Rarity */
		sprintf(buf, "Rar:%d  ", r_ptr->rarity);
		spoil_out(buf);

		/* Speed */
		if (r_ptr->speed >= 110)
		{
			sprintf(buf, "Spd:+%d  ", (r_ptr->speed - 110));
		}
		else
		{
			sprintf(buf, "Spd:-%d  ", (110 - r_ptr->speed));
		}
		spoil_out(buf);

		/* Hitpoints */
		/* Hitpoints */
		if (r_ptr->flags1 & (RF1_FIXED_HPS))
		{
			sprintf(buf, "%d", r_ptr->hitpoints);
		}
		else
		{
			sprintf(buf, "~%d", r_ptr->hitpoints);
		}
		spoil_out(buf);

		/* Armor Class */
		sprintf(buf, "Ac:%d  ", r_ptr->ac);
		spoil_out(buf);

		/* Experience */
		sprintf(buf, "Exp:%ld\n", (long)(r_ptr->mexp));
		spoil_out(buf);


		/* Describe */
		spoil_out(r_text + r_ptr->text);
		spoil_out("  ");


		spoil_out("This");

		if (flags3 & (RF3_ANIMAL)) spoil_out(" natural");
		if (flags3 & (RF3_EVIL)) spoil_out(" evil");
		if (flags3 & (RF3_UNDEAD)) spoil_out(" undead");

		if (flags3 & (RF3_ELEMENTAL)) spoil_out(" elemental");
		else if (flags3 & (RF3_DEMON)) spoil_out(" demon");
		else if (flags3 & (RF3_CONSTRUCT)) spoil_out(" construct");
		else if (flags3 & (RF3_DINOSAUR)) spoil_out(" dinosaur");
		else if (flags3 & (RF3_AUTOMATA)) spoil_out(" automata");
		else if (flags3 & (RF3_ALIEN)) spoil_out(" alien");
		else if (flags3 & (RF3_BEASTMAN)) spoil_out(" beastman");

		else spoil_out(" creature");

		spoil_out(" moves");

		if ((flags1 & (RF1_RAND_50)) && (flags1 & (RF1_RAND_25)))
		{
			spoil_out(" extremely erratically");
		}
		else if (flags1 & (RF1_RAND_50))
		{
			spoil_out(" somewhat erratically");
		}
		else if (flags1 & (RF1_RAND_25))
		{
			spoil_out(" a bit erratically");
		}
		else
		{
			spoil_out(" normally");
		}

		if (flags1 & (RF1_NEVER_MOVE))
		{
			spoil_out(", but does not deign to chase intruders");
		}

		spoil_out(".  ");


#if 0

		if (!r_ptr->level || (flags1 & (RF1_FORCE_DEPTH)))
		{
			sprintf(buf, "%s is never found out of depth.  ", wd_che[msex]);
		}

		if (flags1 & (RF1_FORCE_SLEEP))
		{
			sprintf(buf, "%s is always created sluggish.  ", wd_che[msex]);
		}

#endif

		if (flags1 & (RF1_ESCORT))
		{
			sprintf(buf, "%s usually appears with ", wd_che[msex]);
			spoil_out(buf);
			if (flags1 & (RF1_ESCORTS)) spoil_out("escorts.  ");
			else spoil_out("an escort.  ");
		}

		if ((flags1 & (RF1_FRIEND)) || (flags1 & (RF1_FRIENDS)))
		{
			sprintf(buf, "%s usually appears in groups.  ", wd_che[msex]);
			spoil_out(buf);
		}

		msg_format("test is %d", r_ptr->d_char);

		/* Collect inate attacks */
		vn = 0;
#if 0
		if (flags4 & (RF4_SHRIEK)) vp[vn++] = "shriek for help";
		if (flags4 & (RF4_LASH)) vp[vn++] = "do something";
		if (flags4 & (RF4_BOLT)) vp[vn++] = "do something";
		if (flags4 & (RF4_ARROW)) vp[vn++] = "do something";
		if (flags4 & (RF4_ARROW_1)) vp[vn++] = "fire arrows";
		if (flags4 & (RF4_ARROW_2)) vp[vn++] = "fire arrows";
		if (flags4 & (RF4_ARROW_3)) vp[vn++] = "fire missiles";
		if (flags4 & (RF4_ARROW_4)) vp[vn++] = "fire missiles";
#endif
		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" may ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" or ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		/* Collect breaths */
		vn = 0;
#if 0
		if (flags4 & (RF4_BR_ACID)) vp[vn++] = "acid";
		if (flags4 & (RF4_BR_ELEC)) vp[vn++] = "lightning";
		if (flags4 & (RF4_BR_FIRE)) vp[vn++] = "fire";
		if (flags4 & (RF4_BR_COLD)) vp[vn++] = "frost";
		if (flags4 & (RF4_BR_POIS)) vp[vn++] = "poison";
		if (flags4 & (RF4_BR_NETH)) vp[vn++] = "nether";
		if (flags4 & (RF4_BR_LITE)) vp[vn++] = "light";
		if (flags4 & (RF4_BR_DARK)) vp[vn++] = "darkness";
		if (flags4 & (RF4_BR_CONF)) vp[vn++] = "confusion";
		if (flags4 & (RF4_BR_SOUN)) vp[vn++] = "sound";
		if (flags4 & (RF4_BR_CHAO)) vp[vn++] = "chaos";
		if (flags4 & (RF4_BR_DISE)) vp[vn++] = "disenchantment";
		if (flags4 & (RF4_BR_NEXU)) vp[vn++] = "nexus";
		if (flags4 & (RF4_BR_TIME)) vp[vn++] = "time";
		if (flags4 & (RF4_BR_INER)) vp[vn++] = "inertia";
		if (flags4 & (RF4_BR_GRAV)) vp[vn++] = "gravity";
		if (flags4 & (RF4_BR_SHAR)) vp[vn++] = "shards";
		if (flags4 & (RF4_BR_PLAS)) vp[vn++] = "plasma";
		if (flags4 & (RF4_BR_WALL)) vp[vn++] = "force";
		if (flags4 & (RF4_BR_MANA)) vp[vn++] = "mana";
		if (flags4 & (RF4_BR_NUKE)) vp[vn++] = "radiation";
		if (flags4 & (RF4_BO_NUKE)) vp[vn++] = "radiation";
		if (flags4 & (RF4_XXX7)) vp[vn++] = "something";
		if (flags4 & (RF4_XXX8)) vp[vn++] = "something";
#endif
		if (vn)
		{
			breath = TRUE;
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" may breathe ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" or ");
				spoil_out(vp[i]);
			}
			if (flags2 & (RF2_POWERFUL)) spoil_out(" powerfully");
		}

		/* Collect spells */
		vn = 0;
#if 0
		if (flags5 & (RF5_BA_ACID))           vp[vn++] = "produce acid balls";
		if (flags5 & (RF5_BA_ELEC))           vp[vn++] = "produce lightning balls";
		if (flags5 & (RF5_BA_FIRE))           vp[vn++] = "produce fire balls";
		if (flags5 & (RF5_BA_COLD))           vp[vn++] = "produce frost balls";
		if (flags5 & (RF5_BA_POIS))           vp[vn++] = "produce poison balls";
		if (flags5 & (RF5_BA_NETH))           vp[vn++] = "produce nether balls";
		if (flags5 & (RF5_BA_WATE))           vp[vn++] = "produce water balls";
		if (flags5 & (RF5_BA_MANA))           vp[vn++] = "produce mana storms";
		if (flags5 & (RF5_BA_DARK))           vp[vn++] = "produce darkness storms";
		if (flags5 & (RF5_DRAIN_MANA))        vp[vn++] = "drain mana";
		if (flags5 & (RF5_MIND_BLAST))        vp[vn++] = "cause mind blasting";
		if (flags5 & (RF5_BRAIN_SMASH))       vp[vn++] = "cause brain smashing";
		if (flags5 & (RF5_CAUSE_1))           vp[vn++] = "cause light wounds";
		if (flags5 & (RF5_CAUSE_2))           vp[vn++] = "cause serious wounds";
		if (flags5 & (RF5_CAUSE_3))           vp[vn++] = "cause critical wounds";
		if (flags5 & (RF5_CAUSE_4))           vp[vn++] = "cause mortal wounds";
		if (flags5 & (RF5_BO_ACID))           vp[vn++] = "produce acid bolts";
		if (flags5 & (RF5_BO_ELEC))           vp[vn++] = "produce lightning bolts";
		if (flags5 & (RF5_BO_FIRE))           vp[vn++] = "produce fire bolts";
		if (flags5 & (RF5_BO_COLD))           vp[vn++] = "produce frost bolts";
		if (flags5 & (RF5_BO_POIS))           vp[vn++] = "produce poison bolts";
		if (flags5 & (RF5_BO_NETH))           vp[vn++] = "produce nether bolts";
		if (flags5 & (RF5_BO_WATE))           vp[vn++] = "produce water bolts";
		if (flags5 & (RF5_BO_MANA))           vp[vn++] = "produce mana bolts";
		if (flags5 & (RF5_BO_PLAS))           vp[vn++] = "produce plasma bolts";
		if (flags5 & (RF5_BO_ICEE))           vp[vn++] = "produce ice bolts";
		if (flags5 & (RF5_MISSILE))           vp[vn++] = "produce magic missiles";
		if (flags5 & (RF5_SCARE))             vp[vn++] = "terrify";
		if (flags5 & (RF5_BLIND))             vp[vn++] = "blind";
		if (flags5 & (RF5_CONF))              vp[vn++] = "confuse";
		if (flags5 & (RF5_SLOW))              vp[vn++] = "slow";
		if (flags5 & (RF5_HOLD))              vp[vn++] = "paralyze";
		if (flags6 & (RF6_HASTE))             vp[vn++] = "haste-self";
		if (flags6 & (RF6_CURE))              vp[vn++] = "do something";
		if (flags6 & (RF6_HEAL))              vp[vn++] = "heal-self";
		if (flags6 & (RF6_ADD_MANA))              vp[vn++] = "do something";
		if (flags6 & (RF6_BLINK))             vp[vn++] = "blink-self";
		if (flags6 & (RF6_TPORT))             vp[vn++] = "teleport-self";
		if (flags6 & (RF6_CHARGE))              vp[vn++] = "CHARGE";
		if (flags6 & (RF6_XXX4))              vp[vn++] = "do something";
		if (flags6 & (RF6_TELE_TO))           vp[vn++] = "teleport to";
		if (flags6 & (RF6_TELE_AWAY))         vp[vn++] = "teleport away";
		if (flags6 & (RF6_TELE_LEVEL))        vp[vn++] = "teleport level";
		if (flags6 & (RF6_TELE_SELF_TO))              vp[vn++] = "do something";
		if (flags6 & (RF6_DARKNESS))          vp[vn++] = "create darkness";
		if (flags6 & (RF6_TRAPS))             vp[vn++] = "create traps";
		if (flags6 & (RF6_FORGET))            vp[vn++] = "cause amnesia";
		if (flags6 & (RF6_XXX6))              vp[vn++] = "do something";
		if (flags6 & (RF6_S_KIN))             vp[vn++] = "summon similar monsters";
		if (flags6 & (RF6_S_HI_DEMON))        vp[vn++] = "summon greater demons";
		if (flags6 & (RF6_S_MONSTER))         vp[vn++] = "summon a monster";
		if (flags6 & (RF6_S_MONSTERS))        vp[vn++] = "summon monsters";
		if (flags6 & (RF6_S_AUTOMATA))             vp[vn++] = "summon automata";
		if (flags6 & (RF6_S_SPIDER))          vp[vn++] = "summon spiders";
		if (flags6 & (RF6_S_HOUND))           vp[vn++] = "summon hounds";
		if (flags6 & (RF6_S_MONKEY))           vp[vn++] = "summon monkeys";
		if (flags6 & (RF6_S_ALIEN))           vp[vn++] = "summon an alien";
		if (flags6 & (RF6_S_DEMON))           vp[vn++] = "summon a demon";
		if (flags6 & (RF6_S_UNDEAD))          vp[vn++] = "summon an undead";
		if (flags6 & (RF6_S_ELEMENTAL))          vp[vn++] = "summon a elemental";
		if (flags6 & (RF6_S_HI_UNDEAD))       vp[vn++] = "summon greater undead";
		if (flags6 & (RF6_S_HI_ELEMENTAL))       vp[vn++] = "summon ancient elementals";
		if (flags6 & (RF6_S_WRAITH))          vp[vn++] = "summon ring wraiths";
		if (flags6 & (RF6_S_UNIQUE))          vp[vn++] = "summon unique monsters";
#endif
		if (vn)
		{
			magic = TRUE;
			if (breath)
			{
				spoil_out(", and is also");
			}
			else
			{
				spoil_out(wd_che[msex]);
				spoil_out(" is");
			}
			spoil_out(" magical, casting ");

			/* Describe magic */
			spoil_out(" spells");

			if (flags2 & (RF2_SMART)) spoil_out(" intelligently");
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" which ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" or ");
				spoil_out(vp[i]);
			}
		}

		if (breath || magic)
		{
			sprintf(buf, "; 1 time in %d.  ",
				100 / (r_ptr->freq_ranged));
			spoil_out(buf);
		}


		/* Collect special abilities. */
		vn = 0;
		if (flags2 & (RF2_OPEN_DOOR)) vp[vn++] = "open doors";
		if (flags2 & (RF2_BASH_DOOR)) vp[vn++] = "bash down doors";
		if (flags2 & (RF2_PASS_WALL)) vp[vn++] = "pass through walls";
		if (flags2 & (RF2_KILL_WALL)) vp[vn++] = "bore through walls";
		if (flags2 & (RF2_KILL_BODY)) vp[vn++] = "destroy weaker monsters";
		if (flags2 & (RF2_TAKE_ITEM)) vp[vn++] = "pick up objects";
		if (flags2 & (RF2_KILL_ITEM)) vp[vn++] = "destroy objects";

		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" can ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" and ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		if (flags2 & (RF2_INVISIBLE))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" is invisible.  ");
		}
		if (flags2 & (RF2_COLD_BLOOD))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" is cold blooded.  ");
		}
		if (flags2 & (RF2_EMPTY_MIND))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" is not detected by telepathy.  ");
		}
		if (flags2 & (RF2_WEIRD_MIND))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" is rarely detected by telepathy.  ");
		}
		if (flags2 & (RF2_MULTIPLY))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" breeds explosively.  ");
		}
		if (flags2 & (RF2_REGENERATE))
		{
			spoil_out(wd_che[msex]);
			spoil_out(" regenerates quickly.  ");
		}

		/* Collect susceptibilities */
		vn = 0;
		if (flags3 & (RF3_HURT_ROCK)) vp[vn++] = "rock remover";
		if (flags3 & (RF3_HURT_LIGHT)) vp[vn++] = "bright light";

		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" is hurt by ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" and ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		/* Collect immunities */
		vn = 0;
		if (flags3 & (RF3_IM_ACID)) vp[vn++] = "acid";
		if (flags3 & (RF3_IM_ELEC)) vp[vn++] = "lightning";
		if (flags3 & (RF3_IM_FIRE)) vp[vn++] = "fire";
		if (flags3 & (RF3_IM_ICE)) vp[vn++] = "cold";
		if (flags3 & (RF3_IM_POIS)) vp[vn++] = "poison";

		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" resists ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" and ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		/* Collect resistances */
		vn = 0;
#if 0
		if (flags3 & (RF3_RES_NETH)) vp[vn++] = "nether";
		if (flags3 & (RF3_RES_WATE)) vp[vn++] = "water";
		if (flags3 & (RF3_RES_PLAS)) vp[vn++] = "plasma";
		if (flags3 & (RF3_RES_NEXU)) vp[vn++] = "nexus";
		if (flags3 & (RF3_RES_DISE)) vp[vn++] = "disenchantment";
#endif
		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" resists ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" and ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		/* Collect non-effects */
		vn = 0;
		if (flags3 & (RF3_NO_STUN)) vp[vn++] = "stunned";
		if (flags3 & (RF3_NO_FEAR)) vp[vn++] = "frightened";
		if (flags3 & (RF3_NO_CONF)) vp[vn++] = "confused";
		if (flags3 & (RF3_NO_SLEEP)) vp[vn++] = "slept";

		if (vn)
		{
			spoil_out(wd_che[msex]);
			for (i = 0; i < vn; i++)
			{
				if (!i) spoil_out(" cannot be ");
				else if (i < vn-1) spoil_out(", ");
				else spoil_out(" or ");
				spoil_out(vp[i]);
			}
			spoil_out(".  ");
		}

		spoil_out(wd_che[msex]);
		if (r_ptr->sleep > 200)     spoil_out(" prefers to ignore");
		else if (r_ptr->sleep > 95) spoil_out(" pays very little attention to");
		else if (r_ptr->sleep > 75) spoil_out(" pays little attention to");
		else if (r_ptr->sleep > 45) spoil_out(" tends to overlook");
		else if (r_ptr->sleep > 25) spoil_out(" takes quite a while to see");
		else if (r_ptr->sleep > 10) spoil_out(" takes a while to see");
		else if (r_ptr->sleep > 5)  spoil_out(" is fairly observant of");
		else if (r_ptr->sleep > 3)  spoil_out(" is observant of");
		else if (r_ptr->sleep > 1)  spoil_out(" is very observant of");
		else if (r_ptr->sleep > 0)  spoil_out(" is vigilant for");
		else spoil_out(" is ever vigilant for");

		sprintf(buf, " intruders, which %s may notice from %d feet.  ",
		        wd_lhe[msex], 10 * r_ptr->aaf);
		spoil_out(buf);

		i = 0;
#if 0
		if (flags1 & (RF1_DROP_60)) i += 1;
		if (flags1 & (RF1_DROP_90)) i += 2;
		if (flags1 & (RF1_DROP_1D2)) i += 2;
		if (flags1 & (RF1_DROP_2D2)) i += 4;
		if (flags1 & (RF1_DROP_3D2)) i += 6;
		if (flags1 & (RF1_DROP_4D2)) i += 8;
#endif
		/* Drops gold and/or items */
		if (i)
		{
			sin = FALSE;
			spoil_out(wd_che[msex]);
			spoil_out(" will carry");

			if (i == 1)
			{
				spoil_out(" a");
				sin = TRUE;
			}
			else if (i == 2)
			{
				spoil_out(" one or two");
			}
			else
			{
				sprintf(buf, " up to %u", i);
				spoil_out(buf);
			}

			if (flags1 & (RF1_DROP_GREAT))
			{
				if (sin) spoil_out("n");
				spoil_out(" exceptional object");
			}
			else if (flags1 & (RF1_DROP_GOOD))
			{
				spoil_out(" good object");
			}
			else if (flags1 & (RF1_DROP_USEFUL))
			{
				spoil_out(" useful object");
			}
			else if (flags1 & (RF1_ONLY_ITEM))
			{
				if (sin) spoil_out("n");
				spoil_out(" object");
			}
			else if (flags1 & (RF1_ONLY_GOLD))
			{
				spoil_out(" treasure");
			}
			else
			{
				if (sin) spoil_out("n");
				spoil_out(" object");
				if (i > 1) spoil_out("s");
				spoil_out(" or treasure");
			}
			if (i > 1) spoil_out("s");

			if (flags1 & (RF1_DROP_CHOSEN))
			{
				spoil_out(", in addition to chosen objects");
			}

			spoil_out(".  ");
		}

		/* Count the actual attacks */
		for (i = 0, j = 0; j < 4; j++)
		{
			if (r_ptr->blow[j].method) i++;
		}

		/* Examine the actual attacks */
		for (k = 0, j = 0; j < 4; j++)
		{
			if (!r_ptr->blow[j].method) continue;

			/* No method yet */
			p = "?";

			/* Get the method */
			switch (r_ptr->blow[j].method)
			{
#if 0
				case RBM_HIT:	p = "hit"; break;
				case RBM_TOUCH:	p = "touch"; break;
				case RBM_PUNCH:	p = "punch"; break;
				case RBM_KICK:	p = "kick"; break;
				case RBM_CLAW:	p = "claw"; break;
				case RBM_BITE:	p = "bite"; break;
				case RBM_STING:	p = "sting"; break;
				case RBM_SEDUCE:	p = "seduce";	break;
				case RBM_BUTT:	p = "butt"; break;
				case RBM_CRUSH:	p = "crush"; break;
				case RBM_ENGULF:	p = "engulf"; break;
				case RBM_SHOCK:	p = "shock";break;
				case RBM_CRAWL:	p = "crawl on you"; break;
				case RBM_DROOL:	p = "drool on you"; break;
				case RBM_SPIT:	p = "spit"; break;
				case RBM_XXX3:	break;
				case RBM_GAZE:	p = "gaze"; break;
				case RBM_WAIL:	p = "wail"; break;
				case RBM_SPORE:	p = "release spores"; break;
				case RBM_XXX4:	break;
				case RBM_BEG:	p = "beg"; break;
				case RBM_INSULT:	p = "insult"; break;
				case RBM_MOAN:	p = "moan"; break;
				case RBM_SPEAK:	p = "speak"; break;
#endif
			}


			/* Default effect */
			q = "?";

			/* Get the effect */
			switch (r_ptr->blow[j].effect)
			{
#if 0
				case RBE_HURT:	q = "attack"; break;
				case RBE_POISON:	q = "poison"; break;
				case RBE_UN_BONUS:	q = "disenchant"; break;
				case RBE_UN_POWER:	q = "drain charges"; break;
				case RBE_EAT_GOLD:	q = "steal gold"; break;
				case RBE_EAT_ITEM:	q = "steal items"; break;
				case RBE_EAT_FOOD:	q = "eat your food"; break;
				case RBE_EAT_LITE:	q = "absorb light"; break;
				case RBE_ACID:	q = "shoot acid"; break;
				case RBE_ELEC:	q = "electrify"; break;
				case RBE_FIRE:	q = "burn"; break;
				case RBE_COLD:	q = "freeze"; break;
				case RBE_BLIND:	q = "blind"; break;
				case RBE_CONFUSE:	q = "confuse"; break;
				case RBE_TERRIFY:	q = "terrify"; break;
				case RBE_PARALYZE:	q = "paralyze"; break;
				case RBE_LOSE_MUS:	q = "reduce muscle"; break;
				case RBE_LOSE_AGI:	q = "reduce agility"; break;
				case RBE_LOSE_VIG:	q = "reduce vigor"; break;
				case RBE_LOSE_SCH:	q = "reduce schooling"; break;
				case RBE_LOSE_EGO:	q = "reduce ego"; break;
				case RBE_LOSE_CHR:	q = "reduce charm"; break;
				case RBE_LOSE_ALL:	q = "reduce all stats"; break;
				case RBE_SHATTER:	q = "shatter"; break;
				case RBE_EXP_10:	q = "lower experience"; break;
				case RBE_EXP_20:	q = "lower experience"; break;
				case RBE_EXP_40:	q = "lower experience"; break;
				case RBE_EXP_80:	q = "lower experience"; break;
#endif
			}


			if (!k)
			{
				spoil_out(wd_che[msex]);
				spoil_out(" can ");
			}
			else if (k < i-1)
			{
				spoil_out(", ");
			}
			else
			{
				spoil_out(", and ");
			}

			/* Describe the method */
			spoil_out(p);

			/* Describe the effect, if any */
			if (r_ptr->blow[j].effect)
			{
				spoil_out(" to ");
				spoil_out(q);
				if (r_ptr->blow[j].d_dice && r_ptr->blow[j].d_side)
				{
					spoil_out(" with damage");
					if (r_ptr->blow[j].d_side == 1)
						sprintf(buf, " %d", r_ptr->blow[j].d_dice);
					else
						sprintf(buf, " %dd%d",
					        r_ptr->blow[j].d_dice, r_ptr->blow[j].d_side);
					spoil_out(buf);
				}
			}

			k++;
		}

		if (k)
		{
			spoil_out(".  ");
		}
		else if (flags1 & (RF1_NEVER_BLOW))
		{
			sprintf(buf, "%s has no physical attacks.  ", wd_che[msex]);
			spoil_out(buf);
		}

		spoil_out(NULL);
	}

	/* Free the "who" array */
	C_KILL(who, z_info->r_max, s16b);

	/* Check for errors */
	if (ferror(fff) || my_fclose(fff))
	{
		msg_print("Cannot close spoiler file.");
		return;
	}

	msg_print("Successfully created a spoiler file.");
}



/*
 * Create Spoiler files
 */
void do_cmd_spoilers(void)
{
	char ch;


	/* Save screen */
	screen_save();


	/* Interact */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Info */
		prt("Create a spoiler file.", 2, 0);

		/* Prompt for a file */
		prt("(1) Brief Object Info (obj-desc.spo)", 5, 5);
		prt("(2) Brief Artifact Info (artifact.spo)", 6, 5);
		prt("(3) Brief Monster Info (mon-desc.spo)", 7, 5);
		prt("(4) Full Monster Info (mon-info.spo)", 8, 5);

		/* Prompt */
		prt("Command: ", 12, 0);

		/* Get a choice */
		ch = inkey();

		/* Escape */
		if (ch == ESCAPE)
		{
			break;
		}

		/* Option (1) */
		else if (ch == '1')
		{
			spoil_obj_desc("obj-desc.spo");
		}

		/* Option (2) */
		else if (ch == '2')
		{
			spoil_artifact("artifact.spo");
		}

		/* Option (3) */
		else if (ch == '3')
		{
			spoil_mon_desc("mon-desc.spo");
		}

		/* Option (4) */
		else if (ch == '4')
		{
			spoil_mon_info("mon-info.spo");
		}

		/* Oops */
		else
		{
			bell("Illegal command for spoilers!");
		}

		/* Flush messages */
		message_flush();
	}


	/* Load screen */
	screen_load();
}


#else

#ifdef MACINTOSH
static int i = 0;
#endif

#endif

