#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//--------------------------------------------------------
const int MAX_STR_LEN = 100;
typedef struct 
{
   int num_of_line;
   char **_string;
} Text;
//--------------------------------------------------------
FILE* open_file();
void check_allocation(void *ptr);
void fill_struct(Text *text, FILE *file);
void free_text(Text *text);
bool read_line_into_struct(Text *text, FILE *file, int index);
bool read_line_count(FILE *file, Text *text);
void allocate_string_array(Text *text);
void quick_sort(Text *text, int low, int high);
int partition(Text *text, int low, int high);
int get_first_number(const char *line);
void print_text_to_console(const Text *text);
//--------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error\n");
        return 1;
    }
    Text *text = malloc(sizeof(Text));
    check_allocation(text);
    FILE *input_file = open_file();
    if (input_file == NULL) 
    {
       free_text(text); 
        return 1;
    }
    fill_struct(text, input_file);
    fclose(input_file);
    quick_sort(text, 0, text->num_of_line - 1);
    print_text_to_console(text);
    free_text(text);

return 0;
}

//--------------------------------------------------------
// פונקציה לפתיחת קובץ
FILE* open_file()
{
    FILE *input_file = fopen("ex0.txt", "r");
    if (input_file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }
    return input_file;
}
//--------------------------------------------------------
bool read_line_count(FILE *file, Text *text) 
{
    if (fscanf(file, "%d\n", &text->num_of_line) != 1) 
    { // הוספת '\n' ב- fscanf כדי לדלג על סוף השורה
        printf("Error reading line count\n");
        text->num_of_line = 0;
        return false;
    }

    return true;
}

//--------------------------------------------------------
// פונקציה להקצאת זיכרון עבור מערך השורות
void allocate_string_array(Text *text) 
{
    text->_string = malloc(text->num_of_line * sizeof(char *));
    check_allocation(text->_string);
}
//--------------------------------------------------------
// פונקציה למילוי המבנה Text בעזרת הפונקציות הנפרדות
void fill_struct(Text *text, FILE *file) 
{
    // קריאת מספר השורות
    if (!read_line_count(file, text)) 
    {
        return; // יציאה אם לא הצלחנו לקרוא את מספר השורות
    }
    // הקצאת זיכרון למערך השורות
    allocate_string_array(text);
    // לולאה לקריאת כל שורה למערך השורות
    for (int i = 0; i < text->num_of_line; i++)
     {
        if (!read_line_into_struct(text, file, i)) 
        {
            // אם הקריאה נכשלה, התוכנית תשחרר את הזיכרון ותצא
            printf("Failed to read line %d\n", i + 1);
            text->num_of_line = i; // עדכון מספר השורות שהוקצו בפועל
            break;
        } 
    }
}

//--------------------------------------------------------
bool read_line_into_struct(Text *text, FILE *file, int index)
 {
    char line[MAX_STR_LEN];
    while (fgets(line, MAX_STR_LEN, file) != NULL) 
    {
        // אם השורה אינה ריקה, נמשיך לעיבוד השורה
        if (line[0] != '\n' && line[0] != '\0') 
        {
            size_t len = strlen(line);

            // הוספת '\n' אם חסר בסוף השורה
            if (len > 0 && line[len - 1] != '\n') 
            {
                line[len] = '\n';
                line[len + 1] = '\0';
            }

            // הקצאת זיכרון לאחסון השורה במבנה Text
            text->_string[index] = malloc((strlen(line) + 1) * sizeof(char));
            check_allocation(text->_string[index]);
            // העתקת השורה למבנה Text
            strcpy(text->_string[index], line);
            return true; // השורה נקראה בהצלחה
        }
        // אחרת, השורה ריקה ונחזור ללולאה לקרוא את השורה הבאה
    }
    // אם הגענו לכאן, הקובץ נגמר לפני שמצאנו מספיק שורות
    printf("Error: not enough lines in file\n");
    return false;
}

//--------------------------------------------------------
// פונקציה לבדיקת הקצאת זיכרון
void check_allocation(void *ptr)
{
    if (ptr == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
}
//--------------------------------------------------------
void free_text(Text *text)
{
    // שחרור כל מחרוזת במערך _string
    for (int i = 0; i < text->num_of_line; i++)
    {
        free(text->_string[i]);
    }
    // שחרור המערך _string עצמו
    free(text->_string);
    // שחרור מבנה text
    free(text);
        }
//--------------------------------------------------------
// פונקציה למיון מהיר של מערך השורות לפי המספרים בתחילתן
void quick_sort(Text *text, int low, int high)
{
    if (low < high) 
    {
        int pi = partition(text, low, high);
        quick_sort(text, low, pi - 1);  // לפני ה-pivot
        quick_sort(text, pi + 1, high); // אחרי ה-pivot
    }
}
//--------------------------------------------------------
int partition(Text *text, int low, int high)
{
    int pivot = get_first_number(text->_string[high]); // בחר את ה-pivot
    int i = (low - 1); // מצביע על המקום האחרון של הקטנים מה-pivot

    for (int j = low; j < high; j++) 
    {
        // אם המספר הראשון בשורה הנוכחית קטן או שווה ל-pivot
        if (get_first_number(text->_string[j]) <= pivot) 
        {
            i++;
            // החלפה בין text->_string[i] ו-text->_string[j]
            char *temp = text->_string[i];
            text->_string[i] = text->_string[j];
            text->_string[j] = temp;
        }
}

    // החלפה בין text->_string[i + 1] ו-text->_string[high] (pivot)
    char *temp = text->_string[i + 1];
    text->_string[i + 1] = text->_string[high];
    text->_string[high] = temp;

    return i + 1;
}
//--------------------------------------------------------
// פונקציה לקבלת המספר הראשון במחרוזת
int get_first_number(const char *line)
{
    int num;
    sscanf(line, "%d", &num);
    return num;
}
//--------------------------------------------------------
void write_text_to_file(const char *filename, const Text *text) 
{
    FILE *output_file = fopen(filename, "w");
    if (output_file == NULL) 
    {
        printf("Error opening file for writing\n");
        return;
    }

    // כתיבת מספר השורות בתחילת הקובץ
    fprintf(output_file, "%d\n", text->num_of_line);

    // כתיבת כל שורה מהמערך
    for (int i = 0; i < text->num_of_line; i++) 
    {
        fprintf(output_file, "%s", text->_string[i]);
    }

    fclose(output_file);
}
//--------------------------------------------------------
void print_text_to_console(const Text *text)
 {
    printf("%d\n", text->num_of_line);

    for (int i = 0; i < text->num_of_line; i++) 
    {
        printf("%s", text->_string[i]);
    }
}