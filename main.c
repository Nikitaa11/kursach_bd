#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 20
#define MAX_NAME_LENGTH 50
#define MAX_EXAMS 5
#define MAX_TESTS 10
#define MAX_FILENAME_LENGTH 100

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int result;
    Date date;
} Exam;

typedef struct {
    int result;
    Date date;
} Test;

typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char surName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];
    Exam exams[MAX_EXAMS];
    Test tests[MAX_TESTS];
    Date birthDate;
    int examsCount;
    int testCount;
} Student;

typedef struct {
    Student* students[MAX_STUDENTS];
    int studentsCount;
} Group;

Group group;
char databaseFileName[MAX_FILENAME_LENGTH];

void addStudent() {
    if (group.studentsCount >= MAX_STUDENTS) {
        printf("\nОшибка! Достигнуто максимальное количество студентов в группе!\n");
        return;
    }

    Student* student = malloc(sizeof(Student));

    char surName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];

    printf("Введите ФИО студента в формате >> Фамилия Имя Отчество: ");
    scanf("%s %s %s", surName, firstName, patronymic);
    for (int i = 0; i < group.studentsCount; i++) {
        Student* student = group.students[i];

        if (strcmp(student->surName, surName) == 0 && strcmp(student->firstName, firstName) == 0 &&
            strcmp(student->patronymic, patronymic) == 0) {
            printf("Этот студент был добавлен раннее!\n");
            return;
        }
    }
    strcpy(student->surName, surName);
    strcpy(student->firstName, firstName);
    strcpy(student->patronymic, patronymic);

    printf("Введите дату рождения студента в формате >> день.месяц.год: ");
    scanf("%d.%d.%d", &student->birthDate.day, &student->birthDate.month, &student->birthDate.year);

    printf("Введите количество зачетов (не более %d): ", MAX_TESTS);
    if (scanf("%d", &student->testCount) != 1 || student->testCount < 0 || student->testCount > MAX_TESTS) {
        printf("Ошибка! Некорректное количество зачетов!\n");
        printf("Создавайте студента заново\n");
        free(student);
        return;
    }

    for (int i = 0; i < student->testCount; i++) {
        printf("Введите результат зачета %d (1 - зачет, 0 - незачет): ", i + 1);
        if (scanf("%d", &student->tests[i].result) != 1 || (student->tests[i].result != 0 && student->tests[i].result != 1)) {
            printf("Ошибка! Некорректный результат зачета!\n");
            printf("Создавайте студента заново\n");

            for (int j = 0; j < i; j++) {
                free(&student->tests[j]);
            }
            free(student);
            return;
        }

        printf("Введите дату сдачи зачета %d в формате >> день.месяц.год: ", i + 1);
        scanf("%d.%d.%d", &student->tests[i].date.day, &student->tests[i].date.month, &student->tests[i].date.year);
    }

    printf("Введите количество экзаменов (не более %d): ", MAX_EXAMS);
    if (scanf("%d", &student->examsCount) != 1 || student->examsCount < 0 || student->examsCount > MAX_EXAMS) {
        printf("Ошибка! Некорректное количество экзаменов!\n");
        printf("Создавайте студента заново\n");
        free(student);
        return;
    }

    for (int i = 0; i < student->examsCount; i++) {
        printf("Введите оценку за экзамен %d (от 2 до 5): ", i + 1);
        if (scanf("%d", &student->exams[i].result) != 1 || student->exams[i].result < 2 || student->exams[i].result > 5) {
            printf("Ошибка! Некорректный результат экзамена!\n");
            printf("Создавайте студента заново\n");

            for (int j = 0; j < i; j++) {
                free(&student->exams[j]);
            }
            free(student);
            return;
        }

        printf("Введите дату сдачи экзамена %d в формате >> день.месяц.год: ", i + 1);
        scanf("%d.%d.%d", &student->exams[i].date.day, &student->exams[i].date.month, &student->exams[i].date.year);
    }

    group.students[group.studentsCount++] = student;

    printf("\nСтудент успешно добавлен!\n");
}

void printStudentInfo(Student* student) {

    printf("ФИО: %s %s %s\n", student->surName, student->firstName, student->patronymic);
    printf("Дата рождения: %d.%d.%d\n", student->birthDate.day, student->birthDate.month, student->birthDate.year);

    printf("Сданные зачеты:\n");
    for (int i = 0; i < student->testCount; i++) {
        printf("Зачет %d: %s\n", i + 1, student->tests[i].result ? "Зачет" : "Незачет");
        printf("Дата сдачи: %d.%d.%d\n", student->tests[i].date.day, student->tests[i].date.month, student->tests[i].date.year);
    }

    printf("Экзамены:\n");
    for (int i = 0; i < student->examsCount; i++) {
        printf("Оценка за экзамен %d: %d\n", i + 1, student->exams[i].result);
        printf("Дата сдачи: %d.%d.%d\n", student->exams[i].date.day, student->exams[i].date.month, student->exams[i].date.year);
    }
}


void printDatabase() {
    printf("\nИнформация о найденных студентах:\n\n");

    for (int i = 0; i < group.studentsCount; i++) {
        printf("Студент %d:\n", i + 1);
        printStudentInfo(group.students[i]);
        printf("\n");
    }
}


void searchStudentByName() {
    char surName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];

    printf("Введите ФИО студента в формате >> Фамилия Имя Отчество: ");
    scanf("%s %s %s", surName, firstName, patronymic);

    int fl = 0;
    for (int i = 0; i < group.studentsCount; i++) {
        Student* student = group.students[i];

        if (strcmp(student->surName, surName) == 0 && strcmp(student->firstName, firstName) == 0 &&
            strcmp(student->patronymic, patronymic) == 0) {
            printf("Информация о студенте:\n");
            printStudentInfo(student);
            fl = 1;
            break;
        }
    }

    if (!fl) {
        printf("\nСтудент не найден!\n");
    }
}


int compareStudents(const void* a, const void* b) {
    Student* studentA = *(Student**)a;
    Student* studentB = *(Student**)b;
    return strcmp(studentA->surName, studentB->surName);
}

void sortStudentBySurname() {
    qsort(group.students, group.studentsCount, sizeof(Student*), compareStudents);
}


void saveDatabase() {
    FILE* file = fopen(databaseFileName, "wb");

    if (file == NULL) {
        printf("\nОшибка при сохранении базы данных!\n");
        return;
    }

    fwrite(&group.studentsCount, sizeof(int), 1, file);

    for (int i = 0; i < group.studentsCount; i++) {
        fwrite(group.students[i], sizeof(Student), 1, file);
    }

    fclose(file);
    printf("\nБаза данных успешно сохранена!\n");
}

void loadDatabase() {
    FILE* file = fopen(databaseFileName, "rb");

    if (file == NULL) {
        printf("\nОшибка при загрузке базы данных!\n");
        return;
    }

    fread(&group.studentsCount, sizeof(int), 1, file);

    for (int i = 0; i < group.studentsCount; i++) {
        Student* student = malloc(sizeof(Student));
        fread(student, sizeof(Student), 1, file);
        group.students[i] = student;
    }

    fclose(file);

    printf("\nБаза данных успешно загружена!\n");
}

void deleteStudent() {
    if (group.studentsCount == 0) {
        printf("\nГруппа студентов пуста!\n");
        return;
    }

    char surName[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char patronymic[MAX_NAME_LENGTH];

    printf("Введите ФИО студента в формате >> Фамилия Имя Отчество: ");
    scanf("%s %s %s", surName, firstName, patronymic);
    int fl = 0;

    for (int i = 0; i < group.studentsCount; i++) {
        Student* student = group.students[i];

        if (strcmp(student->surName, surName) == 0 && strcmp(student->firstName, firstName) == 0 &&
            strcmp(student->patronymic, patronymic) == 0) {
            free(student);

            for (int j = i; j < group.studentsCount - 1; j++) {
                group.students[j] = group.students[j + 1];
            }

            group.studentsCount--;

            printf("\nСтудент успешно удален!\n");
            fl = 1;
            break;
        }
    }

    if (!fl) {
        printf("\nСтудент не найден!\n");
    }
}

void printMenu() {
    printf("\n>>>>>>>>>>> Меню <<<<<<<<<<<\n");
    printf("1 >> Добавить студента\n");
    printf("2 >> Найти студента по ФИО\n");
    printf("3 >> Сортировка студентов по фамилии\n");
    printf("4 >> Вывести найденную информацию из базы данных\n");
    printf("5 >> Удалить студента\n");
    printf("6 >> Сохранить базу данных\n");
    printf("7 >> Загрузить базу данных\n");
    printf("8 >> Завершить работу программы\n");
    printf("-----------------------\n");
    printf("Введите номер операции: ");
}

int main() {
    group.studentsCount = 0;

    printf("Введите имя файла с существующей базой данных или придумайте имя для новой: ");
    scanf("%s", databaseFileName);

    int choice = 0;

    while (choice != 8) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                searchStudentByName();
                break;
            case 3:
                sortStudentBySurname();
                break;
            case 4:
                printDatabase();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                saveDatabase();
                break;
            case 7:
                loadDatabase();
                break;
            case 8:
                printf("\nВыход из программы\n");
                break;
            default:
                printf("\nОшибка! Неверный номер операции!\n");
        }
    }

    return 0;
}
