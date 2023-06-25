#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

const string FILE_NAME = "passwords.txt";
const int KEY_SHIFT = 3;

// Функция для генерации случайного пароля
string generatePassword(int length)
{
    const string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password;

    srand(time(0));

    for (int i = 0; i < length; i++)
    {
        int index = rand() % CHARACTERS.length();
        password += CHARACTERS[index];
    }

    return password;
}
// Функция для шифрования текста методом XOR
string encryptXOR(const string& text, char key)
{
    string encryptedText = text;

    for (int i = 0; i < encryptedText.length(); i++)
    {
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',') // Исключаем символы 'l', 'L' и ','
        {
            encryptedText[i] = text[i] ^ key;
        }
    }

    return encryptedText;
}

// Функция для дешифрования текста методом XOR
string decryptXOR(const string& encryptedText, char key)
{
    string decryptedText = encryptedText;

    for (int i = 0; i < decryptedText.length(); i++)
    {
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',') // Исключаем символы 'l', 'L' и ','
        {
            decryptedText[i] = encryptedText[i] ^ key;
        }
    }

    return decryptedText;
}

// Функция для шифрования текста методом сдвига ASCII-кодов
string encryptShift(const string& text, int shift)
{
    string encryptedText = text;

    for (int i = 0; i < encryptedText.length(); i++)
    {
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',') // Исключаем символы 'l', 'L' и ','
        {
            encryptedText[i] = (text[i] + shift) % 256;
        }
    }

    return encryptedText;
}

// Функция для дешифрования текста методом сдвига ASCII-кодов
string decryptShift(const string& encryptedText, int shift)
{
    string decryptedText = encryptedText;

    for (int i = 0; i < decryptedText.length(); i++)
    {
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',') // Исключаем символы 'l', 'L' и ','
        {
            decryptedText[i] = (encryptedText[i] - shift + 256) % 256;
        }
    }

    return decryptedText;
}
// Функция для добавления новых данных в файл
void addData()
{
    string login;
    string password;
    string serviceName;
    cin.ignore(); // Очистка буфера ввода

    cout << "Введите логин: ";
    getline(cin, login);

    cout << "Введите пароль или оставьте поле пустым для случайной генерации: ";
    getline(cin, password);

    if (password.empty())
    {
        cout << "Введите длину пароля (по умолчанию 10): ";
        string passwordLengthInput;
        getline(cin, passwordLengthInput);

        int passwordLength = 10; // Значение по умолчанию

        if (!passwordLengthInput.empty())
        {
            try
            {
                passwordLength = stoi(passwordLengthInput);
            }
            catch (const std::exception& e)
            {
                cout << "Некорректное значение длины пароля. Используется значение по умолчанию (10)." << endl;
            }
        }

        if (passwordLength <= 0)
        {
            cout << "Некорректное значение длины пароля. Используется значение по умолчанию (10)." << endl;
            passwordLength = 10;
        }

        password = generatePassword(passwordLength);
        cout << "Сгенерированный пароль: " << password << endl;
    }

    cout << "Введите название сервиса: ";
    getline(cin, serviceName);

    ofstream file(FILE_NAME, ios::app);

    if (file.is_open())
    {
        char key = 'k'; // Ключ для XOR-шифрования

        string encryptedLogin = encryptShift(encryptXOR(login, key), KEY_SHIFT);
        string encryptedPassword = encryptShift(encryptXOR(password, key), KEY_SHIFT);
        string encryptedServiceName = encryptShift(encryptXOR(serviceName, key), KEY_SHIFT);

        file << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

        file.close();

        cout << "Данные успешно добавлены." << endl;
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}
void viewData()
{
    ifstream file(FILE_NAME);

    if (file.is_open())
    {
        char key = 'k'; // Ключ для XOR-шифрования

        string line;
        int lineNumber = 1;

        cout << "Доступные сервисы:" << endl;
        string decryptedServiceName;
        bool isEmpty = true; // Флаг, указывающий на пустой файл

        while (getline(file, line))
        {
            isEmpty = false; // Файл не является пустым

            string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

            size_t commaPos1 = decryptedLine.find(",");
            size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

            string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);
            size_t serviceNameEnd = decryptedServiceName.find(",");
            if (serviceNameEnd != string::npos) {
                decryptedServiceName = decryptedServiceName.substr(0, serviceNameEnd);
            }
            cout << lineNumber << ". " << decryptedServiceName << endl;

            lineNumber++;
        }

        file.close();

        if (isEmpty) {
            cout << "Файл пустой." << endl;
            return; // Программа завершает выполнение
        }

        int selectedLine;

        cout << "Введите номер сервиса, чтобы просмотреть логин и пароль: ";
        std::cin >> selectedLine;

        lineNumber = 1;
        file.open(FILE_NAME); // Переоткрываем файл для поиска выбранной строки

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (lineNumber == selectedLine)
                {
                    string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedLogin = decryptedLine.substr(0, commaPos1);
                    string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);
                    cout << "Логин: " << decryptedLogin << endl;
                    cout << "Пароль: " << decryptedPassword << endl;
                    cout << "Сервис: " << decryptedServiceName << endl;

                    break;
                }

                lineNumber++;
            }

            file.close();
        }
        else
        {
            cout << "Ошибка при открытии файла." << endl;
        }
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

void editData()
{
    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");
    if (file.is_open() && tempFile.is_open())
    {
        char key = 'k'; // Ключ для XOR-шифрования

        string line;
        int lineNumber = 1;
        int selectedLine;
        bool lineEdited = false; // Флаг для отслеживания редактирования строки

        cout << "Доступные сервисы:" << endl;

        while (getline(file, line))
        {
            string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

            size_t commaPos1 = decryptedLine.find(",");
            size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

            string decryptedLogin = decryptedLine.substr(0, commaPos1);
            string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
            string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

            cout << lineNumber << ". " << decryptedServiceName << endl;

            lineNumber++;
        }

        file.close();
        file.open(FILE_NAME); // Переоткрываем файл для считывания данных с начала

        if (lineNumber == 1) {
            cout << "Файл пустой. Нет данных для редактирования." << endl;
            return; // Программа завершает выполнение
        }

        cout << "Введите номер сервиса, который необходимо отредактировать: ";
        cin >> selectedLine;

        int currentLine = 1;

        while (getline(file, line))
        {
            if (currentLine == selectedLine)
            {
                lineEdited = true; // Устанавливаем флаг редактирования строки

                string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

                size_t commaPos1 = decryptedLine.find(",");
                size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                string decryptedLogin = decryptedLine.substr(0, commaPos1);
                string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

                string editedLogin;
                string editedPassword;
                string editedServiceName;

                cout << "Текущие данные:" << endl;
                cout << "Логин: " << decryptedLogin << endl;
                cout << "Пароль: " << decryptedPassword << endl;
                cout << "Сервис: " << decryptedServiceName << endl;

                cout << "Введите новый логин (оставьте пустым, чтобы оставить текущее значение): ";
                cin.ignore();
                getline(cin, editedLogin);

                cout << "Введите новый пароль (оставьте пустым, чтобы сгенерировать новый): ";
                getline(cin, editedPassword);

                cout << "Введите новое название сервиса (оставьте пустым, чтобы оставить текущее значение): ";
                getline(cin, editedServiceName);

                if (editedLogin.empty())
                {
                    editedLogin = decryptedLogin;
                }

                if (editedPassword.empty())
                {
                    editedPassword = generatePassword(10);
                }

                if (editedServiceName.empty())
                {
                    editedServiceName = decryptedServiceName;
                }

                string encryptedLogin = encryptShift(encryptXOR(editedLogin, key), KEY_SHIFT);
                string encryptedPassword = encryptShift(encryptXOR(editedPassword, key), KEY_SHIFT);
                string encryptedServiceName = encryptShift(encryptXOR(editedServiceName, key), KEY_SHIFT);

                tempFile << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

                cout << "Данные успешно отредактированы." << endl;
            }
            else
            {
                tempFile << line << endl;
            }

            currentLine++;
        }

        file.close();
        tempFile.close();

        remove(FILE_NAME.c_str());

        // Переименовываем временный файл в оригинальное имя файла, только если строка была отредактирована
        if (lineEdited)
        {
            rename("temp.txt", FILE_NAME.c_str());
        }
        else
        {
            remove("temp.txt");
        }
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}


// Функция для удаления данных из файла
void deleteData()
{
    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");
    if (file.is_open() && tempFile.is_open())
    {
        char key = 'k'; // Ключ для XOR-шифрования

        string line;
        int lineNumber = 1;
        int selectedLine;

        cout << "Доступные сервисы:" << endl;

        while (getline(file, line))
        {
            string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

            size_t commaPos1 = decryptedLine.find(",");
            size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

            string decryptedLogin = decryptedLine.substr(0, commaPos1);
            string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
            string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

            cout << lineNumber << ". " << decryptedServiceName << endl;

            lineNumber++;
        }

        file.close();
        file.open(FILE_NAME); // Переоткрываем файл для считывания данных с начала

        if (lineNumber == 1) {
            cout << "Файл пустой. Нет данных для удаления." << endl;
            return; // Программа завершает выполнение
        }

        cout << "Введите номер сервиса, который необходимо удалить: ";
        cin >> selectedLine;

        int currentLine = 1;

        while (getline(file, line))
        {
            if (currentLine != selectedLine)
            {
                tempFile << line << endl;
            }

            currentLine++;
        }

        file.close();
        tempFile.close();

        remove(FILE_NAME.c_str());
        rename("temp.txt", FILE_NAME.c_str());

        cout << "Данные успешно удалены." << endl;
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

// Функция для шифрования файла
void encryptFile()
{
    ifstream inputFile(FILE_NAME);
    ofstream encryptedFile("encrypted.txt");

    if (inputFile.is_open() && encryptedFile.is_open())
    {
        char key1 = 'k'; // Ключ для первого XOR-шифрования
        char key2 = 's'; // Ключ для второго XOR-шифрования

        string line;

        while (getline(inputFile, line))
        {
            string encryptedLine = encryptXOR(encryptShift(line, KEY_SHIFT), key1);
            encryptedLine = encryptXOR(encryptedLine, key2);

            encryptedFile << encryptedLine << endl;
        }

        inputFile.close();
        encryptedFile.close();

        remove(FILE_NAME.c_str());

        cout << "Файл успешно зашифрован." << endl;
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

// Функция для дешифрования файла
void decryptFile()
{
    ifstream encryptedFile("encrypted.txt");
    ofstream decryptedFile("decrypted.txt");

    if (encryptedFile.is_open() && decryptedFile.is_open())
    {
        char key1 = 'k'; // Ключ для первого XOR-шифрования
        char key2 = 's'; // Ключ для второго XOR-шифрования

        string line;

        while (getline(encryptedFile, line))
        {
            string decryptedLine = decryptXOR(decryptXOR(line, key2), key1);
            decryptedLine = decryptShift(decryptedLine, KEY_SHIFT);

            decryptedFile << decryptedLine << endl;
        }

        encryptedFile.close();
        decryptedFile.close();

        remove("encrypted.txt");

        cout << "Файл успешно дешифрован." << endl;
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "RU");
    int choice;

    do
    {
        cout << "Выберите действие:" << endl;
        cout << "1. Ввод новых данных" << endl;
        cout << "2. Просмотр данных" << endl;
        cout << "3. Редактирование данных" << endl;
        cout << "4. Удаление данных" << endl;
        cout << "5. Выход" << endl;
        cout << "Введите номер действия: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addData();
            break;
        case 2:
            viewData();
            break;
        case 3:
            editData();
            break;
        case 4:
            deleteData();
            break;
        case 5:
            cout << "Завершение работы программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
            break;
        }
    } while (choice != 5);

    return 0;
}
