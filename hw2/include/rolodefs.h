#define VERSION "2.0"

#define CHUNKSIZE 100000        /* storage size for holding data file */

#define NOLOCKFLAG 'l'
#define SUMMARYFLAG 's'
#define OTHERUSERFLAG 'u'




#define LEGAL_OPTIONS "lsu"
#define USAGE "rolo [ person1 person2 ...] [ -l -s -u user ] "
/**
 * bug fix
 */
void user_eof ();
/**
 * bug fix
 */
void cathelpfile ();
/**
 * bug fix
 */
void save_and_exit ();

/**
 * bug fix
 */
void rolo_peruse_mode();
/**
 * bug fix
 */
void rolo_update_mode();
/**
 * bug fix
 */
void roloexit ();
/**
 * bug fx
 * @return
 */
int select_field_to_search_by ();
/**
 * bug fix
 */
int select_field_to_search_by ();
/**
 * bug fix
 */
void rolo_search_mode ();
/**
 * bug fix
 */
void rolo_add ();
/**
 * bug fix
 */
void save_to_disk ();

