#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace std;

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
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',' && encryptedText[i] != 'B') // Исключаем символы 'l', 'L' и ','
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
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',' && encryptedText[i] != 'B') // Исключаем символы 'l', 'L' и ','
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
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',' && encryptedText[i] != 'B') // Исключаем символы 'l', 'L' и ','
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
        if (encryptedText[i] != 'l' && encryptedText[i] != 'L' && encryptedText[i] != ',' && encryptedText[i] != 'B') // Исключаем символы 'l', 'L' и ','
        {
            decryptedText[i] = (encryptedText[i] - shift + 256) % 256;
        }
    }

    return decryptedText;
}

// Экспериментальное шифрование
string encryptCaesar(const string& message)
{
    string encryptedMessage = message;

    for (size_t i = 0; i < message.length(); i++)
    {
        char originalChar = message[i];

        // Perform character shift based on the Caesar cipher
        if (isprint(originalChar) && originalChar != ',')
        {
            char shiftedChar = static_cast<char>((originalChar - 32 + 3) % 94 + 32);
            encryptedMessage[i] = shiftedChar;
        }
    }

    return encryptedMessage;
}

// Экспериментальное дешифрование
string decryptCaesar(const string& encryptedMessage)
{
    string decryptedMessage = encryptedMessage;

    for (size_t i = 0; i < encryptedMessage.length(); i++)
    {
        char encryptedChar = encryptedMessage[i];

        // Perform character shift based on the Caesar cipher
        if (isprint(encryptedChar) && encryptedChar != ',')
        {
            char shiftedChar = static_cast<char>((encryptedChar - 32 - 3 + 94) % 94 + 32);
            decryptedMessage[i] = shiftedChar;
        }
    }

    return decryptedMessage;
}

// Функция для добавления новых данных в файл
void addData(const string& fileName, const int& method)
{
    string login;
    string password;
    string serviceName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore until newline

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
            catch (const exception& e)
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

    ofstream file(fileName, ios::app);

    if (file.is_open())
    {
        char key = 'k'; // Ключ для XOR-шифрования

        file.seekp(0, ios::end);
        if (file.tellp() != 0)
        {
            file << endl;
        }
        if (method == 1) {
            string encryptedLogin = encryptShift(encryptXOR(login, key), KEY_SHIFT);
            string encryptedPassword = encryptShift(encryptXOR(password, key), KEY_SHIFT);
            string encryptedServiceName = encryptShift(encryptXOR(serviceName, key), KEY_SHIFT);

            file << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName;

            file.close();
            cout << "Данные успешно добавлены." << endl;
        }
        if (method == 2) {
            string encryptedLogin = encryptCaesar(login);
            string encryptedPassword = encryptCaesar(password);
            string encryptedServiceName = encryptCaesar(serviceName);

            file << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName;

            file.close();
            cout << "Данные успешно добавлены." << endl;
        }
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

void viewData(const string& fileName, const int& method)
{
    ifstream file(fileName);

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
            if (lineNumber > 1)
            {
                isEmpty = false; // Файл не является пустым
                if (method == 1) {
                    string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);
                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);
                    size_t serviceNameEnd = decryptedServiceName.find(",");
                    if (serviceNameEnd != string::npos) {
                        decryptedServiceName = decryptedServiceName.substr(0, serviceNameEnd);
                    }
                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl;
                }
                else if (method == 2) {
                    string decryptedLine = decryptCaesar(line);
                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);
                    size_t serviceNameEnd = decryptedServiceName.find(",");
                    if (serviceNameEnd != string::npos) {
                        decryptedServiceName = decryptedServiceName.substr(0, serviceNameEnd);
                    }
                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl;
                }

            }

            lineNumber++;
        }

        file.close();

        if (isEmpty) {
            cout << "Файл пустой." << endl;
            return; // Программа завершает выполнение
        }

        int selectedLine;

        cout << "Введите номер сервиса, чтобы просмотреть логин и пароль: ";
        cin >> selectedLine;
        selectedLine = selectedLine + 1;
        lineNumber = 1;
        file.open(fileName); // Переоткрываем файл для поиска выбранной строки

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (lineNumber == selectedLine)
                {
                    if (method == 1) {
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
                    else if (method == 2) {
                        string decryptedLine = decryptCaesar(line);
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
void editData(const string& fileName, const int& method)
{
    ifstream file(fileName);
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
            if (lineNumber > 1)
            {
                if (method == 1) {
                    string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);
                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedLogin = decryptedLine.substr(0, commaPos1);
                    string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl; 

                    lineNumber++;
                }
                if (method == 2) {
                    string decryptedLine = decryptCaesar(line);
                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedLogin = decryptedLine.substr(0, commaPos1);
                    string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl; 

                    lineNumber++;
                }

            }
            else
            {
                lineNumber++;
            }
        }

        file.close();
        file.open(fileName); // Переоткрываем файл для считывания данных с начала

        if (lineNumber <= 2) {
            cout << "Файл пустой или содержит только одну строку. Нет данных для редактирования." << endl;
            return; // Программа завершает выполнение
        }

        cout << "Введите номер сервиса, который необходимо отредактировать: ";
        cin >> selectedLine;

        int currentLine = 1;

        while (getline(file, line))
        {
            if (currentLine == selectedLine + 1) 
            {
                lineEdited = true; // Устанавливаем флаг редактирования строки
                if (method == 1) {
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
                    if (method == 1) {
                        string encryptedLogin = encryptShift(encryptXOR(editedLogin, key), KEY_SHIFT);
                        string encryptedPassword = encryptShift(encryptXOR(editedPassword, key), KEY_SHIFT);
                        string encryptedServiceName = encryptShift(encryptXOR(editedServiceName, key), KEY_SHIFT);

                        tempFile << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

                        cout << "Данные успешно отредактированы." << endl;
                    }
                    if (method == 2) {
                        string encryptedLogin = encryptCaesar(editedLogin);
                        string encryptedPassword = encryptCaesar(editedPassword);
                        string encryptedServiceName = encryptCaesar(editedServiceName);

                        tempFile << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

                        cout << "Данные успешно отредактированы." << endl;
                    }

                }
                if (method == 2) {
                    string decryptedLine = encryptCaesar(line);

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
                    if (method == 1) {
                        string encryptedLogin = encryptShift(encryptXOR(editedLogin, key), KEY_SHIFT);
                        string encryptedPassword = encryptShift(encryptXOR(editedPassword, key), KEY_SHIFT);
                        string encryptedServiceName = encryptShift(encryptXOR(editedServiceName, key), KEY_SHIFT);

                        tempFile << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

                        cout << "Данные успешно отредактированы." << endl;
                    }
                    if (method == 2) {
                        string encryptedLogin = encryptCaesar(editedLogin);
                        string encryptedPassword = encryptCaesar(editedPassword);
                        string encryptedServiceName = encryptCaesar(editedServiceName);

                        tempFile << encryptedLogin << "," << encryptedPassword << "," << encryptedServiceName << endl;

                        cout << "Данные успешно отредактированы." << endl;
                    }

                }

            }
            else
            {
                tempFile << line << endl;
            }

            currentLine++;
        }

        file.close();
        tempFile.close();

        remove(fileName.c_str());

        // Переименовываем временный файл в оригинальное имя файла, только если строка была отредактирована
        if (lineEdited)
        {
            rename("temp.txt", fileName.c_str());
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


void deleteData(const string& fileName, const int& method)
{
    ifstream file(fileName);
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
            if (lineNumber > 1) 
            {
                if (method == 1) {
                    string decryptedLine = decryptXOR(decryptShift(line, KEY_SHIFT), key);

                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedLogin = decryptedLine.substr(0, commaPos1);
                    string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl; 

                    lineNumber++;
                }
                if (method == 2) {
                    string decryptedLine = decryptCaesar(line);

                    size_t commaPos1 = decryptedLine.find(",");
                    size_t commaPos2 = decryptedLine.find(",", commaPos1 + 1);

                    string decryptedLogin = decryptedLine.substr(0, commaPos1);
                    string decryptedPassword = decryptedLine.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                    string decryptedServiceName = decryptedLine.substr(commaPos2 + 1);

                    cout << lineNumber - 1 << ". " << decryptedServiceName << endl; 

                    lineNumber++;
                }

            }
            else
            {
                lineNumber++;
            }
        }

        file.close();
        file.open(fileName); // Переоткрываем файл для считывания данных с начала

        if (lineNumber <= 2) {
            cout << "Файл пустой или содержит только одну строку. Нет данных для удаления." << endl;
            return; // Программа завершает выполнение
        }

        cout << "Введите номер сервиса, который необходимо удалить: ";
        cin >> selectedLine;

        int currentLine = 1;

        while (getline(file, line))
        {
            if (currentLine != selectedLine + 1) // Adjust the line number for deletion
            {
                tempFile << line << endl;
            }

            currentLine++;
        }

        file.close();
        tempFile.close();

        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        cout << "Данные успешно удалены." << endl;
    }
    else
    {
        cout << "Ошибка при открытии файла." << endl;
    }
}

vector<string> createdFiles;

void viewCreatedFiles()
{
    if (createdFiles.empty())
    {
        cout << "Нет созданных файлов." << endl;
    }
    else
    {
        cout << "Созданные файлы:" << endl;
        for (size_t i = 0; i < createdFiles.size(); i++)
        {
            cout << i + 1 << ". " << createdFiles[i] << endl;
        }
    }
}

string encryptPassword(const string& password, char key, const int& method)
{
    if (method == 1) {
        string encryptedPassword = encryptShift(encryptXOR(password, key), KEY_SHIFT); // Применение комбинированного шифрования
        return encryptedPassword;
    }
    if (method == 2) {
        string decryptedPassword = encryptCaesar(password);
        return decryptedPassword;
    }
}

string decryptPassword(const string& encryptedPassword, char key, const int& method)
{
    if (method == 1) {
        string decryptedPassword = decryptXOR(decryptShift(encryptedPassword, KEY_SHIFT), key); // Применение комбинированного дешифрования
        return decryptedPassword;
    }
    if (method == 2) {
        string decryptedPassword = decryptCaesar(encryptedPassword);
        return decryptedPassword;
    }
}

bool isPasswordCorrect(const string& fileName, const string& password, const int& method)
{
    char key = 'k';
    ifstream file(fileName);
    if (file.is_open())
    {
        string storedPassword;
        getline(file, storedPassword);

        file.close();

        // Расшифровка хранимого пароля перед сравнением
        string decryptedPassword = decryptPassword(storedPassword, key, method);

        // Сравнение введенного пароля с расшифрованным хранимым паролем
        if (password == decryptPassword(decryptedPassword, key, method))
        {
            return true;
        }
    }

    return false;
}

void saveCreatedFiles()
{
    ofstream file("created_files.txt");

    if (file.is_open())
    {
        for (const string& fileName : createdFiles)
        {
            file << fileName << endl;
        }

        file.close();
    }
    else
    {
        cout << "Ошибка при сохранении списка созданных файлов." << endl;
    }
}

void createEncryptedFile(const string& fileName, const string& password, const int& method)
{
    char key = 'k';
    ofstream file(fileName + ".txt");  // Открытие файла в текстовом режиме

    if (file.is_open())
    {
        if (method == 1) {
            // Шифрование пароля перед записью в файл
            string encryptedPassword = encryptPassword(password, key, method);

            // Записываем зашифрованный пароль в файл
            file << encryptedPassword;

            file.close();
            cout << "Файл " << fileName << ".txt" << " успешно создан и зашифрован паролем." << endl;
        }
        if (method == 2) {
            // Шифрование пароля перед записью в файл
            string encryptedPassword = encryptPassword(password, key, method);

            // Записываем зашифрованный пароль в файл
            file << encryptedPassword;

            file.close();
            cout << "Файл " << fileName << ".txt" << " успешно создан и зашифрован паролем." << endl;
        }
        // Добавляем имя файла в вектор созданных файлов
        createdFiles.push_back(fileName + ".txt");
        saveCreatedFiles();
    }
    else
    {
        cout << "Ошибка при создании файла " << fileName << ".txt" << "." << endl;
    }
}

void secondaryMenu(string fileName, const int& method)
{
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
            addData(fileName, method);
            break;
        case 2:
            viewData(fileName, method);
            break;
        case 3:
            editData(fileName, method);
            break;
        case 4:
            deleteData(fileName, method);
            break;
        case 5:
            cout << "Завершение работы с файлом." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
            break;
        }
    } while (choice != 5);
}

void openEncryptedFile(const string& fileName, const string& password, const int& method)
{
    char key = 'k';
    ifstream file(fileName);  // Добавляем расширение .txt к имени файла

    if (file.is_open())
    {
        string storedPassword;
        getline(file, storedPassword);

        // Расшифровываем хранимый пароль перед сравнением
        string decryptedPassword = decryptPassword(storedPassword, key, method);
        if (password == decryptedPassword){
                cout << "Файл " << fileName << " успешно открыт." << endl;
                secondaryMenu(fileName, method); // Передаем имя файла в secondaryMenu
        }
        else{
                cout << "Неверный пароль для файла " << fileName << "." << endl;
        }
        file.close();
    }
    else
    {
        cout << "Ошибка при открытии файла " << fileName << ".txt" << "." << endl;
    }
}

void viewCreatedFilesAndOpen()
{
    viewCreatedFiles();

    if (createdFiles.empty())
    {
        cout << "Нет созданных файлов." << endl;
        return;
    }

    cout << "Введите номер файла для открытия: ";
    int fileNumber;
    cin >> fileNumber;

    if (fileNumber >= 1 && fileNumber <= createdFiles.size())
    {
        string fileName = createdFiles[fileNumber - 1];

        string password;
        cout << "Введите пароль для файла " << fileName << ": ";
        cin >> password;
        int method = 0;
        do {
            cout << "Выберите метод расшифровки:" << endl;
            cout << "1. Библиотечный" << endl;
            cout << "2. Экспериментальный" << endl;
            cin >> method;
        } while (method != 1 && method != 2);
        openEncryptedFile(fileName, password, method);
    }
    else
    {
        cout << "Неверный номер файла." << endl;
    }
}

void loadCreatedFiles()
{
    ifstream file("created_files.txt");

    if (file.is_open())
    {
        string fileName;
        while (getline(file, fileName))
        {
            createdFiles.push_back(fileName);
        }

        file.close();
    }
    else
    {
        cout << "Ошибка при загрузке списка созданных файлов." << endl;
    }
}

void deleteFile(const string& fileName) {
    if (remove(fileName.c_str()) == 0) {
        cout << "Файл " << fileName << " успешно удален." << endl;
        // Удаление имени файла из вектора созданных файлов      
        for (auto it = createdFiles.begin(); it != createdFiles.end(); ++it)
        {
            if (*it == fileName)
            {
                createdFiles.erase(it);
                break;
            }
        }
        saveCreatedFiles();
    }
    else {
        cout << "Ошибка при удалении файла " << fileName << "." << endl;
    }
}

void FdeleteData() {
    viewCreatedFiles();
    if (createdFiles.empty()) {
        cout << "Нет созданных файлов." << endl;        return;
    }
    cout << "Введите номер файла для удаления: ";    int fileNumber;
    cin >> fileNumber;
    if (fileNumber >= 1 && fileNumber <= createdFiles.size()) {
        string fileName = createdFiles[fileNumber - 1];        deleteFile(fileName);
    }
    else
    {
        cout << "Неверный номер файла." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    loadCreatedFiles();
    int choice1;
    int method = 0;
    do {
        cout << "Выберите действие:" << endl;
        cout << "1. Просмотреть созданные файлы" << endl;
        cout << "2. Создать новый файл" << endl;
        cout << "3. Открыть файл" << endl;
        cout << "4. Удалить файл" << endl;
        cout << "5. Выход " << endl;
        cout << "Введите номер действия: ";
        cin >> choice1;
        string fileName;
        string password;
        switch (choice1) {
        case 1:
            viewCreatedFiles();
            break;
        case 2:
            viewCreatedFiles();
            cout << "Введите имя файла: ";
            cin.ignore(); // Очистка буфера перед вводом имени файла     
            getline(cin, fileName);
            cout << "Введите пароль: ";
            getline(cin, password);
            do {
                cout << "Выберите метод шифрования: " << endl;
                cout << "1. Библиотечный" << endl;
                cout << "2. Экспериментальный" << endl;
                cin >> method;
            } while (method != 1 && method != 2);
            createEncryptedFile(fileName, password, method);
            break;
        case 3:
            viewCreatedFilesAndOpen();
            break;
        case 4:
            FdeleteData();
            break;
        case 5: 
            cout << "Завершение работы программы." << endl;
            break;        
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;            
            break;
        }
        // Проверка пароля и запуск второго цикла 
        {
            secondaryMenu(fileName, method);
        }
    } while (choice1 != 5);
    return 0;
}
