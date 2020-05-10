/*Предметная область: Библиотека
Информационные объекты: Библиотека, Библиотекарь, Книги, Авторы, Посетитель
У книги может быть только один автор.
Посетитель посещает библиотеку с целью найти книгу по имени автора И/ИЛИ по названию книги.
Необходимо реализовать данный функционал, придерживаясь принципов DRY, KISS, YAGNI и Single Responsibility Principle*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Book {
private:
    string Name;
    string Author;

public:
    Book(const string &name, const string &author) {
        Name = name;
        Author = author;
    }


    string getName() {
        return Name;
    }

    string getAuthor() {
        return Author;
    }

    friend ostream& operator<<(ostream &out, const Book &book)
    {
        return out << '\"' << book.Name << "\" " << book.Author << " ";
    }

    friend bool operator==(const Book &a, const Book &b) {
        return  (a.Name == b.Name) && (a.Author == b.Author);
    }
};

class Library { // Хранит книги
private:
    vector<Book> Books;

public:
    explicit Library(vector<Book> &books) {  // рекомендуется использовать explicit
        Books = books;
    };

    ~Library() {
        Books.clear();
    };

    int find_book(Book &book) { // проверка на существование книги
        auto it = find(Books.begin(), Books.end(), book); // vector<Book>::iterator it вместо auto
        if (it != Books.end()) {
            return (it - Books.begin());
        } else {
            return -1;
        }
    }

    void find_book_by_name(string name) { // доп-но выводить номер найденной книги
        unsigned int n = 0;
        for (auto & Book : Books) {  // замена auto it = Books.begin() ; it != Books.end(); ++it
            if (Book.getName() == name) cout << "№" << n << ' ' << Book << endl;
            n++;
        }
    }

    void find_book_by_author(string author) { // доп-но выводить номер найденной книги
        unsigned int n = 0;
        for (auto & Book : Books) {
            if (Book.getAuthor() == author) cout << "№" << n << ' ' << Book << endl;
            n++;
        }
    }

    bool add_book(Book &book) { // непонятно почему рекомендуется использовать empty() вместо ""
        if ((book.getName() != "") && (book.getName() != "")) {
            Books.emplace(Books.end(), book);
            return true;
        } else {
            return false;
        }
    }

    bool del_book(Book &book) {
        int find = find_book(book);
        if (find != -1) {
            auto it = Books.begin() + find;
            Books.erase(it);
            return true;
        } else {
            return false;
        }
    };
};

class Librarian: public Library { // Хранит список отданных книг
private:
    string Name;

public:
    Librarian(const string &name, vector<Book> books)  : Library(books) {
        Name = name;
    }

    Book give_book(Book book, Library &library) {
        bool success = library.del_book(book);
        if (success){
            add_book(book);
            cout << "Возьмите эту книгу." << endl;
            return book;
        } else {
            Book nothing("","");
            cout << "Извините, такой книги нет." << endl;
            return nothing;
        }
    };

    bool return_book(Book book, Library &library) {
        bool success = library.add_book(book);
        if (success) {
            bool exist = del_book(book);
            if (exist){
                cout << "Спасибо за то, что вернули книгу." << endl;
            } else {
                cout << "Спасибо за пожертвование книги!" << endl;
            }
        }
        return success;
    }
};

class Visitor: public Library { // Хранит свои и взятые книги
private:
    string Name;

public:
    Visitor(const string &name, vector<Book> books)  : Library(books) {
        Name = name;
    }

    bool take_book(Book book, Librarian librarian, Library &library){
        book = librarian.give_book(book, library);
        bool success = add_book(book);
        return success;
    }

    Book return_book(Book book, Librarian librarian, Library &library){
        bool success = del_book(book);
        if (success) {
            cout << "Возвращаю вам книгу." << endl;
            librarian.return_book(book, library);
            return book;
        } else {
            Book nothing("","");
            cout << "Извините, но у меня нет такой книги!" << endl;
            return nothing;
        }
    }
};

int main() {
    setlocale( LC_ALL,"Russian" );

    Book book1("Алиса в Зазеркалье","Льюис Кэрролл");
    Book book2("Каштанка","Антон Чехов");
    Book book3("Лошадиная фамилия","Антон Чехов");
    Book book4("Царевна-лягушка","Валентин Берестов");
    Book book5("Похождение рубля","Сергей Михалков");
    Book book6("Ёжик в тумане","Козлов С.Г.");
    Book book7("Яблоко","Сутеев В.Г.");
    Book book8("Вредные советы","Остер Г.Б.");
    Book book9("Цветик семицветик","Катаев В.П.");
    Book book10("Последнее желание","Анджей Сапковский");

    vector<Book> Books = {book1, book2, book3};
    Library library(Books);

    if (library.find_book(book2)) cout << "Книга " << book2 << " есть" << endl << endl;
    library.find_book_by_author("Антон Чехов"); cout << endl;
    library.find_book_by_name("Ёжик в тумане"); cout << endl;
    library.find_book_by_name("Каштанка"); cout << endl;

    if (library.add_book(book4)) cout << "Книга " << book4 << " добавлена её место теперь ";
    cout << library.find_book(book4) << endl;
    if (library.del_book(book4)) cout << "Книга " << book4 << " удалена её место теперь ";
    cout << library.find_book(book4) << endl << endl;

    vector<Book> Gived_Books = {book7, book8, book9}; // список отсутствующих книг
    Librarian librarian("Артем",Gived_Books);

    librarian.return_book(book7, library);
    librarian.return_book(book10, library); cout << endl;

    cout << librarian.give_book(book2, library) << endl;
    cout << library.find_book(book2) << endl << endl;


    cout << librarian.give_book(book2, library) << endl;
    cout << librarian.give_book(book8, library) << endl;
    cout << librarian.give_book(book10, library) << endl;
    cout << library.find_book(book10) << endl << endl;

    Visitor visitor("Слава", {});
    cout << visitor.take_book(book2, librarian, library) << endl;
    cout << visitor.take_book(book3, librarian, library) << endl << endl;
    cout << visitor.return_book(book2, librarian, library) << endl;
    cout << visitor.return_book(book3, librarian, library) << endl;

    return 0;
};