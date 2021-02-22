#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

enum ERRORS {
    TO_MUCH_ARGS,
    NO_FILE_PATH,
    FILE_DOES_NOT_EXIST,
    FILE_READING_ERROR
};

string errors[] = {
    (char*)"TOO MUCH ARGS",
    (char*)"NO FILE PATH",
    (char*)"FILE DOES NOT EXIST",
    (char*)"FILE READING ERROR"
};

class CVS {

private:
    vector<string> cvs_table;

    //Нужны для вычисления уравнений
    vector<string> name_column;
    vector<string> index_row;


    //Нужны для вычисления уравнений
    vector<string> need_cacl;
    vector<string> need_cacl_key;

    //Хранит все полученные значения
    map<string, int> table;

    //Считывание данных в таблицу
    void read_table() {
        size_t size = cvs_table.size();
        
        
        //Проходим по каждой строке
        for (int i = 0; i < size; i++) {

            size_t next = 0;
            size_t prev = 0;
            bool first_readed = false;

            int column = 0;
            int row = i - 1;

            

            //Разбиваем строку
            while ((next = cvs_table.at(i).find(",", prev)) != string::npos) {
 
                string elem = cvs_table.at(i).substr(prev, next - prev);
                if ((i == 0) && first_readed) name_column.push_back(elem); //Считываем имена столбцов
                else if ((i != 0) && !first_readed) index_row.push_back(elem); //Считываем индексы строк
                else if (first_readed) read_elem(elem, name_column.at(column) + index_row.at(row)); //Добавляем элемент в таблицу
                
                if ((i != 0) && first_readed) column++;
                first_readed = true;

                prev = next + 1;
                next = next + elem.size();
            }

            //Для последней строки
            string elem = cvs_table.at(i).substr(prev);
            if ((i == 0) && first_readed) name_column.push_back(elem);
            else if ((i != 0) && !first_readed) index_row.push_back(elem);
            else if (first_readed) read_elem(elem, name_column.at(column) + index_row.at(row));
        }

        
        //Вычисляем уравнения
        while (need_cacl.size() != 0) {
            calc_elems();
        }

    }

    //Добавляет элемент в балицу или уравнение в массив
    void read_elem(string elem, string key) {
        try {
            int number = stoi(elem);
            table.insert(make_pair(key, number));
        }
        catch (const std::invalid_argument&) {
            need_cacl.push_back(elem);
            need_cacl_key.push_back(key);
        }
    }

    //Вычисляет все элементы, сохраняя те из них, которые не удалось решить
    void calc_elems() {

        size_t size = need_cacl.size();

        vector<string> need_cacl_tmp;
        vector<string> need_cacl_key_tmp;

        //Проход по всем уравнениям
        for (size_t i = 0, j = 0; i < size; i++) {

            const char* equation = need_cacl.at(i).c_str();

            string key1;
            string key2;

            size_t length = need_cacl.at(i).length();

            size_t start_key = 0;
            char op = '\0';

            //Считывается ключ1 и ключ2 а также оператор *,+,-,/
            while (j < length) {
                switch (equation[j])
                {
                    case '=': {
                        j++;
                        continue;
                        break;
                    }

                    case '+': {
                        key1 = need_cacl.at(i).substr(start_key, j - 1);
                        op = equation[j];
                        start_key = 0;
                        break;
                    }

                    case '-': {
                        key1 = need_cacl.at(i).substr(start_key, j - 1);
                        op = equation[j];
                        start_key = 0;
                        break;
                    }

                    case '*': {
                        key1 = need_cacl.at(i).substr(start_key, j - 1);
                        op = equation[j];
                        start_key = 0;
                        break;
                    }

                    case '/': {
                        key1 = need_cacl.at(i).substr(start_key, j - 1);
                        op = equation[j];
                        start_key = 0;
                        break;
                    }

                    default: {
                        if (start_key == 0) start_key = j;
                    }
                }
                j++;
                if (j == length - 1) key2 = need_cacl.at(i).substr(start_key, j);
                
            }

            map <string, int> ::iterator it;
            bool is_cacl = false;

            //Вычисление уравнения
            switch (op)
            {

            case '+': {
                it = table.find(key1);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k1 = it->second;

                it = table.find(key2);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k2 = it->second;

                table.insert(make_pair(need_cacl_key.at(i), k1 + k2));

                is_cacl = true;
                break;
            }

            case '-': {
                it = table.find(key1);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k1 = it->second;

                it = table.find(key2);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k2 = it->second;


                table.insert(make_pair(need_cacl_key.at(i), k1 - k2));

                is_cacl = true;
                break;
            }

            case '*': {
                it = table.find(key1);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k1 = it->second;

                it = table.find(key2);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k2 = it->second;

                table.insert(make_pair(need_cacl_key.at(i), k1 * k2));

                is_cacl = true;
                break;
            }

            case '/': {
                it = table.find(key1);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k1 = it->second;

                it = table.find(key2);
                if (it == table.end()) {
                    is_cacl = false;
                    break;
                }
                int k2 = it->second;


                table.insert(make_pair(need_cacl_key.at(i), k1 / k2));

                is_cacl = true;
                break;
            }

            }

            if (!is_cacl) {
                need_cacl_tmp.push_back(need_cacl.at(i));;
                need_cacl_key_tmp.push_back(need_cacl_key.at(i));;
            }

        }

        //Сохранение уравнений, которые не удалось решить
        need_cacl.swap(need_cacl_tmp);
        need_cacl_key.swap(need_cacl_key_tmp);

    }


public:

    //Считывает файл и просчитывает таблицу
    bool read_file(ifstream* file) {
        
        string buff;
        while (getline(*file, buff)) {
            cout << buff << endl;
            cvs_table.push_back(buff);
        }
        read_table();
        return true;
    }

    //Печатает таблицу на консоль
    int print_table() {
        
        map <string, int> ::iterator it;
        size_t columns = name_column.size();
        size_t rows = index_row.size();
        
        cout << endl;

        for (int j = 0; j < columns; j++) {
            cout << "," << name_column.at(j);
        }
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << index_row.at(i);
            for (int j = 0; j < columns; j++) {
                it = table.find(name_column.at(j) + index_row.at(i));
                cout << "," << it->second;
            }
            cout << endl;
        }

    }




};



int print_err(int err_code) {

    cout << endl << endl << "Program exit with error." << endl;
    cout << "Error_code = [" << err_code << "]" << endl;
    cout << errors[err_code] << endl << endl;

    cout << "Usage: CVS.exe <file_path>" << endl;
    cout << "<file_path> example c:\\folder\\file.cvs" << endl << endl;

    cout << "Press any key to exit" << endl;
    int i;
    cin >> i;

    return err_code;
}

int main(int argc, char* argv[])
{
    
    ifstream file;
    CVS cvs;

    if (argc > 2) return print_err(TO_MUCH_ARGS);

    if (argc == 2) {
        cout << argv[1];
        file.open((char*) argv[1]);
    }
    else {
        cout << "Enter file path: ";
        string file_name = "";
        cin >> file_name;
        if (file_name.empty()) return print_err(NO_FILE_PATH);
        file.open(file_name);
    }

    if (!file.is_open()) return print_err(FILE_DOES_NOT_EXIST);

    cout << "File is open" << endl;

    cout << "Reading CVS table" << endl << endl;

    if (!cvs.read_file(&file)) return print_err(FILE_READING_ERROR);

    cout << endl << "Success Reading CVS table" << endl;

    cout << endl << "Print CVS table" << endl;

    cvs.print_table();


    return 0;

}