#include "const.h"
#include "transplant.h"
#include "debug.h"
#include "addedfun.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/*
 * A function that returns printable names for the record types, for use in
 * generating debugging printout.
 */
static char *record_type_name(int i) {
    switch(i) {
    case START_OF_TRANSMISSION:
	return "START_OF_TRANSMISSION";
    case END_OF_TRANSMISSION:
	return "END_OF_TRANSMISSION";
    case START_OF_DIRECTORY:
	return "START_OF_DIRECTORY";
    case END_OF_DIRECTORY:
	return "END_OF_DIRECTORY";
    case DIRECTORY_ENTRY:
	return "DIRECTORY_ENTRY";
    case FILE_DATA:
	return "FILE_DATA";
    default:
	return "UNKNOWN";
    }
}

/**
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int path_init(char *name) {
    // compare length
    int len = string_length(name) + 1;
    if (len  <= PATH_MAX){
        for (int i = 0; i < len; i++){
            *(path_buf+i) = *(name+i);
        }
        path_length = len - 1;
        return 0;
    }
    return -1;
}
/**
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 *
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {
    // To be implemented.
    int length = string_length(name);
    for (int i = 0; i < length; ++i) {
        if (*(name+i)=='/'){
            return -1;
        }
    }
    //if is valid name
    int totalLength = path_length+length+1;
    if (totalLength<=PATH_MAX){
        for (int i = path_length; i <=totalLength; ++i) {
            *(path_buf+i) = *(name+i-path_length);
        }
        path_length+=length;
        return  0;
    }
    return -1;
}

/**
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {
    // To be implemented
    // check if is empty or not
    int length = string_length(path_buf);
    if (length==0){
        path_length = 0;
        return -1;
    }
    // if not empty
    // check '/'
    int lastOccur = -1;
    for (int i = length; i >=0 ; --i) {
        if (*(path_buf+i) == '/'){
            lastOccur = i;
            break;
        }
    }
    if (lastOccur == -1){
        // clear path_baf
        for (int i = 0; i <=length ; ++i) {
            *(path_buf+i) = 0;
        }
        path_length = 0;
        return  0;
    }else {
        int counter =0;
        for (int i = lastOccur+1; i <length; ++i) {
            *(path_buf+i) = 0;
            counter++;
        }
        path_length -= counter;
        return  0;
    }
    return -1;
}

/**
 * @brief Deserialize directory contents into an existing directory.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory.  It reads (from the standard input) a sequence of DIRECTORY_ENTRY
 * records bracketed by a START_OF_DIRECTORY and END_OF_DIRECTORY record at the
 * same depth and it recreates the entries, leaving the deserialized files and
 * directories within the directory named by path_buf.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * each of the records processed.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including depth fields in the records read that do not match the
 * expected value, the records to be processed to not being with START_OF_DIRECTORY
 * or end with END_OF_DIRECTORY, or an I/O error occurs either while reading
 * the records from the standard input or in creating deserialized files and
 * directories.
 */
int deserialize_directory(int depth) {
    /* To be implemented.*/
    /* read DE
     * -> CREATE DIR -> RECURSIVE CALL deserialize_directory push dir
     * -> CREATE FILE
     * if no file then pop
     * */

    // check  magic number
    if (getchar()!= MAGIC0){
        return -1;
    }
    if (getchar()!= MAGIC1){
        return -1;
    }
    if (getchar()!= MAGIC2){
        return -1;
    }
    //check data type
    int type = getchar();

    //check depth
    int actual_depth = 0;
    for (int i = 3; i >=0; --i) {
        int tempNum = getchar();
        actual_depth += tempNum << (8 * i);
    }
    if (actual_depth!=depth){
        return  -1;
    }
    //finish a pair dir
    if (type == END_OF_DIRECTORY){
        //check record size
        off_t record_size = 0;
        for (int i = 7; i >= 0; i--){
            off_t num = getchar();
            record_size += num << (8 * i);
        }
        if (record_size!=HEADER_SIZE){
            return -1;
        }else {
            return 0;
        }
    } else if (type != DIRECTORY_ENTRY){
        return  -1;
    }
    //get total size
    off_t  total_size = 0;
    for (int i = 7; i >= 0; i--) {
        off_t tempNum = getchar();
        total_size += tempNum << (8 * i);
    }
    //get name size for loop
    off_t nameSize =  total_size - HEADER_SIZE - 12;

    // determine is file or is dir
    mode_t mode = 0;
    for (int i = 3; i >= 0 ; --i) {
        mode_t tempNum = getchar();
        mode += tempNum << (8*i);
    }
    if (S_ISREG(mode)!=0){
        // is file
        // ignore size in meta data because FD will have it
        for (int i = 0; i <8 ; ++i) {
            getchar();
        }
        // get file name and ready to push it
        if (nameSize>NAME_MAX){
            //case of greater than buff
            return -1;
        }
        // clear current content
        name_clear(name_buf);
        for (off_t i = 0; i < nameSize ; ++i) {
            *(name_buf+i) = getchar();
        }
        path_push(name_buf);
#ifdef DEBUGGING
        printf("%s\n",path_buf);
#endif
        deserialize_file(depth);
        //set permission
        chmod(path_buf, mode & 0777);
        path_pop();
        //continue finish upper dir if is needed
        deserialize_directory(depth);
        return 0;
    } else if (S_ISDIR(mode)!=0){
        // case of directory
        // check exist or not
        struct stat my_stat;
        if (stat(path_buf, &my_stat)!=0 && (global_options & 8UL) ){
            return -1;
        }
        // get file name and ready to push it
        if (nameSize>NAME_MAX){
            //case of greater than buff
            return -1;
        }
        // ignore size in meta data because we dont need it
        for (int i = 0; i <8 ; ++i) {
            getchar();
        }
        // clear current content
        name_clear(name_buf);
        for (off_t i = 0; i < nameSize ; ++i) {
            *(name_buf+i) = getchar();
        }
        // push dir
        path_push(name_buf);

        //create dir
        mkdir(path_buf, 0700);

        //create path for sub directory
        path_buf[path_length++] = '/';
        path_buf[path_length] = '\0';
#ifdef DEBUGGING
        printf("%s\n",path_buf);
#endif
        //CHECK SOD
        int flag = check_header_record(START_OF_DIRECTORY,depth+1);
        if (flag==-1){
            return -1;
        }
        deserialize_directory(depth+1);
        //pop back to upper path
        path_buf[--path_length] = '\0';

        //set permission
        chmod(path_buf, mode & 0777);

        path_pop();

        deserialize_directory(depth);
        return 0;
    } else{
        //nether file nor dir
        return  -1;
    }
}

/**
 * @brief Deserialize the contents of a single file.
 * @details  This function assumes that path_buf contains the name of a file
 * to be deserialized.  The file must not already exist, unless the ``clobber''
 * bit is set in the global_options variable.  It reads (from the standard input)
 * a single FILE_DATA record containing the file content and it recreates the file
 * from the content.
 *
 * @param depth  The value of the depth field that is expected to be found in
 * the FILE_DATA record.
 * @return 0 in case of success, -1 in case of an error.  A variety of errors
 * can occur, including a depth field in the FILE_DATA record that does not match
 * the expected value, the record read is not a FILE_DATA record, the file to
 * be created already exists, or an I/O error occurs either while reading
 * the FILE_DATA record from the standard input or while re-creating the
 * deserialized file.
 */
int deserialize_file(int depth){
    //check -c flag
    struct stat my_stat;
    if (stat(path_buf, &my_stat)!=0 && (global_options & 8UL) ){
        return -1;
    }
    //start write file
    FILE *fp = fopen(path_buf, "w");

    if (!fp){
        printf("failed to open file\n");
        return -1;
    }

    //freopen("1.txt", "r", stdin);

    // check  magic number
    if (getchar()!= MAGIC0){
        return -1;
    }
    if (getchar()!= MAGIC1){
        return -1;
    }
    if (getchar()!= MAGIC2){
        return -1;
    }
    //check type
    if (getchar() != FILE_DATA){
        return -1;
    }
    //get actual depth
    int actual_depth = 0;
    for (int i = 3; i >= 0; i--){
        int num = getchar();
        actual_depth += num << (8 * i);
    }
    // check depth
    if (actual_depth != depth){
        return -1;
    }
    //get total size
    off_t record_size = 0;
    for (int i = 7; i >= 0; i--){
        off_t num = getchar();
        record_size += num << (8 * i);
    }
    //get file size
    off_t file_size = record_size - HEADER_SIZE;
    //write file
    for (off_t i = 0; i < file_size; i++){
        fputc(getchar(), fp);
    }
    //finish writing
    fclose(fp);

    return 0;
}



/**
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
//#define DEBUGGING
int serialize_directory(int depth) {
    // To be implemented.
    /*
     * print SOD
     * traverse file & dir
     * if file then print DE print FD
     * else print DE serialize_directory(depth + 1)
     * print EOD
     * */
    //start SOD RECORD
    print_sod(depth);
    //OPEN DIR
    DIR *dir = opendir(path_buf);
    //LOOP DIR
    struct dirent *de;
    while ((de = readdir(dir)) != NULL) {
        //fprintf(stderr, "%s\n", de->d_name);
        // IGNORE . AND .. DIR
        if (compare_string(de->d_name,".") == 0 || compare_string(de->d_name,"..") == 0){
            continue;
        }
        // IF IS DIR
        if (de->d_type == DT_DIR ){
            //START PRINT DE
            struct stat fileBuff;
            path_push(de->d_name);
#ifdef DEBUGGING
            fprintf(stderr, "%s (dir)\n", path_buf);
#endif

            stat(path_buf, &fileBuff);

#ifdef DEBUGGING
            fprintf(stderr,"%s %llo\n", "Dir Size",fileBuff.st_size);
#endif

            //PRINT DE
            path_buf[path_length++] = '/';
            path_buf[path_length] = '\0';
            print_de(depth, HEADER_SIZE + 12 + string_length(de->d_name), fileBuff.st_mode, fileBuff.st_size, de->d_name);
            //PUSH PATH FOR SUB DIR
            serialize_directory(depth + 1);
            //POP BACK TO UPPER DIR
            path_buf[--path_length] = '\0';
            path_pop();
        }else {
            //PRINT DE
            struct stat fileBuff;
            path_push(de->d_name);
#ifdef DEBUGGING
            fprintf(stderr, "%s (file)\n", path_buf);
#endif
            stat(path_buf, &fileBuff);
            print_de(depth, HEADER_SIZE + 12 + string_length(de->d_name), fileBuff.st_mode, fileBuff.st_size, de->d_name);
            serialize_file(depth, fileBuff.st_size);
            //POP FILE NAME
            path_pop();
        }
    }
    //END LOOP
    closedir(dir);
    //PRINT EOD FOR PAIR
    print_eod(depth);

    return 0;
}

/**
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size) {
    // To be implemented.
    //OPEN FILE IO
    FILE *fp = fopen(path_buf, "r");
    if (!fp){
        fprintf(stderr,"failed to open file\n");
        return -1;
    }
    // STAT BUFF CHECK FILE SIZE DIFF
    struct stat statbuf;
    stat(path_buf, &statbuf);

    if (statbuf.st_size != size){
        return -1;
    }

    //PRINT RECORD
    // header
    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);
    putchar(FILE_DATA);
    // depth
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);
    // size
    off_t total_size = size + HEADER_SIZE;
    putchar(total_size >> 56);
    putchar(total_size >> 48);
    putchar(total_size >> 40);
    putchar(total_size >> 32);
    putchar(total_size >> 24);
    putchar(total_size >> 16);
    putchar(total_size >> 8);
    putchar(total_size);
    // content
    for (off_t i = 0; i < size; i++){
        putchar(fgetc(fp));
    }
    //FINISH PRINT FILE
    fflush(stdout);
    // CLOSE FILE IO
    fclose(fp);
    // RETURN SUCCESS
    return 0;
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int serialize() {
    // To be implemented.
    // DEFAULT RECORD DEPTH 0
    int depth = 0;
    // INITIALIZE PATH FROM NAME BUFF
    path_init(name_buf);
    // CHECK DIR EXIST OR NOT
    DIR *dir = opendir(path_buf);
    if (!dir){
        return -1;
    }
    // PRINT SOT
    // MAGIC NUMBER
    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);
    //PRINT TYPE
    putchar(START_OF_TRANSMISSION);
    // DEPTH
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);
    //size fixed 8 byte
    for (int i = 0; i <7 ; ++i) {
        putchar(0);
    }
    //putchar(1);
    putchar(16);

    // serialize directory with depth = 1
    serialize_directory(depth+1);

    // PRINT EOT
    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);

    putchar(END_OF_TRANSMISSION);

    // DEPTH
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);

    //SIZE FIXED 8 BYTE
    for (int i = 0; i <7 ; ++i) {
        putchar(0);
    }
    //putchar(1);
    putchar(16);

    // FINISH PRINT
    fflush(stdout);
    //RETURN SUCCESS
    return 0;
}

/**
 * @brief Reads serialized data from the standard input and reconstructs from it
 * a tree of files and directories.
 * @details  This function assumes path_buf has been initialized with the pathname
 * of a directory into which a tree of files and directories is to be placed.
 * If the directory does not already exist, it is created.  The function then reads
 * from from the standard input a sequence of bytes that represent a serialized tree
 * of files and directories in the format written by serialize() and it reconstructs
 * the tree within the specified directory.  Options that modify the behavior are
 * obtained from the global_options variable.
 *
 * @return 0 if deserialization completes without error, -1 if an error occurs.
 */
int deserialize() {
    // To be implemented.
    // Check Dir Exist or Not
    struct stat statbuf;
    if (stat(name_buf,&statbuf)!= 0){
        debug("%s%s","creating dir: ",name_buf);
        mkdir(name_buf, 0700);
    }
    // CHECK SOT RECORD
    int flag = check_header_record(START_OF_TRANSMISSION,0);
    if (flag==-1){
        return -1;
    }
    //CHECK SOD
    flag = check_header_record(START_OF_DIRECTORY,1);
    if (flag==-1){
        return -1;
    }
    //INITIALIZE DIR
    path_init(name_buf);
    //DESERIALIZE FILE
    int result = deserialize_directory(1);

#ifdef DEBUGGING
    printf("finish deserialize\n");
#endif
    //CHECK EOT
    flag = check_header_record(END_OF_TRANSMISSION,0);
    if (flag==-1){
        return -1;
    }
    //FINISH
    return result;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 *
 */
/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 *
 */
//#define DEBUGING

int validargs(int argc, char **argv)
{
    // if there is no argument
    if (argc<=1){
        return -1;
    }
    //check case
#ifdef DEBUGGING
    fprintf(stderr,"%s\n", *(argv+1));
#endif
    global_options = 0x0;
    //index of -h -s ... in argv
    int h = -1, s = -1, d =-1, p =-1, c = -1;
    int other = -1;

    for (int i= 1; i < argc; ++i) {
        char * arg = *(argv + i);

        if (*arg == '-'){
            switch(*(arg+1)){
                case 'd':
                    d =i;
                    break;
                case 's':
                    s = i;
                    break;
                case 'h':
                    h=i;
                    break;
                case 'c':
                    c = i;
                    break;
                case 'p':
                    p = i;
                    break;
                default:
                    if (other ==-1){
                        other = i;
                    } else{
                        return -1;
                    }
            }
        }
        else{
            if (other ==-1){
                other = i;
            } else{
                return -1;
            }
        }
    }

    //VALID ARG
    if (h == 1 || s == 1 || d == 1){
        if (h==1){
            // CASE -H
            global_options |=1UL;
            return 0;
        } else if (s ==1){
            //CASE -S

            // -C IS NOT LEGAL
            if (c!=-1){
                return -1;
            }
            // -P MUST BECOME AFTER -S
            if (p != -1 && p != s + 1){
                return -1;
            }
            // -P MUST SPECIFIED IN ORDER TO HAVE DIR
            if (p == -1 && other != -1 ){
                return -1;
            }
            // DIR MUST BE AFTER -P
            if (p != -1 && other != p + 1){
                return -1;
            } else{
                //CASE VALID

                //SET G_O
                global_options |= 2UL;
                // COPY DIR TO NAME BUF
                if (p != -1){
                    int len = string_length(*(argv + p + 1));
                    for (int i = 0; i < len; i++){
                        *(name_buf + i) = *(*(argv + p + 1) + i);
                    }
                    if (*(name_buf + len -1) != '/'){
                        *(name_buf + len) = '/';
                        *(name_buf + len +1) = '\0';
                    } else{
                        *(name_buf + len) = '\0';
                    }
                }else{
                    //DEFAULT DIR
                    *(name_buf+0) = '.';
                    *(name_buf+1) = '/';
                    *(name_buf+2) = '\0';
                }
                //SUCCESS
                return 0;
            }
        } else{
            //CASE -D

            //SET -C FLAG
            if (c!=-1){
                global_options |=8UL;
            }
            // -P MUST EXIST WHEN DIR IS SPECIFIED
            if (p == -1 && other != -1){
                return -1;
            }
            // DIR MUST COME AFTER -P
            if (p!=-1&&other!=p+1){
                global_options = 0x0;
                return -1;
            } else{
                //VALID ARG

                //SET -D FLAG
                global_options |=4UL;

                //SET DIR TO NAME BUF
                if (p != -1){
                    int len = string_length(*(argv + p + 1));
                    for (int i = 0; i < len; i++){
                        *(name_buf + i) = *(*(argv + p + 1) + i);
                    }
                    //CHECK IF INPUT IS FULL DIR
                    if (*(name_buf + len -1) != '/'){
                        *(name_buf + len) = '/';
                        *(name_buf + len +1) = '\0';
                    } else{
                        *(name_buf + len) = '\0';
                    }
                }else{
                    //DEFAULT DIR
                    *(name_buf+0) = '.';
                    *(name_buf+1) = '/';
                    *(name_buf+2) = '\0';
                }
                //SUCCESS
                return 0;
            }
        }
    }
    else{
        return -1;
    }
}

int string_length(char* string){
    int counter = 0;
    while (*(string+counter)!='\0'){
        counter++;
    }
    return counter;
}

int compare_string(char* a, char* b)
{
    int counter = 0;
    while(*(a+counter)==*(b+counter)){
        if (*(a+counter)=='\0'||*(b+counter)=='\0'){
            break;
        }
        counter++;
    }
    // check end
    if (*(a+counter)=='\0'&&*(b+counter)=='\0'){
        /* code */
        return 0;
    }else{
        return -1;
    }
}
void print_sod(int depth){
    // create sod record
    // header
    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);

    putchar(START_OF_DIRECTORY);

    // depth
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);

    //size fixed 16
    for (int i = 0; i <7 ; ++i) {
        putchar(0);
    }
    putchar(16);

    fflush(stdout);
}

void print_eod(int depth){
    // create eod record
    // header
    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);

    putchar(END_OF_DIRECTORY);

    // depth
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);

    //size fixed 16
    for (int i = 0; i <7 ; ++i) {
        putchar(0);
    }
    //putchar(1);
    putchar(16);

    fflush(stdout);
}

void print_de(int depth, off_t total_size, mode_t mode, off_t si, char *filename){
    // create sod record
    // header

    putchar(MAGIC0);
    putchar(MAGIC1);
    putchar(MAGIC2);

    //fprintf(stderr, "de magic\n");

    putchar(DIRECTORY_ENTRY);

    // depth
    putchar(depth >> 24);
    putchar(depth >> 16);
    putchar(depth >> 8);
    putchar(depth);

    //size
    putchar(total_size >> 56);
    putchar(total_size >> 48);
    putchar(total_size >> 40);
    putchar(total_size >> 32);
    putchar(total_size >> 24);
    putchar(total_size >> 16);
    putchar(total_size >> 8);
    putchar(total_size);

    //typ/permission
    putchar(mode >> 24);
    putchar(mode >> 16);
    putchar(mode >> 8);
    putchar(mode);

    //size info
    putchar(si >> 56);
    putchar(si >> 48);
    putchar(si >> 40);
    putchar(si >> 32);
    putchar(si >> 24);
    putchar(si >> 16);
    putchar(si >> 8);
    putchar(si);

    //fileName
    int len = string_length(filename);
    for (int i = 0; i < len; ++i) {
        putchar(*(filename+i));
    }

    fflush(stdout);
}

void name_clear(char *name){
    if (string_length(name)!=0){
        int length = string_length(name);
        for (int i = 0; i < length ; ++i) {
            *(name_buf + i) = 0;
        }
    }
}

int check_header_record(int record_type, int depth){
    // check  magic number
    if (getchar()!= MAGIC0){
        return -1;
    }
    if (getchar()!= MAGIC1){
        return -1;
    }
    if (getchar()!= MAGIC2){
        return -1;
    }
    // CHECK TYPE
    int type = getchar();
    if (type!=record_type){
        return -1;
    }
    //CHECK DEPTH
    int actual_depth = 0;
    for (int i = 3; i >= 0; i--){
        int num = getchar();
        actual_depth += num << (8 * i);
    }
    if (depth!=actual_depth){
        return -1;
    }
    //CHECK SIZE
    off_t record_size = 0;
    for (int i = 7; i >= 0; i--){
        off_t num = getchar();
        record_size += num << (8 * i);
    }
    if (record_size!=HEADER_SIZE){
        return -1;
    }
    return 0;
}