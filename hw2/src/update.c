#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "sys5.h"

#ifdef TMC
#include <ctools.h>
#else
#include "ctools.h"
#endif
#include "args.h"
#include "menu.h"
#include "mem.h"

#include "rolofiles.h"
#include "rolodefs.h"
#include "datadef.h"
#include "choices.h"


char *get_new_value ()
{
  char buffer[200];
  int rval;
  switch (rval = getNewLine(stdin,buffer,199)) {
    case AT_EOF :
      user_eof();
      break;
    case TOO_MANY_CHARS :
      fprintf(stderr,"Line too long, truncated...\n");
      sleep(1);
      break;
    default :
      if ('\\' == *buffer && rval == 1) return(0);
      break;
  }
  return(copystr(buffer));
}


Ptr_Rolo_Entry copy_entry (entry) Ptr_Rolo_Entry entry;
{
  Ptr_Rolo_Entry new_entry;
  int j,n;
  char **otherfields;

  new_entry = (Ptr_Rolo_Entry) rolo_emalloc(sizeof(Rolo_Entry));

  /* copy the basic fields, but get a new timestamp */

  for (j = 0; j < N_BASIC_FIELDS - 1; j++) {
      //char* temp = copystr(get_basic_rolo_field(j,entry));
      set_basic_rolo_field(j,new_entry,copystr(get_basic_rolo_field(j,entry)));
      //free(temp);
  }
  //char* temp = timestring();
  set_basic_rolo_field(N_BASIC_FIELDS - 1,new_entry,timestring());
  //free(temp);
  /* copy the user-defined fields, if necessary */

  set_n_others(new_entry,n = get_n_others(entry));
  if (n > 0) {
     otherfields = (char **) rolo_emalloc(n * sizeof(char *));
     new_entry -> other_fields = otherfields;
     for (j = 0; j < n; j++) {
         //char* temp = copystr(get_other_field(j,entry));
         set_other_field(j,new_entry,copystr(get_other_field(j,entry)));
         //free(temp);
     }
  } else{
      new_entry -> other_fields = 0;
  }

  return(new_entry);

}


void rolo_update_mode (rlink) Ptr_Rolo_List rlink;

/* Update the fields of an entry.  The user is not allowed to modify the */
/* timestamp field. */

{
  int rval,menuval,findex,updated,newlen,n,nfields,j,name_changed;
  char *response,*s,*newfield,*newval,*other, **others;
  Ptr_Rolo_Entry entry,old_entry;

  cancel_update :

  entry = copy_entry(old_entry = get_entry(rlink));

  updated = 0;
  name_changed = 0;

  redisplay :

  display_entry_for_update(updated ? entry : old_entry);
  nfields = (N_BASIC_FIELDS - 1) + get_n_others(entry);

  reask :

  cathelpfile(libdir("updatemenu"),0,0);

  rval = menu_match (
       &menuval,&response,
       ": ",
       0,1,0,1,4,
       "\\",U_ABORT,
       "?",U_HELP,
       "Help",U_HELP,
       "",U_END_UPDATE
    );

  switch (rval) {

    case MENU_MATCH :

      switch (menuval) {

        case U_HELP :
          cathelpfile(libdir("updatehelp"),"updating",1);
          any_char_to_continue();
          clear_the_screen();
          goto redisplay;

        case U_ABORT :
          if (updated) {
             printf("Previous updates to fields in this entry ignored\n");
          }
          for (int i = 0; i <N_BASIC_FIELDS ; i++) {
               //fprintf(stderr,"%s\n",entry->basicfields[i]);
              char* temp = entry->basicfields[i];
             if (temp!=NULL||strlen(temp)>0){
                 //fprintf(stderr,"%s\n%s\n","basic_fied_before:",entry->basicfields[i]);
                 free(temp);
                 //fprintf(stderr,"%s\n%s\n","basic_fied_after:",entry->basicfields[i]);
             }

          }
          for (int i = 0; i < entry->n_others ; i++) {
              char* temp = entry->other_fields[i];
              if (temp!= NULL||strlen(temp)>0) {
                //fprintf(stderr,"%s\n%s\n","Other data:",entry->basicfields[i]);
                free(entry->other_fields[i]);
              }
          }
          free(entry->other_fields);
          free(entry);
          return;
          break;

        case U_END_UPDATE :
          if (!updated) goto reask;
          display_entry(entry);
          if (MENU_YES == rolo_menu_yes_no (
                   "Confirm Update? ",DEFAULT_YES,1,
                   "confirmhelp","confirming update"
                )) {
             printf("Update confirmed\n");
             sleep(1);
             if (name_changed) {
                rolo_delete(rlink);
                rolo_insert(rlink,compare_links);
             }
             changed = 1;
              free(old_entry->other_fields);
              free(old_entry);
              set_entry(rlink,entry);
             return;
          }
          else {
             printf("Updates ignored...\n");
             sleep(1);
             updated = 0;
              for (int i = 0; i <N_BASIC_FIELDS ; ++i) {
                  if (entry->basicfields[i]!=NULL){
                      free(entry->basicfields[i]);
                  }
              }
              for (int i = 0; i <entry->n_others ; ++i) {
                  if (entry->other_fields[i]!=NULL){
                      free(entry->other_fields[i]);
                  }
              }
              free(entry->other_fields);
              free(entry);
              //free(old_entry);
             goto cancel_update;
          }
         break;

      }
      break;

    case MENU_NO_MATCH :

      /* check that the response is an integer within range */

      findex = str_to_pos_int(response,1,nfields+1);
      if (findex < 0) {
         printf("Not a valid number...Please try again\n");
         goto reask;
      }
      findex--;

      /* we can either be updating a standard field or a user-defined field */
      /* or adding a new field */

      if (findex < N_BASIC_FIELDS - 1) {
         name_changed = (findex == 0);
         printf("Updating '%s'\n",Field_Names[findex]);
         printf("Old value: %s\n",get_basic_rolo_field(findex,entry));
         printf("New value: ");
         char *temp = get_new_value();
         if (0 == (newval = copystr(temp))){
             free(newval);
             free(temp);
             break;
         }
         free(temp);
         free(get_basic_rolo_field(findex,entry));
         set_basic_rolo_field(findex,entry,newval);
         updated = 1;
      }
      else if (findex != nfields) {
         findex -= N_BASIC_FIELDS - 1;
         printf("Updating \'");
         s = other = get_other_field(findex,entry);
         while (*s != ':') putc(*s++,stdout);
         printf("\' field\n");
         printf("Old value: %s\n",++s);
         printf("New value: ");
         char *temp = get_new_value();
         if (0 == (newval = copystr(temp))){
             free(newval);
             free(temp);
             break;
         }
         free(temp);
         if (strlen(newval) == 0) {
            for (j = findex; j < get_n_others(entry); j++) {
                set_other_field(j,entry,get_other_field(j+1,entry));
            }
            set_n_others(entry,get_n_others(entry) - 1);
         }
         else {
            *s = '\0';
            newlen = strlen(other) + strlen(newval) + 2;
            newfield = rolo_emalloc(newlen);
            nbuffconcat(newfield,3,other," ",newval);
            set_other_field(findex,entry,newfield);
            free(newval);
         }
         updated = 1;
      }
      else {
        loop:
        printf("New field (<name>: <value>): ");
        char* temp = get_new_value();
        if (0 == (newfield = copystr(temp))) {
            free(temp);
            free(newfield);
            break;
        }
        free(temp);
        if (0 == index(newfield,':')) {
           fprintf(stderr,"No field name.  Use a ':'...\n");
           goto loop;
        }
        n = get_n_others(entry);
        set_n_others(entry,n + 1);
        others = (char **) rolo_emalloc((n + 1) * sizeof(char *));
        for (j = 0; j < n; j++) others[j] = get_other_field(j,entry);
        others[n] = newfield;
        free(entry->other_fields);
        entry -> other_fields = others;
        updated = 1;
      }
      break;

    case MENU_EOF :
      free(entry->other_fields);
      free(entry);
      user_eof();
      break;

    default :
      fprintf(stderr,"Impossible return from update menu_match\n");
      free(entry->other_fields);
      free(entry);
      save_and_exit(-1);
      break;

  }
  goto redisplay;

}