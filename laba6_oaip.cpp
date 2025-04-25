#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

struct Person
{
    int info;
    char fio[20];
};

struct Tree
{
    int info;
    char fio[20];
    Tree* left, * right;
};

void CreateTree(Tree** root);
Tree* List(int in, char* fio);
void AddList(Tree* root, int in, char* fio); //добавление в дерево
void DeleteTree(Tree** root, int key); //удалить дерево
void DeleteAllTree(Tree* root); //полностью удалить
void View1(Tree* root);
void View2(Tree* root);
void View3(Tree* root);
void Search(Tree* root, int key); //поиск в дереве
void Ind(Tree* root, char* letter, int* kolvo);
void MakeBalance(Tree** root, int n, int k, Person* arr);
void KolvoElementov(Tree* root, int* kolvo);
void TreeToArr(Tree* root, Person* arr, int* n);
void ViewTree(Tree* p, int level);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    Tree* root = NULL;
    int k = 0, kod, n, kolvo = 0;

    while (true)
    {
        char fio[20];
        cout << "1. Создание дерева\n2. Внести информацию из массива в дерево поиска\n3. Сбалансировать дерево поиска\n4. Добавить в дерево поиска новую запись\n"
            "5. Поиск по заданному паспорту\n6. Удалить из дерева поиска информацию с заданным паспортом\n7. Распечатать информацию прямым, обратным обходом и в порядке возрастания ключа\n"
            "8. Кол-во записей в дереве начинающихся с выбранной буквы\n9. Просмотр красивого дерева\n0. EXIT" << endl;
        cin >> kod;
        switch (kod)
        {
        case 1:
        {
            CreateTree(&root); break;
        }
        case 2:
        {
            cout << "Количество элементов:" << endl;
            cin >> kod;
            Person* arr = new Person[kod];
            for (int i = 0; i < kod; i++) //заполнили массив
            {
                cout << "Имя:" << endl;
                cin.ignore(100, '\n');
                cin.getline(fio, 20);
                cout << "Паспорт:" << endl;
                cin >> n;
                arr[i].info = n;
                strcpy(arr[i].fio, fio); // суем строку в структуру
            }
            for (int i = 0; i < kod; i++)//создали дерева по данным из массива
            {
                AddList(root, arr[i].info, arr[i].fio);
            }
            delete[]arr;
            cout << "Сделано!\n";
            break;
        }
        case 3:
        {
            n = 0;
            k = 0;
            KolvoElementov(root, &k);//нашли количество элементов
            Person* arr = new Person[k];
            TreeToArr(root, arr, &n);//заполнили массив(сразу отсортирован по паспорту)
            DeleteAllTree(root); //удалили прошлое дерево
            root = NULL;
            MakeBalance(&root, 0, k, arr);//k-размер массива
            cout << "Дерево сбалансировано!" << endl;
            delete[]arr;
            break;
        }
        case 4:
        {
            cout << "Имя:" << endl;
            cin.ignore(100, '\n');
            cin.getline(fio, 20);
            cout << "Паспорт:" << endl;
            cin >> n;
            AddList(root, n, fio);
            break;
        }
        case 5:
        {
            cout << "Введите паспорт:" << endl;
            cin >> kod;
            Search(root, kod);
            break;
        }
        case 6:
        {
            cout << "Введите номер паспорта, который нужно удалить из дерева:" << endl;
            cin >> n;
            DeleteTree(&root, n);
            cout << "Успешно!" << endl;
            break;
        }
        case 7:
        {
            cout << "1. Центрированный обход(лево-корень-право)\n2. Прямой обход(лево снизу по уровням до корня - корень - право...)\n3. Обратный обход(сверху вниз)\n";
            cin >> kod;
            switch (kod)
            {
            case 1: View1(root); break;
            case 2: View2(root); break;
            case 3: View3(root); break;
            default: cout << "Нет такого пункта!\n";
            }
            break;
        }
        case 8:
        {
            char letter;
            cout << "Введите букву: ";
            cin >> letter;
            Ind(root, &letter, &kolvo);
            cout << "Количество записей в дереве начинающихся с буквы " << letter << " : " << kolvo << endl << endl;
            kolvo = 0;
            break;
        }
        case 9:
            ViewTree(root, 0); break;
        case 0:
        {
            return 0;
        }
        default: cout << "Нет такого пункта!\n";
        }
    }
}

void CreateTree(Tree** root)
{
    int n;
    char fio[20];
    if ((*root) == NULL)
    {
        cout << "Корень:\nИмя:\n";
        cin.ignore(100, '\n');
        cin.getline(fio, 20);
        cout << "Паспорт:\n";
        cin >> n;
        (*root) = List(n, fio);
        cout << "Корень создан!\n";
    }
    else
    {
        cout << "Дерево уже создано!\n";
    }
}

Tree* List(int in, char* fio)
{
    Tree* t = new Tree;
    t->info = in;
    strcpy(t->fio, fio);
    t->left = NULL;
    t->right = NULL;
    return t;
}

void AddList(Tree* root, int in, char* fio)
{
    Tree* prev = nullptr, * t = root; //prev=nullptr чтобы исправить ошибку:
    //использование потенциально неициализированной памяти prev
    while (t)
    {
        prev = t;
        if (in == t->info)
        {
            cout << "Такой паспорт уже есть!\n";
            return;
        }
        if (in > t->info) t = t->right;
        else t = t->left;
    }
    t = List(in, fio);
    if (in > prev->info) prev->right = t;
    else prev->left = t;
}

void DeleteTree(Tree** root, int key)
{
    Tree* Del, * PrevDel, * R, * PrevR;
    Del = *root;
    PrevDel = NULL;

    while (Del != NULL && Del->info != key) //поиск удаляемого элемента и его предка
    {
        PrevDel = Del;
        if (Del->info > key) Del = Del->left;
        else Del = Del->right;
    }
    if (Del == NULL)
    {
        cout << "Элемент не найден!\n";
        return;
    }

    if (Del->right == NULL) R = Del->left; //поиск элемента для замены
    else
    {//ищем самое правое в левом поддереве
        PrevR = Del;
        R = Del->left;
        while (R->right != NULL)
        {
            PrevR = R;
            R = R->right;
        }
        //нашли элемент для замены R и его предка PrevR
        if (PrevR == Del) R->right = Del->right;
        else
        {
            R->right = Del->right;
            PrevR->right = R->left;
            R->left = PrevR;
        }
    }

    if (Del == *root) *root = R;
    else
    {
        if (Del->info < PrevDel->info) PrevDel->left = R;
        else PrevDel->right = R;
    }
    delete Del;
}

void View1(Tree* root) //слева направо
{
    if (root != NULL)
    {
        View1(root->left);
        cout << "Имя: " << root->fio << "\tПаспорт: " << root->info << endl;
        View1(root->right);
    }
}

void View2(Tree* root) //по дереву сверху вниз, префиксаная форма
{
    if (root != NULL)
    {
        View2(root->left);
        View2(root->right);
        cout << "Имя: " << root->fio << "\tПаспорт: " << root->info << endl;
    }
}

void View3(Tree* root) //по дереву снизу вверх
{
    if (root != NULL)
    {
        cout << "Имя: " << root->fio << "\tПаспорт: " << root->info << endl;
        View3(root->left);
        View3(root->right);
    }
}

void Search(Tree* root, int key)
{
    if (root == NULL) cout << "Дерево пусто!\n";
    while (root != NULL)
    {
        if (root->info > key) root = root->left;
        else
        {
            if (root->info < key) root = root->right;
            else
            {
                cout << "Имя: " << root->fio << "\tПаспорт: " << root->info << endl;
                return;
            }
        }
    }
}

void Ind(Tree* root, char* letter, int* kolvo)
{
    if (root != NULL)
    {
        if (root->fio[0] == *letter) (*kolvo)++;
        Ind(root->left, letter, kolvo);
        Ind(root->right, letter, kolvo);
    }
}

void KolvoElementov(Tree* root, int* kolvo)
{
    if (root != NULL)
    {
        (*kolvo)++;
        KolvoElementov(root->left, kolvo);
        KolvoElementov(root->right, kolvo);
    }
}

void TreeToArr(Tree* root, Person* arr, int* n)
{
    if (root != NULL)
    {
        TreeToArr(root->left, arr, n);

        strcpy(arr[*n].fio, root->fio);
        arr[*n].info = root->info;
        (*n)++;
        TreeToArr(root->right, arr, n);
    }
}

void MakeBalance(Tree** root, int n, int k, Person* arr)
{
    if (n == k)
    {
        *root = NULL;
        return;
    }
    else
    {
        int mid = (n + k) / 2;
        *root = new Tree;
        (*root)->info = arr[mid].info;
        strcpy((*root)->fio, arr[mid].fio);
        MakeBalance(&((*root)->left), n, mid, arr);
        MakeBalance(&((*root)->right), mid + 1, k, arr);
    }
}

void DeleteAllTree(Tree* root)
{
    if (root != NULL)
    {
        DeleteAllTree(root->left);
        DeleteAllTree(root->right);
        delete root;
    }
}

void ViewTree(Tree* p, int level)
{
    string str;
    if (p)
    {
        ViewTree(p->right, level + 1);
        for (int i = 0; i < level; i++) str += "\t";
        cout << str << p->info << endl;
        ViewTree(p->left, level + 1);
    }
}