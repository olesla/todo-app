#include <iostream>
#include <string>
#include "thirdparty/SQLiteCpp/include/SQLiteCpp/Database.h"
#include "thirdparty/SQLiteCpp/include/SQLiteCpp/Statement.h"
#include "thirdparty/SQLiteCpp/include/SQLiteCpp/Column.h"
#include "todo.h"

//CREATE TABLE tasks(
//    id INTEGER PRIMARY KEY AUTOINCREMENT,
//    description            TEXT    NOT NULL,
//    complete               INT     NOT NULL DEFAULT 0
//);

void printTodo(SQLite::Statement &q) {
    auto id = q.getColumn(0);
    auto description = q.getColumn(1);
    auto complete = q.getColumn(2);

    std::cout   << "id: " << id
                << ", description: " << description
                << ", completed: " << complete
                << std::endl;
}

void all(SQLite::Database &db, bool withCompleted = false) {
    bool empty = true;
    std::string sql;

    if (withCompleted) {
        sql = "select * from tasks order by id desc";
    } else {
        sql = "select * from tasks where complete = 0 order by id desc";
    }

    SQLite::Statement q(db, sql);

    while (q.executeStep()) {
        empty = !empty ?: false;
        printTodo(q);
    }

    if (empty) {
        std::cout << "No todos!" << std::endl;
    }
}

void add(SQLite::Database &db) {
    std::string description;
    bool success = false;

    std::cin.ignore();
    std::cout << "Description: ";
    std::getline(std::cin, description);

    try {
        auto sql = "insert into tasks (description) values (\""+description+"\")";
        SQLite::Statement q(db, sql);
        q.exec();
        success = true;
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    if (success) {
        std::cout   << "Added task: " << description << std::endl;
    }
}

Task getTask(SQLite::Database &db, int id) {
    Task task;

    try {
        SQLite::Statement q(db, "select * from tasks where id = ?");
        q.bind(1, id);
        q.executeStep();
        task.id = id;
        task.description = q.getColumn(1).getString();
        task.completed = q.getColumn(2).getInt();

        return task;
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
        task.id = 0;
        return task;
    }
}

void edit(SQLite::Database &db) {
    all(db);
    int id;
    std::string description;

    std::cout << "ID: ";
    std::cin >> id;

    std::cin.ignore();
    std::cout << "Description: ";
    std::getline(std::cin, description);

    auto task = getTask(db, id);
    if (task.id == 0) {
        std::cout << "Task not found" << std::endl;
        return;
    }

    try {
        SQLite::Statement q(db, "update tasks set description = ? where id = ?");
        q.bind(1, description);
        q.bind(2, id);
        q.exec();
        std::cout << "Task was updated" << std::endl;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void remove(SQLite::Database &db) {
    all(db);
    int id;

    std::cout << "ID: ";
    std::cin >> id;

    auto task = getTask(db, id);
    if (task.id == 0) {
        std::cout << "Task not found" << std::endl;
        return;
    }

    try {
        SQLite::Statement q(db, "delete from tasks where id = ?");
        q.bind(1, id);
        q.exec();
        std::cout << "Task was removed" << std::endl;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void search(SQLite::Database &db) {
    std::string search;

    std::cout << "Search string: ";
    std::cin.ignore();
    std::getline(std::cin, search);

    bool empty = true;
    SQLite::Statement q(db, "select * from tasks where description like ?");
    q.bind(1, search);
    while (q.executeStep()) {
        empty = !empty ? : false;
        printTodo(q);
    }
    if (empty) {
        std::cout << "No search results" << std::endl;
    }
}

void complete(SQLite::Database &db) {
    all(db);
    int id;

    std::cout << "ID: ";
    std::cin >> id;

    auto task = getTask(db, id);
    if (task.id == 0) {
        std::cout << "Task not found" << std::endl;
        return;
    }

    try {
        SQLite::Statement q(db, "update tasks set complete = 1 where id = ?");
        q.bind(1, id);
        q.exec();
        std::cout << "Task was completed" << std::endl;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    SQLite::Database db("todo.sqlite", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    int choice = 0;

    do {
        std::cout   << "1. All todos\n"
                    << "2. All incomplete todos\n"
                    << "3. Add\n"
                    << "4. Edit\n"
                    << "5. Complete\n"
                    << "6. Remove\n"
                    << "7. Search\n"
                    << "0. Exit\n";

        std::cin >> choice;

        switch (choice) {
            case 1: all(db, true); break;
            case 2: all(db); break;
            case 3: add(db); break;
            case 4: edit(db); break;
            case 5: complete(db); break;
            case 6: remove(db); break;
            case 7: search(db); break;
            case 0: return 0;
            default: std::cout << "Invalid option, try again" << std::endl; break;
        }
    } while (choice != 0);
}