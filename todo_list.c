#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#define MAX_TASKS 100
#define TASK_SIZE 256

char tasks[MAX_TASKS][TASK_SIZE];
int task_count = 0;
const char* FILENAME = "tasks.txt";
const char* TEMP_FILENAME = "tasks_temp.txt";
int change_count = 0;
const int BACKUP_INTERVAL = 10;

void addTask(const char* task);
void removeTask(int index);
void showTasks();
void saveTasks(void);
void loadTasks();
void logMessage(const char *filename, const char *message, ...);

void addTask(const char* task)
{
    if (task_count >= MAX_TASKS)
    {
        printf("Task list is full!\n");
        return;
    }

    strncpy(tasks[task_count], task, TASK_SIZE);
    tasks[task_count][TASK_SIZE - 1] = '\0';
    task_count++;
    change_count++;

    saveTasks();
}

void removeTask(int index)
{
    if (index < 0 || index >= task_count)
    {
        printf("Invalid task index.\n");
        return;
    }

    for (int i = index; i < task_count - 1; i++)
    {
        strcpy(tasks[i], tasks[i + 1]);
    }

    task_count--;
    change_count++;
}

void showTasks()
{
    for (int i = 0; i < task_count; i++)
    {
        printf("%d. %s\n", i + 1, tasks[i]);
    }
}

void saveTasks() 
{
    printf("Opening file...\n");
    FILE *tempFile = fopen(TEMP_FILENAME, "w");
    if (tempFile == NULL)
    {
        perror("Failed to open temporary file for writing");
        return;
    }

    printf("Writing to file...\n");
    for (int i = 0; i < task_count; i++)
    {
        if (fprintf(tempFile, "%s\n", tasks[i]) < 0) 
        {
            perror("Failed to write to temporary file");
            fclose(tempFile);
            return;
        }
    }

    printf("Closing file...\n");
    if (fclose(tempFile) != 0) 
    {
        perror("Error closing temporary file after writing");
        return;
    }

    if (remove(FILENAME) != 0 && errno != ENOENT)
    {
        perror("Failed to remove the original file");
        return;
    }

    printf("Renaming file...\n");
    if (rename(TEMP_FILENAME, FILENAME) != 0)
    {
        perror("Error replacing the original file with temporary file");
        return;
    }

    if (change_count >= BACKUP_INTERVAL)
    {
        char backupFilename[TASK_SIZE];
        sprintf(backupFilename, "%s_backup", FILENAME);

        FILE *backupFile = fopen(backupFilename, "w");
        if (backupFile == NULL)
        {
            perror("Failed to open backup file for writing");
            return;
        }

        for (int i = 0; i < task_count; i++)
        {
            if (fprintf(backupFile, "%s\n", tasks[i]) < 0)
            {
                perror("Failed to write to backup file");
                fclose(backupFile);
                return;
            }
        }

        fclose(backupFile);
        printf("Backup saved successfully.\n");

        change_count = 0;
    }
}


void loadTasks()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            printf("No existing task file found. A new one will be created when adding tasks.\n");
        } else {
            perror("Error opening file");
        }
        return;
    }

    char buffer[TASK_SIZE];
    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) > 0) {
            addTask(buffer);
        }
    }

    if (ferror(file))
    {
        perror("Error reading from file");
    }

    if (fclose(file) != 0)
    {
        perror("Error closing file after reading");
    }
}

void logMessage(const char *filename, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    FILE *file = fopen(filename, "a");
    if (file != NULL)
    {
        time_t now = time(NULL);
        char *date = ctime(&now);
        date[strlen(date) - 1] = '\0';

        fprintf(file, "%s: ", date);
        vfprintf(file, message, args);
        fprintf(file, "\n");
        fclose(file);
    } else {
        perror("Failed to open log file");
    }
    va_end(args);
}

// gcc -o todo_list todo_list.c
int main() {
    char input[TASK_SIZE];
    int choice, index;

    loadTasks();

    while (1) 
    {
        printf("1. Add Task\n2. Remove Task\n3. Show Tasks\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) 
        {
        case 1:
            printf("Enter a task: ");
            fgets(input, TASK_SIZE, stdin);
            input[strcspn(input, "\n")] = 0;
            addTask(input);
            break;
        case 2:
            showTasks();
            printf("Enter task number to remove: ");
            scanf("%d", &index);
            removeTask(index - 1);
            break;
        case 3:
            showTasks();
            break;
        case 4:
            printf("Exiting program.\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    logMessage("app.log", "This is a log message: %s", "Hello, logging world!");
    return 0;
}